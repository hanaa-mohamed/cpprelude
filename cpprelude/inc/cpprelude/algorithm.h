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
			return _get_random_index(0, n);
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
	
	template<typename iterator_type, typename aux_iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	void
	_merge(iterator_type begin_it, aux_iterator_type aux_it, usize lo, usize mid, usize hi, function_type less_than = function_type())
	{
		usize i = lo;
		usize j = mid + 1;
		usize k = lo;

		iterator_type arr_at_i = next(begin_it, i);
		iterator_type arr_at_j = next(begin_it, j);
		aux_iterator_type aux_at_k = next(aux_it, k);

		//loop through the two halfs and do merge
		while (i < mid + 1 && j <= hi)
		{
			if (less_than((*arr_at_i), (*arr_at_j)))
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

		//if the second half has been consumed first then we move the rest of the first half to the back
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

	template<typename iterator_type, typename Comparator = details::default_less_than<typename iterator_type::data_type>>
	void
	merge_sort(iterator_type begin_it, usize count, Comparator less_than = Comparator())
	{
		dynamic_array<typename iterator_type::data_type> aux(count);
		iterator_type aux_it = aux.begin();
		iterator_type end_it = next(begin_it, count);
		iterator_type tmp_it = begin_it;

		//so you copy the array to the aux 
		for (auto& n : aux)
		{
			n = *tmp_it;
			tmp_it = next(tmp_it);
		}

		//this is a bottom up merge sort so we start with rng_count = 1 then double each time
		for (usize rng_count = 1; rng_count < count; rng_count *= 2)
		{
			//for each range in the array
			for (usize lo = 0; lo < count - rng_count; lo += (rng_count*2))
			{
				//calculate the mid point
				usize mid = lo + rng_count - 1;
				//calculate the high point
				usize hi = (mid + rng_count  > count - 1) ? count - 1 : mid + rng_count;
				
				tmp_it = next(aux_it, mid);
				//arr[mid+1] < arr[mid]
				//if the first element in the second half is less than the last element in the first half
				if (less_than(*next(tmp_it) , *tmp_it))
					//if this is the case then merge the two halfs
					_merge(begin_it, aux_it, lo, mid, hi, less_than);
			}

			//commit the range back to the original array
			tmp_it = begin_it;
			for (auto& n : aux)
			{
				*tmp_it = n;
				tmp_it = next(tmp_it);
			}
		}
	}


	template<typename iterator_type, typename Comparator = details::default_less_than<typename iterator_type::data_type>>
	bool
	is_sorted(iterator_type begin_it, iterator_type end_it, Comparator less_than = Comparator())
	{
		iterator_type a = begin_it;
		iterator_type b = next(a);

		while (b != end_it)
		{

			if (less_than(*b ,*a))
				return false;

			a = b;
			b = next(b);
		}
		return true;
	}

}
