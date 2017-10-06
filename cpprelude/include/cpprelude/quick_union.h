#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"

namespace cpprelude
{
	struct API quick_union
	{
		dynamic_array<usize> _nodes;
		dynamic_array<usize> _sizes;

		quick_union();

		explicit quick_union(usize count);

		void
		init(usize count);

		void
		connect(usize a, usize b);

		bool
		is_connected(usize a, usize b);

		usize
		count() const;

		usize
		_root(usize a);
	};
}