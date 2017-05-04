#include "catch.hpp"
#include <cpprelude\stack_array.h>

using namespace cpprelude;

TEST_CASE("stack_array test", "[stack_array]")
{


	SECTION("Case 01")
	{

		stack_array arr;
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);

		arr.push(3);
		CHECK(arr.isEmpty() == false);
		CHECK(arr.size() == 1);
		CHECK(arr.pop() == 3);
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
	}

	SECTION("Case 02")
	{

		stack_array arr(5);
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
		CHECK(arr.capacity() == 5);

		arr.push(3);
		CHECK(arr.isEmpty() == false);
		CHECK(arr.size() == 1);
		CHECK(arr.capacity() == 5);

		CHECK(arr.pop() == 3);

		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
		CHECK(arr.capacity() == 5);
	}

	SECTION("Case 03")
	{

		stack_array arr(5);
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
		CHECK(arr.capacity() == 5);

		arr.push(3);
		arr.push(3);
		arr.push(3);
		arr.push(3);
		arr.push(3);
		CHECK(arr.isEmpty() == false);
		CHECK(arr.size() == 5);
		CHECK(arr.capacity() == 5);

		CHECK(arr.pop() == 3);

		CHECK(arr.isEmpty() == false);
		CHECK(arr.size() == 4);
		CHECK(arr.capacity() == 5);
	}

	SECTION("Case 03")
	{

		stack_array arr(5);
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
		CHECK(arr.capacity() == 5);

		for(usize i =0; i<100;i++)
			arr.push(i);
		
		CHECK(arr.isEmpty() == false);
		CHECK(arr.size() == 100);
		//CHECK(arr.capacity() == 100);
		for (usize i = 99; i >0; i--)
		{
			CHECK(arr.pop() == i);
			CHECK(arr.isEmpty() == false);
			CHECK(arr.size() == i);
		}
		
		CHECK(arr.pop() == 0);
		CHECK(arr.isEmpty() == true);
		CHECK(arr.size() == 0);
		
		//CHECK(arr.capacity() == 100);
	}
}