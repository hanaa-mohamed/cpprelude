#include "catch.hpp"
#include <cpprelude/algorithm.h>
#include <functional>
using namespace cpprelude;



TEST_CASE("quick_sort test", "[quick_sort]")
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

		CHECK(!is_sorted(arr, 0, arr.count() - 1));
		quick_sort(arr);
		CHECK(is_sorted(arr, 0, arr.count() - 1));

	}
	
	SECTION("Case 02")
	{
		usize length = 10;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}

		CHECK(!is_sorted(arr, 0, arr.count() - 1));
		quick_sort(arr);
		CHECK(is_sorted(arr, 0, arr.count() - 1));

	}

	SECTION("Case 03")
	{
		usize length = 40;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}

		CHECK(!is_sorted(arr, 0, arr.count() - 1));
		quick_sort(arr);
		CHECK(is_sorted(arr, 0, arr.count() - 1));
	}

	SECTION("Case 04")
	{
		usize length = 40;
		dynamic_array<usize> arr(length);

		for (usize i = 0; i < length; i++)
		{
			arr[i] = details::_get_random_index(length);
		}

		CHECK(!is_sorted(arr, 0, arr.count() - 1));

		std::function <isize(const usize&, const usize&)> fun = [](const usize& x, const usize& y) {
			if (x < y) return -1;
			else if (x == y) return 0;
			return 1;
		};
		merge_sort(arr, fun);
		CHECK(is_sorted(arr, 0, arr.count() - 1));
	}

}