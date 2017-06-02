#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

#include <random>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/stack_array.h>

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

		template<typename iterator_type>
		struct partition_result
		{
			iterator_type iterator;
			usize index;

			partition_result(iterator_type it, usize ix)
				:iterator(it), index(ix)
			{}
		};

		template<typename iterator_type>
		struct partition_3way_result
		{
			iterator_type less_iterator, greater_iterator;
			usize less_index, greater_index;

			partition_3way_result(iterator_type lt, usize lt_ix, iterator_type gt, usize gt_ix)
				:less_iterator(lt), less_index(lt_ix),
				 greater_iterator(gt), greater_index(gt_ix)
			{}
		};

		template<typename iterator_type>
		inline typename iterator_type::data_type&
		_quick_next(iterator_type& it, usize& ix)
		{
			it = next(it);
			++ix;
			return *it;
		}

		template<typename iterator_type>
		inline typename iterator_type::data_type&
		_quick_prev(iterator_type& it, usize& ix)
		{
			it = prev(it);
			--ix;
			return *it;
		}
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

	template<typename iterator_type, typename Comparator = details::default_less_than<typename iterator_type::data_type>>
	bool
	is_sorted(iterator_type begin_it, usize count, Comparator less_than = Comparator())
	{
		iterator_type a = begin_it;
		iterator_type b = next(a);

		while (--count)
		{

			if (less_than(*b ,*a))
				return false;

			a = b;
			b = next(b);
		}
		return true;
	}

	//INSERTION SORT
	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	void
	insertion_sort(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		auto it = begin_it;
		for(usize i = 1; i < count; ++i)
		{
			it = next(it);

			usize p_ix = i-1;
			auto t_it = it;
			auto t_p_it = prev(t_it);
			
			while(less_than(*t_it, *t_p_it))
			{

				auto temp_value = tmp::move(*t_p_it);
				*t_p_it = tmp::move(*t_it);
				*t_it = tmp::move(temp_value);

				t_p_it = prev(t_p_it);
				t_it = prev(t_it);

				if(p_ix == 0)
					break;
				else
					--p_ix;
			}
		}
	}

	//MERGE SORT
	
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

	//QUICK SORT

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	details::partition_result<iterator_type>
	_median_of3(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		if(count < 3)
			return details::partition_result<iterator_type>(begin_it, 0);

		usize b_ix = count / 2.0f;
		usize c_ix = count-1;
		auto a = begin_it;
		auto b = next(a);
		auto c = next(b);

		if ((less_than(*b, *a) && less_than(*a, *c)) ||
			(less_than(*c, *a) && less_than(*a, *b)))
			return details::partition_result<iterator_type>(a, 0);

		if ((less_than(*a, *b) && less_than(*b, *c)) ||
			(less_than(*c, *b) && less_than(*b, *a)))
			return details::partition_result<iterator_type>(b, b_ix);

		if ((less_than(*b, *c) && less_than(*c, *a)) ||
			(less_than(*a, *c) && less_than(*c, *b)))
			return details::partition_result<iterator_type>(c, c_ix);

		return details::partition_result<iterator_type>(begin_it, 0);
	}	

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	details::partition_3way_result<iterator_type>
	_partition_3way(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		if (count == 0)
			return details::partition_3way_result<iterator_type>(next(begin_it, count), count, next(begin_it), count);

		usize lt_ix = 0;
		auto  lt_it = begin_it;
		usize gt_ix = count-1;
		auto  gt_it = next(begin_it, gt_ix);
		auto  k_val = *begin_it;
		auto  it 	= next(begin_it);
		usize it_ix = 1;

		while(it_ix <= gt_ix)
		{
			bool it_l_k = less_than(*it, k_val);
			bool k_l_it = less_than(k_val, *it);

			if(it_l_k)
			{
				auto temp_value = tmp::move(*lt_it);
				*lt_it = tmp::move(*it);
				*it = tmp::move(temp_value);

				lt_it = next(lt_it);
				++lt_ix;

				it = next(it);
				++it_ix;
			}
			else if(k_l_it)
			{
				auto temp_value = tmp::move(*gt_it);
				*gt_it = tmp::move(*it);
				*it = tmp::move(temp_value);

				gt_it = prev(gt_it);
				--gt_ix;
			}
			else
			{
				it = next(it);
				++it_ix;
			}
		}

		return details::partition_3way_result<iterator_type>(lt_it, lt_ix, gt_it, gt_ix);
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	details::partition_result<iterator_type>
	_partition(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		if (count == 0)
			return details::partition_result<iterator_type>(next(begin_it, count), count);

		// we assume that the pivot is the first element so lo_it starts at index = 1
		usize lo_ix = 1;
		usize hi_ix = count-1;
		auto lo_it = next(begin_it);
		auto hi_it = next(begin_it, hi_ix);
		auto k_it = begin_it;

		while (true)
		{

			while (less_than(*lo_it, *k_it))
			{
				lo_it = next(lo_it);
				++lo_ix;
				if (lo_ix == count)
					break;
			}

			while (less_than(*k_it, *hi_it))
			{
				hi_it = prev(hi_it);
				--hi_ix;
				if (hi_ix == 0)
					break;
			}
			
			//if the indices cross then break
			if (lo_ix >= hi_ix)
				break;

			//exchange lo_it and hi_it values
			auto temp_value = tmp::move(*lo_it);
			*lo_it = tmp::move(*hi_it);
			*hi_it = tmp::move(temp_value);
		}

		//exchange k with the hi_it
		auto temp_value = tmp::move(*hi_it);
		*hi_it = tmp::move(*k_it);
		*k_it = tmp::move(temp_value);

		return details::partition_result<iterator_type>(hi_it, hi_ix);
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	details::partition_result<iterator_type>
	_select(iterator_type begin_it, usize count, usize k, function_type less_than = function_type())
	{
		if (k >= count)
			return details::partition_result<iterator_type>(next(begin_it, count), count);

		shuffle(begin_it, count);

		auto it = begin_it;
		auto it_ix = 0;
		auto it_count = count;

		while (true)
		{
			auto presult = _partition(it, it_count, less_than);

			//convert from relative index of presult to absolute index of it
			if ((it_ix + presult.index) < k)
			{
				it = next(presult.iterator);
				//adjust iterator index
				it_ix += presult.index+1;
				//new count
				it_count = count - (it_ix + 1);
			}
			//convert from relative index of presult to absolute index of it
			else if ((it_ix + presult.index) > k)
			{
				it = it;
				it_ix = it_ix;
				it_count = presult.index;
			}
			else
			{
				return presult;
			}
		}

		return details::partition_result<iterator_type>(next(begin_it, k), k);
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	iterator_type
	quick_select(iterator_type begin_it, usize count, usize k, function_type less_than = function_type())
	{
		return _select(begin_it, count, k, less_than).iterator;
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	usize
	partition_index(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		return _partition(begin_it, count, less_than).index;
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	iterator_type
	partition_iterator(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		return _partition(begin_it, count, less_than).iterator;
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	void
	_quick_sort(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		if (count <= 1)
			return;

		auto presult = _partition(begin_it, count, less_than);
		//sort the first part of the array from begin to partition index
		_quick_sort(begin_it, presult.index, less_than);
		//sort the second part of the array from one past the partition iterator to end of array
		_quick_sort(next(presult.iterator), count - (presult.index + 1), less_than);
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>, usize cutoff_limit = 24>
	void
	_quick_sort_3way(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		if (count <= 1)
			return;

		//use insertion sort when cutoff
		if(count <= cutoff_limit)
		{
			insertion_sort(begin_it, count, less_than);
		}
		//continue quick_sort
		else
		{
			auto presult = _partition_3way(begin_it, count, less_than);
			//sort the first part of the array from begin to partition index
			_quick_sort_3way(begin_it, presult.less_index, less_than);
			//sort the second part of the array from one past the partition iterator to end of array
			_quick_sort_3way(next(presult.greater_iterator), count - (presult.greater_index + 1), less_than);
		}
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>, usize cutoff_limit = 24>
	void
	_quick_sort_3way_nr(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		stack_array<details::partition_result<iterator_type>> call_stack;
		call_stack.push(details::partition_result<iterator_type>(begin_it, count));
		
		while(!call_stack.empty())
		{
			auto call = call_stack.top();
			call_stack.pop();

			if (call.index <= 1)
				continue;

			//use insertion sort when cutoff
			if(call.index <= cutoff_limit)
			{
				insertion_sort(call.iterator, call.index);
			}
			else
			{
				auto presult = _partition_3way(call.iterator, call.index, less_than);
				//sort the second part of the array from one past the partition iterator to end of array
				call_stack.push(details::partition_result<iterator_type>(next(presult.greater_iterator), call.index - (presult.greater_index + 1)));

				//sort the first part of the array from begin to partition index
				call_stack.push(details::partition_result<iterator_type>(call.iterator, presult.less_index));
			}
		}
	}

	template<typename iterator_type, typename function_type = details::default_less_than<typename iterator_type::data_type>>
	void
	quick_sort(iterator_type begin_it, usize count, function_type less_than = function_type())
	{
		auto presult = _median_of3(begin_it, count, less_than);

		auto temp_value = tmp::move(*begin_it);
		*begin_it = tmp::move(*presult.iterator);
		*presult.iterator = tmp::move(temp_value);

		_quick_sort_3way(begin_it, count, less_than);
	}

}
