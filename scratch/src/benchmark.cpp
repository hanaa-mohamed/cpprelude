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
#include <cpprelude/micro_benchmark.h>

using namespace cpprelude;

//Vector like container
usize
benchmark_dynamic_array(abstract_benchmark* bench, usize limit)
{
	dynamic_array<usize> array;
	
	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
benchmark_custom_dynamic_array(abstract_benchmark* bench, usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	slice<ubyte> mem_block;
	auto arena_allocator = make_arena_allocator(MEGABYTES(25), mem_block);

	usize result = 0;
	{
		dynamic_array<usize, linear_allocator> array(arena_allocator);
		bench->watch.start();
		for(usize i = 0; i < limit; ++i)
			array.insert_back(rand());
		result = *array.begin();
		bench->watch.stop();
	}

	virtual_free(mem_block);
	return result;
}

usize
benchmark_vector(abstract_benchmark* bench, usize limit)
{
	std::vector<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_back(rand());

	bench->watch.stop();

	return *array.begin();
}

//single linked list containers
usize
benchmark_slinked_list(abstract_benchmark* bench, usize limit)
{
	slinked_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
benchmark_custom_slinked_list(abstract_benchmark* bench, usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	slice<ubyte> mem_block;
	auto arena_allocator = make_arena_allocator(MEGABYTES(25), mem_block);

	usize result = 0;
	{
		slinked_list<usize, linear_allocator> array(arena_allocator);
		bench->watch.start();
		for(usize i = 0; i < limit; ++i)
			array.insert_front(rand());
		result = *array.begin();
		bench->watch.stop();
	}

	virtual_free(mem_block);
	return result;
}

usize
benchmark_forward_list(abstract_benchmark* bench, usize limit)
{
	std::forward_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_front(rand());

	bench->watch.stop();

	return *array.begin();
}

//double linked list container
usize
benchmark_dlinked_list(abstract_benchmark* bench, usize limit)
{
	dlinked_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
benchmark_custom_dlinked_list(abstract_benchmark* bench, usize limit)
{
	double avg_sec = 0, avg_milli = 0, avg_micro = 0, avg_nano = 0;

	slice<ubyte> mem_block;
	auto arena_allocator = make_arena_allocator(MEGABYTES(25), mem_block);

	usize result = 0;
	{
		dlinked_list<usize, linear_allocator> array(arena_allocator);
		bench->watch.start();
		for(usize i = 0; i < limit; ++i)
			array.insert_front(rand());
		result = *array.begin();
		bench->watch.stop();
	}

	virtual_free(mem_block);
	return result;
}

usize
benchmark_list(abstract_benchmark* bench, usize limit)
{
	std::list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_front(rand());

	bench->watch.stop();

	return *array.begin();
}

//stack containers
usize
benchmark_stack_array(abstract_benchmark* bench, usize limit)
{
	stack_array<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
benchmark_stack_list(abstract_benchmark* bench, usize limit)
{
	stack_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
benchmark_stack(abstract_benchmark* bench, usize limit)
{
	std::stack<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

//queue containers
usize
benchmark_queue_array(abstract_benchmark* bench, usize limit)
{
	queue_array<usize> array;

	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
benchmark_queue_list(abstract_benchmark* bench, usize limit)
{
	queue_list<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
benchmark_queue(abstract_benchmark* bench, usize limit)
{
	std::queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.front();
}

//priority queue containers
usize
benchmark_priority_queue(abstract_benchmark* bench, usize limit)
{
	priority_queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
benchmark_std_priority_queue(abstract_benchmark* bench, usize limit)
{
	std::priority_queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

//deque containers
usize
benchmark_bucket_array(abstract_benchmark* bench, usize limit)
{
	bucket_array<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
benchmark_deque(abstract_benchmark* bench, usize limit)
{
	std::deque<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push_back(rand());

	bench->watch.stop();

	return *array.begin();
}

//merge_sort
void
benchmark_merge_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::merge_sort(array.begin(), array.count());
	bench->watch.stop();
}

void
benchmark_std_stable_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::stable_sort(array.begin(), array.end());
	bench->watch.stop();
}

//quick sort
void
benchmark_quick_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::quick_sort(array.begin(), array.count());
	bench->watch.stop();
}

void
benchmark_std_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::sort(array.begin(), array.end());
	bench->watch.stop();
}

void
benchmark_heap_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::heap_sort(array.begin(), array.count());
	bench->watch.stop();
}

void
benchmark_std_heap_sort(abstract_benchmark* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	bench->watch.stop();
}

void
benchmark_hash_array(abstract_benchmark* bench, usize limit)
{
	hash_array<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
benchmark_unordered_map(abstract_benchmark* bench, usize limit)
{
	std::unordered_map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.emplace(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.find(i);
		array.erase(it);
	}
	bench->watch.stop();
	return;
}

void
benchmark_tree_map(abstract_benchmark* bench, usize limit)
{
	tree_map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
benchmark_map(abstract_benchmark* bench, usize limit)
{
	std::map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.emplace(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.find(i);
		array.erase(it);
	}
	bench->watch.stop();
	return;
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
check_binary_semaphore(cpprelude::usize limit)
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

	std::cout << "check binary_semaphore" << std::endl;
	std::cout << "counter: " << sema._counter << std::endl;
}

void
check_count_semaphore(cpprelude::usize limit)
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

	std::cout << "check count_semaphore" << std::endl;
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
check_thread_unique(cpprelude::usize limit)
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

	std::cout << "check thread_unique" << std::endl;
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
				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));
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
				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));

				continue;
			}
		}
	}
}

cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>
thread_multi_reader_job_queue;

void
check_thread_multi_reader(cpprelude::usize limit)
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

	std::cout << "check thread_multi_reader" << std::endl;
	std::cout << "empty: " << thread_multi_reader_job_queue.value.empty() << std::endl;
	std::cout << "count: " << thread_multi_reader_job_queue.value.count() << std::endl;
}

#define BENCHMARK(func_name, limit) cpprelude::make_benchmark(#func_name, func_name, limit)

void
do_benchmark()
{
	cpprelude::usize limit = 100;

	std::cout << "\nBENCHMARK START\n" << std::endl;

	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_vector, limit),
		BENCHMARK(benchmark_dynamic_array, limit),
		BENCHMARK(benchmark_custom_dynamic_array, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_forward_list, limit),
		BENCHMARK(benchmark_slinked_list, limit),
		BENCHMARK(benchmark_custom_slinked_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_list, limit),
		BENCHMARK(benchmark_dlinked_list, limit),
		BENCHMARK(benchmark_custom_dlinked_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_stack, limit),
		BENCHMARK(benchmark_stack_array, limit),
		BENCHMARK(benchmark_stack_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_queue, limit),
		BENCHMARK(benchmark_queue_array, limit),
		BENCHMARK(benchmark_queue_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_std_priority_queue, limit),
		BENCHMARK(benchmark_priority_queue, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_deque, limit),
		BENCHMARK(benchmark_bucket_array, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_std_stable_sort, limit),
		BENCHMARK(benchmark_merge_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_std_sort, limit),
		BENCHMARK(benchmark_quick_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_std_heap_sort, limit),
		BENCHMARK(benchmark_heap_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_unordered_map, limit),
		BENCHMARK(benchmark_hash_array, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		BENCHMARK(benchmark_map, limit),
		BENCHMARK(benchmark_tree_map, limit)
	});

	std::cout << std::endl;
	check_binary_semaphore(limit/10);
	std::cout << std::endl;
	check_count_semaphore(limit/10);
	std::cout << std::endl;
	check_thread_unique(limit/10);
	std::cout << std::endl;
	check_thread_multi_reader(limit/10);


	std::cout << "\nBENCHMARK END\n" << std::endl;
}
