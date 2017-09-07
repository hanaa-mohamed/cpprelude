#include <iostream>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/algorithm.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	//inserting random numbers in a dynamic_array
	usize length = 15;
	dynamic_array<usize> arr(length);

	for (usize i = 0; i < length; i++)
	{
		arr[i] = details::_get_random_index(length);
	}

	//Using another comparator function
	auto fun = [](const usize& x, const usize& y) {
		return x > y;
	};

	//If third paramater is not passed then it will select the smallest kth element.
	auto kth_element_it = quick_select(arr.begin(), arr.count(), 3, fun);

	std::cout << *kth_element_it;
	
	return 0;
}
