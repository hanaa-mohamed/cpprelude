#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/allocator.h"
#include "cpprelude/bucket_array.h"

namespace cpprelude
{
	template<typename T,
			 usize bucket_size,
			 typename AllocatorT = global_allocator>
	struct ring_buffer
	{
		bucket_array<T, bucket_size, AllocatorT> _array;
		usize _capacity, _write_head, _read_head;

		ring_buffer(const AllocatorT& allocator = AllocatorT())
			:_capacity(bucket_size),
			 _write_head(0),
			 _read_head(0),
			 _array(allocator)
		{}

		bool
		push(const T& value)
		{
			auto write_position = (_write_head + 1) % _capacity;
			if(write_position != _read_head)
			{
				_array[_write_head] = value;
				_write_head = write_position;
				return true;
			}
			return false;
		}

		bool
		push(T&& value)
		{
		    auto write_position = (_write_head + 1) % _capacity;
			if(write_position != _read_head)
			{
				_array[_write_head] = value;
				_write_head = write_position;
				return true;
			}
			return false;
		}

		T&
		front()
		{
			return _array[_read_head];
		}

		T
		front() const
		{
			return _array[_read_head];
		}

		bool
		pop()
		{
			if (_read_head !=  _write_head)
			{
				_read_head = (_read_head + 1) % _capacity;
				return true;
			}

			return false;
		}

		usize
		capacity() const
		{
			return _capacity;
		}

		bool
		expand()
		{
			//if the data isn't wrapped around the end where we will expand
			if(_write_head < _read_head)
				return false;
			_array.expand();
			_capacity += bucket_size;
			return true;
		}
	};
}
