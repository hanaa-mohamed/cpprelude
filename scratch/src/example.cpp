#include <iostream>
#include <cpprelude/array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	array<i32, 10> int_array;
	for (array<i32, 10>::const_iterator it = int_array.begin();
		it != int_array.end();
		++it)
	{
		std::cout << *it << std::endl;
	}

	for (auto it = int_array.cbegin();
		it != int_array.cend();
		++it)
	{
		std::cout << *it << std::endl;
	}

	for(const auto& number: int_array)
		std::cout << number << std::endl;
	return 0;
}
