#include "cpprelude/quick_find.h"

namespace cpprelude{

	quick_find::quick_find()
	{}

	quick_find::quick_find(usize count)
	{
		_arr.resize(count);
		for (usize i = 0; i < count; ++i)
			_arr[i] = i;
	}

	bool
	quick_find::is_connected(usize x, usize y)
	{
			if (_arr[x] == _arr[y])
				return true;

			return false;
		}

	void
	quick_find::connect(usize x, usize y)
	{

			if (_arr[x] != _arr[y])
			{
				usize temp = _arr[y];
				for (usize i = 0; i < _arr.count(); ++i)
				if (_arr[i] == temp)
					_arr[i] = _arr[x];

			}
	}

	usize 
	quick_find::count() const
	{
			return _arr.count();
	}
}