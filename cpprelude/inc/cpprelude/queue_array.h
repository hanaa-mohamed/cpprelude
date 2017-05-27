#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/ring_buffer.h"
#include "cpprelude/allocator.h"
#include "cpprelude/tmp.h"

namespace cpprelude
{
	template<typename T, usize buffer_size = 128, typename AllocatorT = global_allocator>
	struct queue_array
	{
		ring_buffer<T, buffer_size, AllocatorT> _buffer;
		usize _count;

		queue_array(const AllocatorT& allocator = AllocatorT())
			:_count(0), _buffer(allocator)
		{}

		void
		enqueue(const T& item)
		{
			if(!_buffer.push(item))
			{
				_buffer.expand();
				_buffer.push(item);
			}
			
			++_count;
		}

		void
		enqueue(T&& item)
		{
			if(!_buffer.push(tmp::move(item)))
			{
				_buffer.expand();
				_buffer.push(tmp::move(item));
			}
			
			++_count;
		}

		bool
		dequeue()
		{
			bool result =  _buffer.pop();
			if(result)
				--_count;
			return result;
		}

		T
		front() const
		{
			return _buffer.front();
		}

		T&
		front()
		{
			return _buffer.front();
		}

		bool
		empty()
		{
			return _count == 0;
		}

		usize
		count()
		{
			return _count;
		}

	};
}
