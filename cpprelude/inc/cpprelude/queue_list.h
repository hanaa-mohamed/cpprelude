#pragma once
#include "cpprelude/dlinked_list.h"

namespace cpprelude
{
	template<typename T>
	struct queue_list
	{
		dlinked_list<T> _list;

		queue_list()
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

		const T&
		back() const
		{
			return *_list.back();
		}

		T&
		back() 
		{
			return *_list.back();
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