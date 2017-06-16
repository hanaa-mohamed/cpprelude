#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct API linear_allocator
	{
		mem_block _memory;
		usize _alloc_head, _alloc_count;

		linear_allocator();
		explicit linear_allocator(const mem_block& memory);
		explicit linear_allocator(mem_block&& memory);
		~linear_allocator();

		owner_mem_block
		alloc(usize size, ubyte alignment = 4);

		void
		realloc(owner_mem_block& block, usize size);

		void
		free(owner_mem_block& block);

		void
		free(owner_mem_block&& block);

		//handles stuff
		template<typename T>
		handle<T>
		alloc(usize count = 1)
		{
			usize size = sizeof(T) * count;
			if(size > 0 && _alloc_head + size <= _memory.size)
			{
				++_alloc_count;
				handle<T> result(reinterpret_cast<T*>(reinterpret_cast<ubyte*>(_memory.ptr)+_alloc_head));

				_alloc_head += size;

				return result;
			}

			return handle<T>();
		}

		template<typename T>
		void
		free(handle<T>&& value, usize count = 1)
		{
			void* memory_end = reinterpret_cast<ubyte*>(_memory.ptr) + _memory.size;

			if(value.value_ptr >= _memory.ptr && value.value_ptr < memory_end)
			{
				void* latest_block = reinterpret_cast<ubyte*>(_memory.ptr) + _alloc_head - (sizeof(T) * count);

				//if this is the last allocated block then move the head back
				if(value.value_ptr == latest_block)
					_alloc_head -= sizeof(T) * count;

				value.value_ptr = nullptr;
				--_alloc_count;

				//if no other allocations exist then free the whole stack
				if(_alloc_count == 0)
					_alloc_head = 0;
			}
		}

		template<typename T>
		void
		free(handle<T>& value, usize count = 1)
		{
			free(tmp::move(value), count);
		}
	};

	struct API global_allocator
	{
		owner_mem_block
		alloc(usize size, ubyte alignment = 4);

		void
		realloc(owner_mem_block& block, usize size);

		void
		free(owner_mem_block& block);

		void
		free(owner_mem_block&& block);

		//handles stuff
		template<typename T>
		handle<T>
		alloc(usize count = 1)
		{
			return cpprelude::alloc<T>(count);
		}

		template<typename T>
		void
		free(handle<T>&& value, usize count = 1)
		{
			cpprelude::free(tmp::move(value), count);
		}

		template<typename T>
		void
		free(handle<T>& value, usize count = 1)
		{
			cpprelude::free(tmp::move(value), count);
		}
	};

	API linear_allocator
	make_arena_allocator(usize arena_size, owner_mem_block& mem_block);

	API linear_allocator
	make_stack_allocator(ubyte* ptr, usize size);

}
