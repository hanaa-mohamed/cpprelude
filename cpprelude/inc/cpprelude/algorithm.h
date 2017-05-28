#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

#include <random>
#include <cpprelude/dynamic_array.h>

namespace cpprelude
{
	namespace details
	{
		inline usize
		_get_random_index(usize from, usize to)
		{
			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<usize> distribution(from, to);

			return distribution(generator);
		}

		inline usize
		_get_random_index(usize n)
		{
			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<usize> distribution(0, n);

			return distribution(generator);
		}

		template<typename T>
		struct default_less_than
		{
			bool operator()(const T& a, const T& b)
			{
				return a < b;
			}
		};
	}


	template<typename iterator_type>
	void
	shuffle(iterator_type start, usize size)
	{
		iterator_type it = start;
		for (usize i = 1; i < size; ++i)
		{
			usize random_index = details::_get_random_index(i);
			iterator_type random_iterator = next(start, random_index);

			auto temp = tmp::move(*it);
			*it = tmp::move(*random_iterator);
			*random_iterator = tmp::move(temp);

			it = next(it);
		}
	}
	
	template<typename iterator_type, typename function_type = details::default_less_than<iterator_type::alias_type>>
	void
	merge(iterator_type arr, iterator_type aux, usize lo, usize mid, usize hi, function_type lessThan = function_type())
	{
		usize i = lo;
		usize j = mid + 1;
		usize k = lo;

		iterator_type arr_at_i = next(arr, i);
		iterator_type arr_at_j = next(arr, j);
		iterator_type aux_at_k = next(aux, k);

		while (i < mid + 1 && j <= hi)
		{


			if (lessThan((*arr_at_i), (*arr_at_j)))
			{
				//aux[k++] = arr[i++];
				*aux_at_k = *arr_at_i;

				aux_at_k = next(aux_at_k);
				arr_at_i = next(arr_at_i);
				++i;
				++k;
			}

			else
			{
				//	aux[k++] = arr[j++];
				*aux_at_k = *arr_at_j;
				aux_at_k = next(aux_at_k);
				arr_at_j = next(arr_at_j);
				++k;
				++j;
			}
		}

		if (j == hi + 1)
			while (i <= mid)
			{
				//	aux[k++] = arr[i++];
				*aux_at_k = *arr_at_i;
				aux_at_k = next(aux_at_k);
				arr_at_i = next(arr_at_i);
				++k;
				++i;
			}
	}

	template<typename iterator_type, typename Comparator = details::default_less_than<iterator_type::alias_type>>
	void
	merge_sort(iterator_type arr, usize size, Comparator lessThan = Comparator())
	{
		usize hi = 0;
		usize mid = 0;
		usize count = size;

		dynamic_array<iterator_type::alias_type> aux(size);
		iterator_type aux_it = aux.begin();
		iterator_type end = next(arr, size);
		iterator_type temp = arr;

		for (auto& n : aux)
		{
			n = *temp;
			temp = next(temp);
		}

		for (usize range = 1; range < count; range += range)
		{
			for (usize lo = 0; lo < count - range; lo += (range + range))
			{
				mid = lo + range - 1;
				hi = (mid + range  > count - 1) ? count - 1 : mid + range;

				temp = next(aux_it, mid);
				//arr[mid+1] < arr[mid]
				if (lessThan(*next(temp) , *temp))
					merge(arr, aux_it, lo, mid, hi, lessThan);
			}

			temp = arr;
			for (auto& n : aux)
			{
				*temp = n;
				temp = next(temp);
			}
		}
	}


	template<typename iterator_type, typename Comparator = details::default_less_than<iterator_type::alias_type>>
	bool
	is_sorted(iterator_type arr, usize lo, usize hi, Comparator lessThan = Comparator())
	{

		iterator_type start = next(arr, lo);
		iterator_type end = next(arr, hi - 1);
		iterator_type next_start = next(start);
		while (start != end)
		{

			if (lessThan(*next_start ,*start))
				return false;

			start = next(start);
			next_start = next(next_start);

		}
		return true;
	}

}
