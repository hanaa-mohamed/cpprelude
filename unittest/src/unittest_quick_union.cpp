#include "catch.hpp"
#include <cpprelude/quick_union.h>

using namespace cpprelude;

TEST_CASE("quick_union test", "[quick_union]")
{
	quick_union graph;

	SECTION("Case 01")
	{
		graph.init(32);
		for(usize i = 0; i < graph.count(); ++i)
			for(usize j = 0; j < graph.count(); ++j)
				if(i==j)
					CHECK(graph.is_connected(i, j) == true);
				else
					CHECK(graph.is_connected(i, j) == false);

		graph.connect(0, 31);
		CHECK(graph.is_connected(0, 31) == true);

		graph.connect(31, 1);
		CHECK(graph.is_connected(1, 0) == true);

		graph.connect(15, 0);
		CHECK(graph.is_connected(15, 1) == true);

		graph.connect(20, 2);
		CHECK(graph.is_connected(20, 2) == true);

		graph.connect(5, 2);
		CHECK(graph.is_connected(20, 5) == true);

		graph.connect(5, 1);
		CHECK(graph.is_connected(31, 2) == true);
		CHECK(graph.is_connected(15, 5) == true);

		CHECK(graph.is_connected(15, 6) == false);
		CHECK(graph.is_connected(5, 6) == false);
	}

	SECTION("Case 02")
	{
		graph.init(32);
		for(usize i = 0; i < graph.count(); ++i)
			for(usize j = 0; j < graph.count(); ++j)
				if(i==j)
					CHECK(graph.is_connected(i, j) == true);
				else
					CHECK(graph.is_connected(i, j) == false);

		for(usize i = 0; i < graph.count(); i+=2)
			graph.connect(i, i+1);

		for(usize i = 0; i < graph.count(); i+=2)
			CHECK(graph.is_connected(i, i+1) == true);

		for(usize i = 0; i < graph.count(); i+=4)
			graph.connect(i, i+2);

		for(usize i = 0; i < graph.count(); i+=4)
		{
			CHECK(graph.is_connected(i, i+1) == true);
			CHECK(graph.is_connected(i, i+2) == true);
			CHECK(graph.is_connected(i, i+3) == true);
			CHECK(graph.is_connected(i+1, i+3) == true);
			CHECK(graph.is_connected(i+2, i+1) == true);
		}

		graph.connect(0, 4);
		CHECK(graph.is_connected(0, 4) == true);
		CHECK(graph.is_connected(1, 5) == true);
		CHECK(graph.is_connected(2, 6) == true);
		CHECK(graph.is_connected(3, 7) == true);
		CHECK(graph.is_connected(5, 8) == false);
			
	}	
}