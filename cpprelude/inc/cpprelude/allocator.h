#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct global_allocator
	{
		template<typename T>
		slice<T>
		alloc(usize count = 1, ubyte alignment = 4)
		{
			return cpprelude::alloc<T>(count, alignment);
		}

		template<typename T>
		void
		free(slice<T>& slice_)
		{
			cpprelude::free<T>(slice_);
		}

		template<typename T>
		void
		free(slice<T>&& slice_)
		{
			cpprelude::free<T>(tmp::move(slice_));
		}

		template<typename T>
		void
		realloc(slice<T>& slice_, usize count)
		{
			cpprelude::realloc<T>(slice_, count);
		}

		template<typename T>
		void
		realloc(slice<T>&& slice_, usize count)
		{
			cpprelude::realloc<T>(tmp::move(slice_), count);
		}
	};

	template<typename allocator_type>
	struct ref_allocator
	{
		allocator_type* _allocator;

		ref_allocator()
			:_allocator(nullptr)
		{}

		ref_allocator(allocator_type* allocator)
			:_allocator(allocator)
		{}

		~ref_allocator()
		{
			_allocator = nullptr;
		}

		template<typename T>
		slice<T>
		alloc(usize count = 1, ubyte alignment = 4)
		{
			return _allocator->template alloc<T>(count, alignment);
		}

		template<typename T>
		void
		free(slice<T>& slice_)
		{
			_allocator->template free<T>(slice_);
		}

		template<typename T>
		void
		free(slice<T>&& slice_)
		{
			_allocator->template free<T>(tmp::move(slice_));
		}

		template<typename T>
		void
		realloc(slice<T>& slice_, usize count)
		{
			_allocator->template realloc<T>(slice_, count);
		}

		template<typename T>
		void
		realloc(slice<T>&& slice_, usize count)
		{
			_allocator->template alloc<T>(tmp::move(slice_), count);
		}
	};

	struct linear_allocator
	{
		slice<ubyte> _memory;
		usize _alloc_head, _alloc_count;

		linear_allocator()
			:_alloc_count(0), _alloc_head(0)
		{}

		template<typename T>
		linear_allocator(slice<T> memory)
			:_memory(reinterpret_cast<ubyte*>(memory.ptr), memory.size),
			 _alloc_count(0), _alloc_head(0)
		{}

		~linear_allocator()
		{
			_memory.ptr = nullptr;
			_memory.size = 0;
			_alloc_head = 0;
			_alloc_count = 0;
		}

		template<typename T>
		slice<T>
		alloc(usize count = 1, ubyte alignment = 4)
		{
			if(count > 0 && _alloc_head + count * sizeof(T) <= _memory.size)
			{
				++_alloc_count;

				slice<T> result(reinterpret_cast<T*>(&_memory[_alloc_head]), count * sizeof(T));

				_alloc_head += count * sizeof(T);

				return result;
			}

			return slice<T>();
		}

		template<typename T>
		void
		free(slice<T>& slice_)
		{
			linear_allocator::free(tmp::move(slice_));
		}

		template<typename T>
		void
		free(slice<T>&& slice_)
		{
			ubyte* memory_end = _memory.ptr + _memory.size;

			if(reinterpret_cast<ubyte*>(slice_.ptr) >= _memory.ptr && reinterpret_cast<ubyte*>(slice_.ptr) < memory_end)
			{
				ubyte* latest_block = _memory.ptr + _alloc_head - slice_.size;

				//if this is the last allocated block then move the head back
				if(reinterpret_cast<ubyte*>(slice_.ptr) == latest_block)
					_alloc_head -= slice_.size;

				slice_.ptr = nullptr;
				slice_.size = 0;
				--_alloc_count;

				//if no other allocations exist then free the whole stack
				if(_alloc_count == 0)
					_alloc_head = 0;
			}
		}

		template<typename T>
		void
		realloc(slice<T>& slice_, usize count)
		{
			linear_allocator::realloc(tmp::move(slice_), count);
		}

		template<typename T>
		void
		realloc(slice<T>&& slice_, usize count)
		{
			ubyte* memory_end = _memory.ptr + _memory.size;

			//if i own this block
			if(reinterpret_cast<ubyte*>(slice_.ptr) >= _memory.ptr && reinterpret_cast<ubyte*>(slice_.ptr) < memory_end)
			{
				//if the block is the latest block allocated then we can just expand it
				ubyte* latest_block = _memory.ptr + _alloc_head - slice_.size;

				if(reinterpret_cast<ubyte*>(slice_.ptr) == latest_block)
				{
					_alloc_head += (count * sizeof(T)) - slice_.size;
					slice_.size = (count * sizeof(T));
					return;
				}

				//else just allocate and copy data
				slice<T> new_slice = linear_allocator::alloc<T>(count);
				usize limit = slice_.count() < count ? slice_.count() : count;

				//copy the data from the old to the new
				for(usize i = 0; i < limit; ++i)
					new_slice[i] = tmp::move(slice_[i]);

				//now free the old
				linear_allocator::free(slice_);

				//move the new_slice to the old slice
				slice_ = tmp::move(new_slice);
			}
		}

		void
		reset()
		{
			_alloc_head = 0;
			_alloc_count = 0;
		}
	};

	API	linear_allocator
	make_arena_allocator(usize arena_size, slice<ubyte>& mem_block);

	API linear_allocator
	make_stack_allocator(ubyte* ptr, usize size);
}
