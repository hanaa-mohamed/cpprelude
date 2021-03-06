#include "catch.hpp"
#include <cpprelude/algorithm.h>
#include <functional>

using namespace cpprelude;

TEST_CASE("merge_sort test", "[merge_sort]")
{

	SECTION("Case 01")
	{
		dynamic_array<usize> arr(10);
		
		for (size_t i = 0; i < 5; i++)
		{
			arr[i] = i + 2;
		}

		for (size_t i = 5; i < 10; i++)
		{
			arr[i] = (i - 5) * 3;
		}

		dynamic_array<usize> aux(arr);

		CHECK(!cpprelude::is_sorted(aux.begin(), aux.count()));
		cpprelude::_merge(arr.begin(), aux.begin(), 0, 4, 9);
		CHECK(cpprelude::is_sorted(aux.begin(), aux.count()));

		CHECK(!cpprelude::is_sorted(arr.begin(), arr.count()));
		merge_sort(arr.begin(),arr.count());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
				
	}
	
	SECTION("Case 02") 
	{
		usize length = 128;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}
		
		CHECK(!cpprelude::is_sorted(arr.begin(), arr.count()));
		merge_sort(arr.begin(), arr.count());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	}

	SECTION("Case 03")
	{
		usize length = 512;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}

		CHECK(!cpprelude::is_sorted(arr.begin(), arr.count()));
		merge_sort(arr.begin(), arr.count());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	}

	SECTION("Case 04")
	{
		usize length = 128;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}

		//just playing with lamdas
		std::function <bool (const usize& , const usize& )> fun = [](const usize& x, const usize& y){
			return x > y;
		};

		CHECK(!cpprelude::is_sorted(arr.begin(), arr.count(), fun));
		merge_sort(arr.begin(), arr.count(), fun);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count(), fun));
	}
	
}