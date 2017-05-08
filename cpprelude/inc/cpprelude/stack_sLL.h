#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/slinked_list.h"


namespace cpprelude
{
	template<typename T>
	struct stack_sLL
	{
		slinked_list<T> _list;
		
		stack_sLL()
		{}

		stack_sLL(const stack_sLL<T>& other)
			:_list(other._list)
		{}

		stack_sLL(stack_sLL<T>&& other)
			:_list(tmd::move(other._list))
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
		//Maybe remove it as you can type stack_sLLObj._list.count();
		//But for unifying between stack_array and stack_sLL we can leave it like this.
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