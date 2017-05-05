#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"


namespace cpprelude
{
	
	struct API quick_find
	{
		dynamic_array<usize> _arr;

		quick_find();

		quick_find(usize count);

		bool
		is_connected(usize x, usize y);

		void
		connect(usize x, usize y);

		usize
		count() const;
		
	};
}