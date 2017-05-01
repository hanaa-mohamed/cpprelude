#pragma once

#include "cpprelude\defines.h"
#include "cpprelude\dynamic_array.h"


namespace cpprelude
{
	
	struct quick_find
	{
		dynamic_array<isize> _arr;

		quick_find(){}

		quick_find(isize count)
		{
			_arr.reserve(count);
			for (usize i = 0; i < count; ++i)
				_arr.insert_back(i);
		}

		bool
		connected(isize x, isize y)
		{
			if (_arr[x] == _arr[y])
				return true;

			return false;
		}

		void
		union1(isize x, isize y)
		{
			
			if (_arr[x] != _arr[y])
			{
				isize temp = _arr[y];
				for (usize i = 0; i < _arr.count(); ++i)
					if (_arr[i] == temp)
						_arr[i] = _arr[x];

			}
		}
	};
}