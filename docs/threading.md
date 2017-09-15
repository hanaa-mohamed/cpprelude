# Threading

Threading file provides a simple multithreading programming primitives.

## binary_semaphore
```c++
struct binary_semaphore;
```

### Interface

#### take
```c++
isize take();
isize wait_take(r32 micros = 0);
```

1. tries to take a ticket from this semaphore and returns its id if it fails it returns -1
2. tries to take a ticket from this semaphore if it fails it sleeps for the provided amount of micros then try again if micros is 0 then it will not sleep

#### give
```c++
isize give();
isize wait_give(r32 micros = 0);
```

1. tries to give a ticket back to this semaphore and returns its id if it fails it returns -1
2. tries to give a ticket back to this semaphore and returns its id if it fails it will sleep for the provided amount of micros then try again if micros is 0 then it will not sleep

#### available
```c++
bool available() const;
```

Returns whether there're any available tickets or not

## count_semaphore

```c++
template<isize max_count>
struct count_semaphore;
```

### Interface

#### take
```c++
isize take();
isize wait_take(r32 micros = 0);
```

1. tries to take a ticket from this semaphore and returns its id if it fails it returns -1
2. tries to take a ticket from this semaphore if it fails it sleeps for the provided amount of micros then try again if micros is 0 then it will not sleep

#### give
```c++
isize give();
isize wait_give(r32 micros = 0);
```

1. tries to give a ticket back to this semaphore and returns its id if it fails it returns -1
2. tries to give a ticket back to this semaphore and returns its id if it fails it will sleep for the provided amount of micros then try again if micros is 0 then it will not sleep

#### available
```c++
bool available() const;
```

Returns whether there're any available tickets or not

## thread_unique

Wraps a type T and provide an easy to use access to it in a thread safe manner in this case it guarantees access to one thread at a time

```c++
template<typename T>
struct thread_unique;
```

### Interface

#### read
```c++
bool read(thread_context& context, r32 micros = 0);
```

Gives the current thread a read permission in the provided context and if it fails due to other thread having the access then it will sleep for the provided micros. if micros is 0 then it will not sleep

#### read_release
```c++
bool read_release(thread_context& context, r32 micros = 0);
```

Release the read access of the calling thread in the provided context and if it fails it will sleep for the amount of the provided micros. if micros is 0 then it will not sleep

#### write
```c++
bool write(thread_context& context, r32 micros = 0);
```
Gives the current thread a write permission in the provided context and if it fails due to other thread having the access then it will sleep for the provided micros. if micros is 0 then it will not sleep

#### write_release
```c++
bool write_release(thread_context& context, r32 micros = 0);
```
Release the write access of the calling thread in the provided context and if it fails it will sleep for the amount of the provided micros. if micros is 0 then it will not sleep

#### new_context
```c++
thread_context new_context();
```

Creates a new context of accessing to the current thread calling this function the caller must save the returned context as it's necessary to acquire any read or write permission


## thread_multi_reader

Wraps a type T and provide an easy to use access to it in a thread safe manner in this case it guarantees access to a single unique writer or to multi readers at a time.

```c++
template<typename T>
struct thread_multi_reader;
```

### Interface

#### read
```c++
bool read(thread_context& context, r32 micros = 0);
```

Gives the current thread a read permission in the provided context and if it fails due to other thread having the access then it will sleep for the provided micros. if micros is 0 then it will not sleep

#### read_release
```c++
bool read_release(thread_context& context, r32 micros = 0);
```

Release the read access of the calling thread in the provided context and if it fails it will sleep for the amount of the provided micros. if micros is 0 then it will not sleep

#### write
```c++
bool write(thread_context& context, r32 micros = 0);
```
Gives the current thread a write permission in the provided context and if it fails due to other thread having the access then it will sleep for the provided micros. if micros is 0 then it will not sleep

#### write_release
```c++
bool write_release(thread_context& context, r32 micros = 0);
```
Release the write access of the calling thread in the provided context and if it fails it will sleep for the amount of the provided micros. if micros is 0 then it will not sleep

#### new_context
```c++
thread_context new_context();
```

Creates a new context of accessing to the current thread calling this function the caller must save the returned context as it's necessary to acquire any read or write permission

### Example

```C++
#include <iostream>
#include <thread>
#include <cpprelude/queue_array.h>
#include <cpprelude/threading.h>
using namespace cpprelude;

void producer(thread_unique<queue_array<i32>>* production_queue, i32 limit)
{
	thread_context context = production_queue->new_context();
	while(limit--)
	{
		if(production_queue->write(context))
		{
			production_queue->value.enqueue(limit);
			production_queue->write_release(context);
		}
	}
}

void consumer(thread_unique<queue_array<i32>>* production_queue, i32 limit)
{
	thread_context context = production_queue->new_context();
	while(limit)
	{
		if(production_queue->write(context))
		{
			if(!production_queue->value.empty())
			{
				std::cout << "consumed: " << production_queue->value.front() << std::endl;
				production_queue->value.dequeue();
				limit--;
			}
			else
			{
				std::cout << "nothing!" << std::endl;
			}

			production_queue->write_release(context);
		}
	}
}

int
main(int argc, char** argv)
{
	thread_unique<queue_array<i32>> production_queue;

	std::thread p1(producer, &production_queue, 1000);
	std::thread p2(producer, &production_queue, 1000);

	std::thread c1(consumer, &production_queue, 500);
	std::thread c2(consumer, &production_queue, 1500);

	p1.join();
	p2.join();
	c1.join();
	c2.join();

	std::cout << (production_queue.value.empty() ? "queue is empty" : "oops! not empty queue") << std::endl;
	return 0;
}
```
