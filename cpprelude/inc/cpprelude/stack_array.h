#pragma once
#include "cpprelude\defines.h"
#include "cpprelude\dynamic_array.h"


namespace cpprelude
{

	struct API stack_array
	{
		dynamic_array <usize> _array;
		usize _count;

		stack_array();

		stack_array(usize count);

		void
			push(usize item);

		usize
			pop();

		bool
			isEmpty();

		usize
			size();

		usize
			capacity();

	};

}
