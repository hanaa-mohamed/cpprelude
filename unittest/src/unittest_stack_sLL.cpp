#include "catch.hpp"
#include <cpprelude/stack_sLL.h>

using namespace cpprelude;

TEST_CASE("stack_sLL test", "[stack_sLL]")
{

	SECTION("Case 01")
	{
		stack_sLL<usize> arr;
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);

		arr.push(3);
		CHECK(arr.empty() == false);
		CHECK(arr.count() == 1);
		CHECK(arr.top() == 3);
		CHECK(arr.pop());
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);
	}

	SECTION("Case 02")
	{
		stack_sLL<usize> arr;
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);

		arr.push(3);
		CHECK(arr.empty() == false);
		CHECK(arr.count() == 1);

		CHECK(arr.top() == 3);
		CHECK(arr.pop());

		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);
	}

	SECTION("Case 03")
	{

		stack_sLL<usize> arr;
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);

		arr.push(3);
		arr.push(3);
		arr.push(3);
		arr.push(3);
		arr.push(3);
		CHECK(arr.empty() == false);
		CHECK(arr.count() == 5);

		CHECK(arr.top() == 3);
		CHECK(arr.pop());

		CHECK(arr.empty() == false);
		CHECK(arr.count() == 4);
	}

	SECTION("Case 03")
	{

		stack_sLL<usize> arr;
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);

		for (usize i = 0; i<100; i++)
			arr.push(i);

		CHECK(arr.empty() == false);
		CHECK(arr.count() == 100);
		for (usize i = 99; i >0; i--)
		{
			CHECK(arr.top() == i);
			CHECK(arr.pop());
			CHECK(arr.empty() == false);
			CHECK(arr.count() == i);
		}

		CHECK(arr.top() == 0);
		CHECK(arr.pop());
		CHECK(arr.empty() == true);
		CHECK(arr.count() == 0);
	}

}