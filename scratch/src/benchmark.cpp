#include "benchmark.h"
#include "stopwatch.h"

#include <cpprelude/dynamic_array.h>
#include <vector>

#include <cpprelude/stack_array.h>
#include <cpprelude/stack_list.h>
#include <stack>

#include <cpprelude/slinked_list.h>
#include <cpprelude/dlinked_list.h>
#include <list>
#include <deque>
#include <forward_list>

#include <cpprelude/queue_list.h>
#include <queue>

#include <iostream>

void
benchmark_dynamic_array(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::dynamic_array<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(i);
	w.stop();

	std::cout << "benchmark dynamic_array" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_dynamic_array(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;
	{
		cpprelude::dynamic_array<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();
	}

	std::cout << "benchmark dynamic_array custom allocator" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;
}

void
benchmark_slinked_list(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::slinked_list<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_front(i);
	w.stop();

	std::cout << "benchmark slinked_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_slinked_list(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;

	{
		cpprelude::slinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_front(i);
		w.stop();
	}

	std::cout << "benchmark custom slinked_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;
}

void
benchmark_dlinked_list(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::dlinked_list<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(i);
	w.stop();

	std::cout << "benchmark dlinked_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_dlinked_list(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);
	stopwatch w;

	{
		cpprelude::dlinked_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.insert_back(i);
		w.stop();
	}

	std::cout << "benchmark custom dlinked_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;
}

void
benchmark_stack_array(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::stack_array<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push(i);
	w.stop();

	std::cout << "benchmark stack_array" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_stack_array(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
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
}

void
benchmark_stack_list(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::stack_list<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push(i);
	w.stop();

	std::cout << "benchmark stack_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_stack_list(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	{
		cpprelude::stack_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.push(i);
		w.stop();
	}

	std::cout << "benchmark custom stack_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;
}

void
benchmark_queue_list(cpprelude::usize limit)
{
	stopwatch w;
	cpprelude::queue_list<cpprelude::usize> array;

	w.start();
	for(cpprelude::usize i = 0; i < limit; ++i)
		array.enqueue(i);
	w.stop();

	std::cout << "benchmark queue_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_custom_queue_list(cpprelude::usize limit)
{
	cpprelude::owner_mem_block mem_block;
	auto arena_allocator = cpprelude::make_arena_allocator(MEGABYTES(25), mem_block);

	stopwatch w;

	{
		cpprelude::queue_list<cpprelude::usize, cpprelude::linear_allocator> array(arena_allocator);

		w.start();
		for(cpprelude::usize i = 0; i < limit; ++i)
			array.enqueue(i);
		w.stop();
	}

	std::cout << "benchmark custom queue_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
	std::cout << "alive allocation: " << arena_allocator._alloc_count << std::endl;
}

//STD

void
benchmark_vector(std::size_t limit)
{
	stopwatch w;
	std::vector<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push_back(i);
	w.stop();

	std::cout << "benchmark vector" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_stack(std::size_t limit)
{
	stopwatch w;
	std::stack<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push(i);
	w.stop();

	std::cout << "benchmark stack" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_list(std::size_t limit)
{
	stopwatch w;
	std::list<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push_front(i);
	w.stop();

	std::cout << "benchmark list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_deque(std::size_t limit)
{
	stopwatch w;
	std::deque<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push_back(i);
	w.stop();

	std::cout << "benchmark deque" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_forward_list(std::size_t limit)
{
	stopwatch w;
	std::forward_list<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push_front(i);
	w.stop();

	std::cout << "benchmark forward_list" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void
benchmark_queue(std::size_t limit)
{
	stopwatch w;
	std::queue<std::size_t> array;

	w.start();
	for(std::size_t i = 0; i < limit; ++i)
		array.push(i);
	w.stop();

	std::cout << "benchmark queue" << std::endl;
	std::cout << "seconds: " << w.seconds() << std::endl;
	std::cout << "milliseconds: " << w.milliseconds() << std::endl;
	std::cout << "microseconds: " << w.microseconds() << std::endl;
	std::cout << "nanoseconds: " << w.nanoseconds() << std::endl;
}

void 
benchmark()
{
	cpprelude::usize limit = 10000;


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

	benchmark_slinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_slinked_list(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_list(limit);
	std::cout << std::endl;
	benchmark_deque(limit);
	std::cout << std::endl;
	benchmark_forward_list(limit);
	std::cout << std::endl;
	benchmark_dlinked_list(limit);
	std::cout << std::endl;
	benchmark_custom_dlinked_list(limit);

	std::cout <<"============================================================"<< std::endl;

	benchmark_queue(limit);
	std::cout << std::endl;
	benchmark_queue_list(limit);
	std::cout << std::endl;
	benchmark_custom_queue_list(limit);
}