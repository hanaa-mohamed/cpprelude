#include "catch.hpp"
#include <cpprelude/quick_find.h>

using namespace cpprelude;

TEST_CASE("quick_find test", "[quick_find]")
{
	
	SECTION("Case 01")
	{
		
		quick_find array(10);
		CHECK(array.is_connected(4, 3) == false);
		CHECK(array.is_connected(3, 8) == false);
		CHECK(array.is_connected(6, 5) == false);
		CHECK(array.is_connected(9, 4) == false);
		CHECK(array.is_connected(2, 1) == false);
		CHECK(array.is_connected(8, 9) == false);
		CHECK(array.is_connected(5, 0) == false);
		CHECK(array.is_connected(7, 2) == false);
		CHECK(array.is_connected(6, 1) == false);
		CHECK(array.is_connected(1, 0) == false);
		CHECK(array.is_connected(6, 7) == false);

		array.connect(4, 3);		CHECK(array.is_connected(4, 3) == true);
		array.connect(3, 8);		CHECK(array.is_connected(3, 8) == true);
		array.connect(6, 5);		CHECK(array.is_connected(6, 5) == true);
		array.connect(9, 4);		CHECK(array.is_connected(9, 4) == true);
		array.connect(2, 1);		CHECK(array.is_connected(2, 1) == true);
		array.connect(5, 0);		CHECK(array.is_connected(5, 0) == true);
		array.connect(7, 2);		CHECK(array.is_connected(7, 2) == true);


		CHECK(array.is_connected(4, 3) == true);
		CHECK(array.is_connected(3, 8) == true);
		CHECK(array.is_connected(6, 5) == true);
		CHECK(array.is_connected(9, 4) == true);
		CHECK(array.is_connected(2, 1) == true);
		CHECK(array.is_connected(8, 9) == true);
		CHECK(array.is_connected(5, 0) == true);
		CHECK(array.is_connected(7, 2) == true);
		CHECK(array.is_connected(6, 1) == false);
		CHECK(array.is_connected(1, 0) == false);
		CHECK(array.is_connected(6, 7) == false);

		CHECK(array.is_connected(0, 1) == false);
		CHECK(array.is_connected(0, 2) == false);
		CHECK(array.is_connected(0, 7) == false);
		CHECK(array.is_connected(5, 1) == false);
		CHECK(array.is_connected(5, 2) == false);
		CHECK(array.is_connected(5, 7) == false);
		CHECK(array.is_connected(6, 1) == false);
		CHECK(array.is_connected(6, 2) == false);
		CHECK(array.is_connected(6, 7) == false);
		CHECK(array.is_connected(0, 3) == false);
		CHECK(array.is_connected(0, 4) == false);
		CHECK(array.is_connected(0, 8) == false);
		CHECK(array.is_connected(0, 9) == false);

	}
	
}