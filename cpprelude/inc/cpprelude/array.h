#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	template<typename T, usize fixed_count>
	struct array
	{
		T _data[fixed_count];

		T&
		operator[](usize index)
		{
			return _data[index];
		}

		T
		operator[](usize index) const
		{
			return _data[index];
		}

		usize
		count() const
		{
			return fixed_count;
		}

		const T*
		data() const
		{
			return _data;
		}

		T*
		data()
		{
			return _data;
		}
	};
}