#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

#include <random>

namespace cpprelude
{
	namespace details
	{
		inline usize
		_get_random_index(usize n)
		{
			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<usize> distribution(0, n);

			return distribution(generator);
		}
	}

	
	template<typename iterator_type>
	API void
	shuffle(iterator_type start, usize size)
	{
		iterator_type it = start;
		for(usize i = 1; i < size; ++i)
		{
			usize random_index = details::_get_random_index(i);
			iterator_type random_iterator = next(start, random_index);
			
			auto temp = tmp::move(*it);
			*it = tmp::move(*random_iterator);
			*random_iterator = tmp::move(temp);
			
			it = next(it);
		}
	}
}
