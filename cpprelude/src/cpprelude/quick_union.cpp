#include "cpprelude/quick_union.h"

namespace cpprelude
{
	quick_union::quick_union()
	{}

	quick_union::quick_union(usize count)
	{
		init(count);
	}

	void
	quick_union::init(usize count)
	{
		_nodes.resize(count);
		for(usize i = 0; i < count; ++i)
			_nodes[i] = i;

		_sizes.resize(count);
		for(usize i = 0; i < count; ++i)
			_sizes[i] = 1;
	}

	void
	quick_union::connect(usize a, usize b)
	{
		auto a_root = _root(a);
		auto b_root = _root(b);

		if(a_root == b_root)
			return;

		if(_sizes[a_root] < _sizes[b_root])
		{
			//put a subtree under b subtree. put small tree under big tree
			_nodes[a_root] = b_root;
			_sizes[b_root] += _sizes[a_root];
		}
		else
		{
			_nodes[b_root] = a_root;
			_sizes[a_root] += _sizes[b_root];
		}
	}

	bool
	quick_union::is_connected(usize a, usize b)
	{
		return _root(a) == _root(b);
	}

	usize
	quick_union::count() const
	{
		return _nodes.count();
	}

	usize
	quick_union::_root(usize a)
	{
		while(a != _nodes[a])
		{
			//path compression make it point to the grand parent given enough calls this will flatten the tree
			_nodes[a] = _nodes[_nodes[a]];
			a = _nodes[a];
		}
		return a;
	}
}