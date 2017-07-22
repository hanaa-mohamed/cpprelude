#include "benchmark.h"
#include "stopwatch.h"

#include <cpprelude/dynamic_array.h>
#include <vector>

#include <cpprelude/stack_array.h>
#include <cpprelude/stack_list.h>
#include <stack>

#include <cpprelude/slinked_list.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/bucket_array.h>
#include <list>
#include <deque>
#include <forward_list>

#include <cpprelude/queue_list.h>
#include <cpprelude/queue_array.h>
#include <queue>
#include <cpprelude/priority_queue.h>

#include <cpprelude/algorithm.h>
#include <algorithm>

#include <cpprelude/threading.h>
#include <functional>

#include <cpprelude/string.h>
#include <string>
#include <sstream>

#include <cpprelude/hash_array.h>
#include <unordered_map>
#include <cpprelude/tree_map.h>
#include <map>

#include <iostream>

using namespace cpprelude;

void
benchmark_dynamic_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dynamic_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_dynamic_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dynamic_array custom allocator" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_slinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::slinked_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();

	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark slinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_slinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::slinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark custom slinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_dlinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dlinked_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark dlinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_dlinked_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dlinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom dlinked_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_stack_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark stack_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_stack_array(cpprelude::usize limit)
{
	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	{
		cpprelude::stack_array<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();
	}

	std::cout << "benchmark custom stack_array" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_stack_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark stack_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_stack_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::stack_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom stack_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_queue_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_list<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_queue_list(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark custom queue_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_queue_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_priority_queue(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::priority_queue<cpprelude::usize>  array;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark priority_queue" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_custom_queue_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	cpprelude::slice<cpprelude::ubyte> mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::queue_array<cpprelude::usize, 128, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark custom queue_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;

	cpprelude::virtual_free(mem_block);
}

void
benchmark_bucket_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::bucket_array<cpprelude::usize> array;

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark bucket_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_merge_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::merge_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark merge_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_quick_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::quick_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark quick_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_insertion_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::insertion_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark insertion_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_heap_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::dynamic_array<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(distribution(generator));

		w.start();
		cpprelude::heap_sort(array.begin(), array.count());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark heap_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

template<typename semaphore_t>
void
semaphore_thread_take_func(semaphore_t* sema, cpprelude::usize limit)
{
	for(cpprelude::usize i = 0; i < limit; ++i)
		sema->wait_take();
}

template<typename semaphore_t>
void
semaphore_thread_give_func(semaphore_t* sema, cpprelude::usize limit)
{
	for(cpprelude::usize i = 0; i < limit; ++i)
		sema->wait_give();
}

void
benchmark_binary_semaphore(cpprelude::usize limit)
{
	cpprelude::binary_semaphore sema;
	auto give_func = semaphore_thread_give_func<cpprelude::binary_semaphore>;
	auto take_func = semaphore_thread_take_func<cpprelude::binary_semaphore>;

	std::thread g1(give_func, &sema, limit);
	std::thread g2(give_func, &sema, limit*3);
	std::thread g3(give_func, &sema, limit*2);

	std::thread t1(take_func, &sema, limit);
	std::thread t2(take_func, &sema, limit);
	std::thread t3(take_func, &sema, limit);
	std::thread t4(take_func, &sema, limit);
	std::thread t5(take_func, &sema, limit);
	std::thread t6(take_func, &sema, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "benchmark binary_semaphore" << std::endl;
	std::cout << "counter: " << sema._counter << std::endl;
}

void
benchmark_count_semaphore(cpprelude::usize limit)
{
	cpprelude::count_semaphore<1024> sema;
	auto give_func = semaphore_thread_give_func<cpprelude::count_semaphore<1024>>;
	auto take_func = semaphore_thread_take_func<cpprelude::count_semaphore<1024>>;

	std::thread g1(give_func, &sema, limit);
	std::thread g2(give_func, &sema, limit*3);
	std::thread g3(give_func, &sema, limit*2);

	std::thread t1(take_func, &sema, limit);
	std::thread t2(take_func, &sema, limit);
	std::thread t3(take_func, &sema, limit);
	std::thread t4(take_func, &sema, limit);
	std::thread t5(take_func, &sema, limit);
	std::thread t6(take_func, &sema, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "benchmark count_semaphore" << std::endl;
	std::cout << "counter: " << sema._counter << std::endl;
}

void
thread_unique_dequeue_func(
	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
	cpprelude::usize limit)
{
	auto context = job_queue->new_context();

	while(limit)
	{
		if(job_queue->read(context))
		{
			if(!job_queue->value.empty())
			{
				job_queue->value.dequeue();
				--limit;
				job_queue->read_release(context);
			}
			else
			{
				job_queue->read_release(context);
				continue;
			}
		}
	}

}

void
thread_unique_enqueue_func(
	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
	cpprelude::usize limit)
{
	auto context = job_queue->new_context();

	while(limit)
	{
		if(job_queue->write(context))
		{
			job_queue->value.enqueue(1);
			--limit;
			job_queue->write_release(context);
		}
	}
}

void
benchmark_thread_unique(cpprelude::usize limit)
{
	cpprelude::thread_unique<cpprelude::queue_array<int>> job_queue;
	auto enqueue_func = thread_unique_enqueue_func;
	auto dequeue_func = thread_unique_dequeue_func;

	std::thread g1(dequeue_func, &job_queue, limit);
	std::thread g2(dequeue_func, &job_queue, limit*3);
	std::thread g3(dequeue_func, &job_queue, limit*2);

	std::thread t1(enqueue_func, &job_queue, limit);
	std::thread t2(enqueue_func, &job_queue, limit);
	std::thread t3(enqueue_func, &job_queue, limit);
	std::thread t4(enqueue_func, &job_queue, limit);
	std::thread t5(enqueue_func, &job_queue, limit);
	std::thread t6(enqueue_func, &job_queue, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "benchmark thread_unique" << std::endl;
	std::cout << "empty: " << job_queue.value.empty() << std::endl;
	std::cout << "count: " << job_queue.value.count() << std::endl;
}

void
thread_multi_reader_read_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->read(context))
		{
			if(array->value.empty())
			{
				array->read_release(context);
				std::this_thread::sleep_for(std::chrono::duration<float, std::micro>(10));
				continue;
			}

			//std::cout << "thread_multi_reader_read_func.count = " << array->value.count() << std::endl;

			for(const auto& x: array->value)
				if(x != 1)
					std::cout << "ERROR!" << std::endl;
			array->read_release(context);
			return;
		}
	}
}

void
thread_multi_reader_insert_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->write(context))
		{
			//std::cout << "thread_multi_reader_insert_func" << std::endl;
			while(limit--)
				array->value.insert_back(1);
			array->write_release(context);
			return;
		}

	}
}

