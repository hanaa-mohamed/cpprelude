#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T,
			 usize bucket_size = 128,
			 typename AllocatorT = global_allocator>
	struct bucket_list
	{
		using data_type = T;
		
		owner_mem_block _memory;
		AllocatorT _allocator;
		handle<bucket_list> _next;

		bucket_list(const AllocatorT& allocator = AllocatorT())
			:_allocator(allocator)
		{ _memory = _allocator.alloc(bucket_size * sizeof(T)); }

		bucket_list(const bucket_list&) = delete;
		
		bucket_list&
		operator=(const bucket_list&) = delete;

		~bucket_list()
		{
			if(_memory.ptr != nullptr && _memory.size > 0)
				_allocator.free(_memory);

			if(_next)
			{
				_next.destroy();
				_allocator.free(_next);
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
			{
				_next = _allocator.template alloc<bucket_list>();
				_next.construct_inplace(_allocator);
			}
		}
	};
}
