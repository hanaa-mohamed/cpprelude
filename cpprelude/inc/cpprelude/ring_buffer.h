#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T,
			 usize bucket_size,
			 typename AllocatorT = global_allocator>
	struct ring_buffer
	{
		bucket_array<T, bucket_size, AllocatorT> _array;
		usize _count, _write_head, _read_head;
		

		ring_buffer(const AllocatorT& allocator = AllocatorT())
			:_count(bucket_size), _write_head(0), _read_head(-1)
		{}

		bool
		write(const T& value)
		{
			if(_write_head != _read_head)
			{
				_array[_write_head] = value;
				_write_head = (_write_head + 1) % _count;
				return true;
			}
			return false;
		}

		void
		write(T&& value)
		{
			if(_write_head != _read_head)
			{
				_array[_write_head] = tmp::move(value);
				_write_head = (_write_head + 1) % _count;
				return true;
			}
			return false;
		}

		bool
		read(T& value)
		{
			if(_read_head != _
		}
	};
}
