#include "catch.hpp"
#include <cpprelude/quick_find.h>

using namespace cpprelude;

TEST_CASE("quick_find test", "[quick_find]")
{
	
	
	SECTION("Case 01")
	{
		
		quick_find array(10);
		CHECK(array.connected(4, 3) == false);
		CHECK(array.connected(3, 8) == false);
		CHECK(array.connected(6, 5) == false);
		CHECK(array.connected(9, 4) == false);
		CHECK(array.connected(2, 1) == false);
		CHECK(array.connected(8, 9) == false);
		CHECK(array.connected(5, 0) == false);
		CHECK(array.connected(7, 2) == false);
		CHECK(array.connected(6, 1) == false);
		CHECK(array.connected(1, 0) == false);
		CHECK(array.connected(6, 7) == false);

		array.union1(4, 3);		CHECK(array.connected(4, 3) == true);
		array.union1(3, 8);		CHECK(array.connected(3, 8) == true);
		array.union1(6, 5);		CHECK(array.connected(6, 5) == true);
		array.union1(9, 4);		CHECK(array.connected(9, 4) == true);
		array.union1(2, 1);		CHECK(array.connected(2, 1) == true);
		array.union1(5, 0);		CHECK(array.connected(5, 0) == true);
		array.union1(7, 2);		CHECK(array.connected(7, 2) == true);
		
		
		

		CHECK(array.connected(4, 3) == true);
		CHECK(array.connected(3, 8) == true);
		CHECK(array.connected(6, 5) == true);
		CHECK(array.connected(9, 4) == true);
		CHECK(array.connected(2, 1) == true);
		CHECK(array.connected(8, 9) == true);
		CHECK(array.connected(5, 0) == true);
		CHECK(array.connected(7, 2) == true);
		CHECK(array.connected(6, 1) == false);
		CHECK(array.connected(1, 0) == false);
		CHECK(array.connected(6, 7) == false);
		
		CHECK(array.connected(0, 1) == false);
		CHECK(array.connected(0, 2) == false);
		CHECK(array.connected(0, 7) == false);
		CHECK(array.connected(5, 1) == false);
		CHECK(array.connected(5, 2) == false);
		CHECK(array.connected(5, 7) == false);
		CHECK(array.connected(6, 1) == false);
		CHECK(array.connected(6, 2) == false);
		CHECK(array.connected(6, 7) == false);
		CHECK(array.connected(0, 3) == false);
		CHECK(array.connected(0, 4) == false);
		CHECK(array.connected(0, 8) == false);
		CHECK(array.connected(0, 9) == false);

	}

	
}