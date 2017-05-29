#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T,
			 usize bucket_size,
			 typename AllocatorT = global_allocator>
	struct bucket_array
	{
		using data_type = T;
		
		owner_mem_block _memory;
		AllocatorT _allocator;
		handle<bucket_array> _next;

		bucket_array(const AllocatorT& allocator = AllocatorT())
			:_allocator(allocator)
		{ _memory = _allocator.alloc(bucket_size * sizeof(T)); }

		bucket_array(const bucket_array&) = delete;
		
		bucket_array&
		operator=(const bucket_array&) = delete;

		~bucket_array()
		{
			if(_memory.ptr != nullptr && _memory.size > 0)
				_allocator.free(_memory);

			if(_next)
			{
				_next.destroy();
				_allocator.free(resurrect(_next));
			}
		}

		T&
		operator[](usize index)
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return *it->_memory.template at<T>(index);
			}

			return *_memory.template at<T>(index);
		}

		const T&
		operator[](usize index) const
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return *it->_memory.template at<T>(index);
			}

			return *_memory.template at<T>(index);
		}

		T*
		at(usize index)
		{
			if(index >= bucket_size)
			{
				auto it = _next;
				index -= bucket_size;
				while(index >= bucket_size)
				{
					it = it->_next;
					index -= bucket_size;
				}
				return *it->_memory.template at<T>(index);
			}

			return _memory.template at<T>(index);
		}

		void
		expand()
		{
			if(_next)
				_next->expand();
			else
				_next.construct(_allocator.alloc(sizeof(bucket_array)).template decay<bucket_array>(), _allocator);
		}
	};
}
