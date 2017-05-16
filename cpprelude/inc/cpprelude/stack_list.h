#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/slinked_list.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	template<typename T, typename AllocatorT = global_allocator>
	struct stack_list
	{
		slinked_list<T, AllocatorT> _list;
		
		stack_list(const AllocatorT& allocator = AllocatorT())
			:_list(allocator)
		{}

		void
		push(const T& item)
		{
			_list.insert_front(item);
		}

		void
		push(T&& item)
		{
			_list.insert_front(tmp::move(item));
		}

		const T&
		top() const
		{
			return *_list.begin();
		}

		T&
		top()
		{
			return *_list.begin();
		}

		bool
		pop()
		{
			if (_list.count() > 0)
			{
				_list.remove_front();
				return true;
			}

			return false;
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