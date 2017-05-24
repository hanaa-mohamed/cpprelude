#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"
#include<functional>

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
	}

	
	template<typename iterator_type>
	void
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

	template<typename T>
	void
	merge(const dynamic_array<T>& arr, dynamic_array<T>& aux, usize lo, usize mid, usize hi)
	{
		usize i = lo;
		usize j = mid + 1;
		usize k = lo;
		
		while (i < mid + 1 && j <= hi)
		{
			if (arr[i] <= arr[j])
				aux[k++] = arr[i++];
			else
				aux[k++] = arr[j++];
		}

		if (j == hi +1 )
			while (i <= mid)
				aux[k++] = arr[i++];
	}

	template<typename T>
	void
	merge_sort(dynamic_array<T>& arr)
	{
		usize hi = 0;
		usize mid = 0;
		usize count = arr.count();
		dynamic_array<T> aux(arr);
		
		for (usize range = 1; range < count; range += range)
		{
			for (usize lo = 0; lo < count - range; lo += (range + range))
			{
				mid = lo + range - 1;
				hi = (mid + range  > count - 1) ? count - 1 : mid + range;

				if(aux[mid] > aux[mid+1])
					merge(arr, aux, lo, mid, hi);
			}
			arr = aux;
		}
	}

	template<typename T>
	void
	_merge(const dynamic_array<T>& arr, dynamic_array<T>& aux, usize lo, usize mid, usize hi,
		  std::function <isize(const T&, const T&)> compareTo)
	{
		usize i = lo;
		usize j = mid + 1;
		usize k = lo;

		while (i < mid + 1 && j <= hi)
		{
			if (compareTo(arr[i] ,arr[j])<=0)
				aux[k++] = arr[i++];
			else
				aux[k++] = arr[j++];
		}

		if (j == hi + 1)
			while (i <= mid)
				aux[k++] = arr[i++];
	}

	template<typename T>
	void
	merge_sort(dynamic_array<T>& arr , std::function <isize(const T&, const T&)> compareTo)
	{
		usize hi = 0;
		usize count = arr.count();
		dynamic_array<T> aux(arr);

		for (usize range = 1; range < count; range += range)
		{
			for (usize lo = 0; lo < count - range; lo += (range + range))
			{
				hi = (lo + range + range - 1 > count - 1) ? count - 1 : lo + range + range - 1;
				_merge(arr, aux, lo, lo + range - 1, hi , compareTo);
			}
			arr = aux;
		}
	}

	template<typename T>
	bool
	is_sorted(const dynamic_array<T> &arr, usize lo , usize hi )
	{
		for (usize i = lo; i < hi; i++)
		{
			if (arr[i] > arr[i + 1])
				return false;
		}
		return true;
	}

	template<typename T>
	void
	_partition(dynamic_array<T>& arr, usize start, usize end)
	{
		//Stoping condiction
		if (end <= start) return;

		usize lt = start;
		usize i = start;
		usize gt = end;

		//Chosing pivot randomly		
		usize index_pivot = start;// details::_get_random_index(start, end);
		T pivot = arr[index_pivot];
		T temp;
		//Partitioning 
		while (i <= gt)
		{
			if (arr[i] < pivot)
			{
				//swap(arr[lt++], arr[i++]);
				temp = arr[lt];
				arr[lt] = arr[i];
				arr[i] = temp;
				++lt;
				++i;
			}
			else if (arr[i] > pivot)
			{
				//swap(arr[i], arr[gt--]);
				temp = arr[gt];
				arr[gt] = arr[i];
				arr[i] = temp;
				--gt;
			}
			else 
				++i;
		}

		_partition(arr, start, lt - 1);
		_partition(arr, gt + 1, end);
	}

	template<typename T>
	void 
	quick_sort(dynamic_array<T>& arr)
	{
		_partition(arr, 0, arr.count() - 1);		
	}
	
}
