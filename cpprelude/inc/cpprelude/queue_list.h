#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dlinked_list.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct queue_list
	{
		dlinked_list<T, AllocatorT> _list;

		queue_list(const AllocatorT& allocator = AllocatorT())
			:_list(allocator)
		{}

		queue_list(const queue_list&) = default;

		queue_list(queue_list&&) = default;

		queue_list(const queue_list& other, const AllocatorT& allocator)
			:_list(other._list, allocator)
		{}

		queue_list(queue_list&& other, const AllocatorT& allocator)
			:_list(tmp::move(other._list), allocator)
		{}

		void
		enqueue(const T& item)
		{
			_list.insert_back(item);
		}

		void
		enqueue(T&& item)
		{
			_list.insert_back(tmp::move(item));
		}

		bool
		dequeue()
		{
			if (_list.count() > 0)
			{
				_list.remove_front();
				return true;
			}

			return false;
		}

		const T&
		front() const
		{
			return *_list.front();
		}

		T&
		front() 
		{
			return *_list.front();
		}

		bool
		empty()
		{
			return _list.empty();
		}

		usize
		count()
		{
			return _list.count();
		}

	};
}