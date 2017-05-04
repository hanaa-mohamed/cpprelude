#include "..\..\inc\cpprelude\stack_array.h"
#pragma once
namespace cpprelude 
{
	cpprelude::stack_array::stack_array():_count(0)
	{
		
	}

	cpprelude::stack_array::stack_array(usize count):_count(0)
	{
		_array.resize(count);
	}
	void stack_array::push(usize item)
	{
		if(_count == _array.count())
			_array.insert_back(item);
		else
			_array[_count]=item;

		_count++;
	}

	usize stack_array::pop()
	{
		
		usize x;
		if (_count > 0) {
			x = _array[_count - 1];
			//Shrinking array size with factor 25%,
			//Most probably it will never enter this case! due to increase factor 
			//in dynamic_array 1.5

			if (_count == _array.count() / 4.0)
				_array.resize(_array.count() / 2);
		}
		else
			x = usize();

		_count--;
		return x;
	}

	bool stack_array::isEmpty()
	{
		return (_count==0);
	}

	usize stack_array::size()
	{
		return _count;
	}

	usize stack_array::capacity()
	{
		return _array.capacity();
	}
}
