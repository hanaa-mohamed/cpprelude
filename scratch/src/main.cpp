#include <iostream>
#include "benchmark.h"
#include <cpprelude/dynamic_array.h>
#include <cpprelude/slinked_list.h>
#include <cpprelude/bucket_array.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/algorithm.h>
#include <vector>
#include <cstdlib>

struct screamer
{
	screamer()
	{
		std::cout << "ctor" << std::endl;
	}

	~screamer()
	{
		std::cout << "dtor" << std::endl;
	}

	screamer(const screamer& other)
	{
		std::cout << "copy ctor" << std::endl;
	}

	screamer(screamer&& other)
	{
		std::cout << "move ctor" << std::endl;
	}

	screamer&
	operator=(const screamer& other)
	{
		std::cout << "= copy" << std::endl;
		return *this;
	}

	screamer&
	operator=(screamer&& other)
	{
		std::cout << "= move" << std::endl;
		return *this;
	}
};

void
vec()
{
	std::vector<screamer> vec;
	for(int i = 0; i < 2; ++i)
		vec.push_back(screamer());
}

void
darr()
{
	cpprelude::dynamic_array<screamer> darr;
	for(int i = 0; i < 2; ++i)
		darr.insert_back(screamer());
}

//shuffle_test
void
shuffle_dynamic_array()
{
	cpprelude::dynamic_array<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";

	std::cout << "\nsorted:\n";

	cpprelude::insertion_sort(array.begin(), array.count());
	for (auto& n : array)
		std::cout << n << " ";
}

void
shuffle_slinked_list()
{
	cpprelude::slinked_list<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_front(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";
}

void
shuffle_dlinked_list()
{
	cpprelude::dlinked_list<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";

	std::cout << "\nsorted:\n";

	cpprelude::quick_sort(array.begin(), array.count());
	for (auto& n : array)
		std::cout << n << " ";

}

void
shuffle_test()
{
	shuffle_dynamic_array();
	std::cout << std::endl;
	shuffle_slinked_list();
	std::cout << std::endl;
	shuffle_dlinked_list();
	std::cout << std::endl;
}

void
quick_select_test()
{
	cpprelude::dlinked_list<int> array;
	for (int i = 0; i < 10; ++i)
		array.insert_back(i);
	
	for (auto& n : array)
		std::cout << n << " ";
	std::cout << "\nquick_select(4): " << *quick_select(array.begin(), array.count(), 4) << std::endl;
}

void
scratch()
{
	vec();
	std::cout <<"============================================================"<< std::endl;
	darr();

	std::cout <<"============================================================"<< std::endl;

	cpprelude::bucket_array<cpprelude::usize, 16> array;
	for(cpprelude::usize i = 0; i < 16; i++)
		array[i] = i;
	array.expand();
	for(cpprelude::usize i = 16; i < 32; i++)
		array[i] = i;

	cpprelude::dynamic_array<int> arr;
	arr.insert_back({2,2,2,2,1,1,1,1,0,0,0,0});

	auto result = cpprelude::_partition_3way(arr.begin(), arr.count());
	std::cout << "result.less_index = " << result.less_index << std::endl;
	std::cout << "result.greater_index = " << result.greater_index << std::endl;
	for (auto& n : arr)
		std::cout << n << " ";
	std::cout << "\n";
}

int
main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	benchmark();
	shuffle_test();
	quick_select_test();
	scratch();

	return 0;
}
