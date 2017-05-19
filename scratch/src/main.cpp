#include <iostream>
#include "benchmark.h"
#include <cpprelude/dynamic_array.h>
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
	scratch();
	return 0;
}
