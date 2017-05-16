#include "catch.hpp"
#include <cpprelude/queue_list.h>

using namespace cpprelude;

TEST_CASE("queue_list test", "[queue_list]")
{

	SECTION("Case 01")
	{
		queue_list<usize> queue;

		CHECK(queue.count() == 0);

		for (usize i = 0; i < 100; i++)
		{
			queue.enqueue(i);
			CHECK(queue.front() == 0);
		}

		CHECK(queue.count() == 100);

		for (usize i = 0; i < 100; i++)
		{
			CHECK(queue.front() == i);
			CHECK(queue.dequeue());
		}

		CHECK(queue.count() == 0);

	}

}