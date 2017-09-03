#include <iostream>
#include <cpprelude/slinked_list.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	slinked_list<usize> numbers;

	//so we expect this half to be reversed since we append at the front
	for(usize i = 0; i < 10; ++i)
		numbers.insert_front(i+1);

	//when inserting a list of values slinked_list keeps the order of the elements so this half of the list will be ordered
	numbers.insert_front({11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

	for(const auto& number: numbers)
		std::cout << number << std::endl;
	return 0;
}
