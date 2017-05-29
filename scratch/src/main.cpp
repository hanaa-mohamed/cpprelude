#include <iostream>
#include "benchmark.h"
#include <cpprelude/dynamic_array.h>
#include <cpprelude/slinked_list.h>
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
scratch()
{
	vec();
	std::cout <<"============================================================"<< std::endl;
	darr();

	std::cout <<"============================================================"<< std::endl;

	screamer* arr = (screamer*) malloc(sizeof(screamer) * 2);
	new (arr+0) screamer();
	new (arr+1) screamer();

	std::cout << arr << std::endl;

	arr = (screamer*) realloc(arr, sizeof(screamer)*4);

	std::cout << arr << std::endl;

	new (arr+2) screamer();
	new (arr+3) screamer();

	free(arr);

	//this is just a comment
	auto virtual_block = cpprelude::virtual_alloc(GIGABYTES(1));
	auto int_arr = virtual_block.template as<int>();
	int_arr[0] = 1;
	std::cout << "2" << std::endl;
	cpprelude::virtual_free(virtual_block);
}

int
main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	benchmark();
	shuffle_test();
	scratch();

	return 0;
}
