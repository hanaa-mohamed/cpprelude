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
				_allocator.free(_next.memory_block);
			}
		}

		T&
		operator[](usize index)
		{
			if(index >= bucket_size)
				return _next->operator[](index - bucket_size);

			return *_memory.template at<T>(index);
		}

		const T&
		operator[](usize index) const
		{
			if(index >= bucket_size)
				return _next->operator[](index - bucket_size);

			return *_memory.template at<const T>(index);
		}

		T*
		at(usize index)
		{
			if(index >= bucket_size)
				return _next->operator[](index - bucket_size);
			return _memory.template at<T>(index);
		}

		void
		expand()
		{
			if(_next)
				_next->expand();
			else
				_next.construct(_allocator.alloc(sizeof(bucket_array)), _allocator);
		}
	};
}