void
thread_multi_reader_remove_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->write(context))
		{
			//std::cout << "thread_multi_reader_remove_func" << std::endl;

			if(array->value.count() >= limit)
			{
				array->value.remove_back(limit);
				array->write_release(context);
				return;
			}
			else
			{
				array->write_release(context);
				std::this_thread::sleep_for(std::chrono::duration<float, std::micro>(10));

				continue;
			}
		}
	}
}

cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>
thread_multi_reader_job_queue;

void
benchmark_thread_multi_reader(cpprelude::usize limit)
{
	static bool read_done = false;

	auto insert_func = thread_multi_reader_insert_func;
	auto remove_func = thread_multi_reader_remove_func;
	auto read_func = thread_multi_reader_read_func;

	std::thread g1(remove_func, &thread_multi_reader_job_queue, limit);
	std::thread g2(remove_func, &thread_multi_reader_job_queue, limit*3);
	std::thread g3(remove_func, &thread_multi_reader_job_queue, limit*2);

	if(!read_done)
	{
		std::thread r1(read_func, &thread_multi_reader_job_queue, limit);
		std::thread r2(read_func, &thread_multi_reader_job_queue, limit);
		std::thread r3(read_func, &thread_multi_reader_job_queue, limit);
		r1.detach();
		r2.detach();
		r3.detach();
		read_done = true;
	}

	std::thread t1(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t2(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t3(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t4(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t5(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t6(insert_func, &thread_multi_reader_job_queue, limit);

	g1.join();
	g2.join();
	g3.join();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "benchmark thread_multi_reader" << std::endl;
	std::cout << "empty: " << thread_multi_reader_job_queue.value.empty() << std::endl;
	std::cout << "count: " << thread_multi_reader_job_queue.value.count() << std::endl;
}

void
benchmark_string_writer(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::string_writer<cpprelude::global_allocator> writer;
		auto str = "koko wawa"_l;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
			writer.write_back(str);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark string_writer" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}
void
benchmark_hash_array(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::hash_array<usize, usize> array;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
		{
			array.insert(i, i+9);
			auto it = array.lookup(i);
			array.remove(it);
		}
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark hash_array" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_tree_map(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		cpprelude::tree_map<usize, bool> tree;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
		{
			tree.insert(i, true);
			auto it = tree.lookup(i);
			tree.remove(it);
		}
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark tree_map" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}


//STD

void
benchmark_vector(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark vector" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_stack(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;

	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::stack<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark stack" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_list(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::list<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_deque(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::deque<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_back(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();

	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark deque" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_forward_list(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::forward_list<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push_front(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark forward_list" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_queue(std::size_t limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::queue<std::size_t> array;

		w.start();
		for(std::size_t i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark queue" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push_back(distribution(generator));

		w.start();
		std::sort(array.begin(), array.end());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_stable_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for(cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<cpprelude::usize> array;

		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push_back(distribution(generator));

		w.start();
		std::stable_sort(array.begin(), array.end());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark std::stable_sort" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_priority_queue(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::priority_queue<cpprelude::usize>  array;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::priority_queue" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_heap_sort(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::vector<cpprelude::usize> array;

		for (cpprelude::usize i = 0; i < limit; ++i)
			array.push_back(distribution(generator));

		w.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::sort_heap" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_string(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::string str;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
			str += "koko wawa";
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::string" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_stringstream(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::stringstream str;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
			str << "koko wawa";
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;


	std::cout << "benchmark std::stringstream" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_unordered_map(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::unordered_map<usize, usize> array;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
		{
			array.insert({i, i+9});
			auto it = array.find(i);
			array.erase(it);
		}
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark unordered_map" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark_std_map(cpprelude::usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	stopwatch w;
	for (cpprelude::usize j = 0; j < 100; ++j)
	{
		std::map<usize, bool> tree;

		w.start();
		for (cpprelude::usize i = 0; i < limit; ++i)
		{
			tree.insert({i, true});
			auto it = tree.find(i);
			tree.erase(it);
		}
		w.stop();

		avg_sec += w.seconds();
		avg_milli += w.milliseconds();
		avg_micro += w.microseconds();
		avg_nano += w.nanoseconds();
	}

	avg_sec /= 100;
	avg_milli /= 100;
	avg_micro /= 100;
	avg_nano /= 100;

	std::cout << "benchmark map" << std::endl;
	std::cout << "seconds: " << avg_sec << std::endl;
	std::cout << "milliseconds: " << avg_milli << std::endl;
	std::cout << "microseconds: " << avg_micro << std::endl;
	std::cout << "nanoseconds: " << avg_nano << std::endl;
}

void
benchmark()
{
	cpprelude::usize limit = 10000;

	std::cout << "\nBENCHMARK START\n" << std::endl;

	benchmark_vector(limit);
	std::cout << std::endl;
	benchmark_dynamic_array(limit);
	std::cout << std::endl;
	benchmark_custom_dynamic_array(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_stack(limit);
	std::cout << std::endl;
	benchmark_stack_array(limit);
	std::cout << std::endl;
	benchmark_stack_list(limit);
	std::cout << std::endl;
	benchmark_custom_stack_array(limit);
	std::cout << std::endl;
	benchmark_custom_stack_list(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_forward_list(limit);
	std::cout << std::endl;
	benchmark_slinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_slinked_list(limit);


	std::cout <<"============================================================"<< std::endl;

	benchmark_list(limit);
	std::cout << std::endl;
	benchmark_deque(limit);
	std::cout << std::endl;
	benchmark_dlinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_dlinked_list(limit);
	std::cout << std::endl;
	benchmark_bucket_array(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_queue(limit);
	std::cout << std::endl;
	benchmark_queue_list(limit);
	std::cout << std::endl;
	benchmark_custom_queue_list(limit);
	std::cout << std::endl;
	benchmark_queue_array(limit);
	std::cout << std::endl;
	benchmark_custom_queue_array(limit);
	std::cout << std::endl;
	benchmark_priority_queue(limit);
	std::cout << std::endl;
	benchmark_std_priority_queue(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_string_writer(limit);
	std::cout << std::endl;
	benchmark_std_string(limit);
	std::cout << std::endl;
	benchmark_std_stringstream(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_hash_array(limit);
	std::cout << std::endl;
	benchmark_std_unordered_map(limit);
	std::cout << std::endl;
	benchmark_tree_map(limit);
	std::cout << std::endl;
	benchmark_std_map(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_std_sort(limit);
	std::cout << std::endl;
	benchmark_std_stable_sort(limit);
	std::cout << std::endl;
	benchmark_merge_sort(limit);
	std::cout << std::endl;
	benchmark_quick_sort(limit);
	std::cout << std::endl;
	benchmark_heap_sort(limit);
	std::cout << std::endl;
	benchmark_std_heap_sort(limit);

	std::cout << "\nnext size = 1/10 of original benchmark size\n" << std::endl;

	benchmark_insertion_sort(limit/10);
	std::cout << std::endl;
	benchmark_binary_semaphore(limit/10);
	std::cout << std::endl;
	benchmark_count_semaphore(limit/10);
	std::cout << std::endl;
	benchmark_thread_unique(limit/10);
	std::cout << std::endl;
	benchmark_thread_multi_reader(limit/10);


	std::cout << "\nBENCHMARK END\n" << std::endl;
}
