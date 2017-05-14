#include "catch.hpp"
#include <cpprelude/queue_array.h>

using namespace cpprelude;

TEST_CASE("queue_array test", "[queue_array]")
{
	
	SECTION("Case 01")
	{
		queue_array<usize> queue;
		

		CHECK(queue.count() == 0);

		for (usize i = 0; i < 100; i++)
		{
			queue.enqueue(i);
			CHECK(queue.back() == i);
			CHECK(queue.front() == 0);
		}
		
		CHECK(queue.count() == 100);
		
		for (usize i = 0; i < 100; i++)
		{
			CHECK(queue.front() == i);
			CHECK(queue.back() == 99);
			CHECK(queue.dequeue());
		}

		CHECK(queue.count() == 0);

	}

	SECTION("Case 02")
	{
		queue_array<usize> queue(10);


		CHECK(queue.count() == 0);

		for (usize i = 0; i < 100; i++)
		{
			queue.enqueue(i);
			CHECK(queue.back() == i);
			CHECK(queue.front() == 0);
		}

		CHECK(queue.count() == 100);

		for (usize i = 0; i < 100; i++)
		{
			CHECK(queue.front() == i);
			CHECK(queue.back() == 99);
			CHECK(queue.dequeue());
		}

		CHECK(queue.count() == 0);

	}

	SECTION("Case 03")
	{
		queue_array<usize> queue(50);


		CHECK(queue.count() == 0);

		for (usize i = 0; i < 100; i++)
		{
			queue.enqueue(i);
			CHECK(queue.back() == i);
			CHECK(queue.front() == 0);
		}

		CHECK(queue.count() == 100);

		for (usize i = 0; i < 50; i++)
		{
			CHECK(queue.front() == i);
			CHECK(queue.back() == 99);
			CHECK(queue.dequeue());
		}

		CHECK(queue.count() == 50);

		for (usize i = 0; i < 10; i++)
		{
			queue.enqueue(i);
			CHECK(queue.back() == i);
			CHECK(queue.front() == 50);
		}

		CHECK(queue.count() == 60);

		usize temp = 0, j=0;
		for (usize i = 0; i < queue.count(); i++)
		{
			temp = queue.front();
			j = i < 50 ? i + 50 : i;
			CHECK( temp == j);
			CHECK(queue.back() == 9);
			CHECK(queue.dequeue());
		}
	}
}