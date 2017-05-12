#include "cpprelude/quick_find.h"

namespace cpprelude{

	quick_find::quick_find()
	{}

	quick_find::quick_find(usize count)
	{
		_nodes.expand_back(count);
		for (usize i = 0; i < count; ++i)
			_nodes[i] = i;
	}

	bool
	quick_find::is_connected(usize x, usize y)
	{
		return _nodes[x] == _nodes[y];
	}

	void
	quick_find::connect(usize x, usize y)
	{
		if (_nodes[x] != _nodes[y])
		{
			auto temp = _nodes[y];
			for (usize i = 0; i < _nodes.count(); ++i)
				if (_nodes[i] == temp)
					_nodes[i] = _nodes[x];

		}
	}

	usize 
	quick_find::count() const
	{
		return _nodes.count();
	}
}