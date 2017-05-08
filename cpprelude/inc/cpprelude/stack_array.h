#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"


namespace cpprelude
{
	template<typename T>
	struct stack_array
	{
		dynamic_array<T> _array;
		usize _count;

		stack_array()
			:_count(0)
		{}

		stack_array(usize count)
			:_count(0), _array(count)
		{}

		stack_array(const stack_array<T>& other)
			:_count(other._count), _array(other._array)
		{}

		stack_array(stack_array<T>&& other)
			:_count(other._count), _array(tmp::move(other._array))
		{}

		void
		push(const T& item)
		{
			if (_count == _array.count())
				_array.insert_back(item);
			else
				_array[_count] = item;

			++_count;
		}

		void
		push(T&& item)
		{
			if (_count == _array.count())
				_array.insert_back(tmp::move(item));
			else
				_array[_count] = tmp::move(item);

			++_count;
		}
		
		const T&
		top() const
		{
			return _array[_count-1];
		}

		T&
		top()
		{
			return _array[_count-1];
		}

		bool
		pop()
		{
			if(_count > 0)
			{
				--_count;
				//maybe shrink the array here
				return true;
			}

			return false;
		}

		bool
		empty()
		{
			return (_count == 0);
		}

		usize
		count()
		{
			return _count;
		}

	};

}
