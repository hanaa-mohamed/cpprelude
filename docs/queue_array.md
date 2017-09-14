# Queue_array

`cpprelude::queue_array`  is a container that its first inserted element from one end of the container is its first output in the other end of the container a.k.a. FIFO principle. This queue is implemented using array. 

### Meta interface

#### Template interface

```c++
template<typename T,
		 usize buffer_size = details::default_size(sizeof(T)),
		 typename AllocatorT = global_allocator>
struct queue_array;
```

1. **T**: specifies the type of queue elements.

2. **buffer_size**: determines number of elements that queue can hold.

3. **AllocatorT**: specifies how to allocate in memory by default it is `global_allocator`.

   ​

   #### Using interface

   ```C++
   using data_type = T;
   ```

   **data_type**: provides a type definition of the element type of this container.

   ​

   ### Interface

   #### Constructor

   ```c++
   queue_array(const AllocatorT& allocator = AllocatorT());
   ```

   A constructor that builds the container with the provided allocator.

   #### Member data

   ```c++
   bucket_array<T, buffer_size, AllocatorT> _list;
   ```

   The underneath used container array that holds queue elements.

   #### enqueue

   ```C++
   void enqueue(const T& item);
   void enqueue(T&& item);
   ```

   1. adds another element to the queue. It guaranties no modification will happen to the passed `item`.
   2. adds another element to the queue with moving the `item`.

   ​

   #### dequeue

   ```C++
   bool dequeue();
   ```

   Removes the first element that is inserted to the queue and returns `bool` to check if it is a successful operation or not.  

   #### front

   ```C++
   const T& front() const;
   T& front();
   ```

   1. returns const reference of the first element in the queue.
   2. returns reference of the first element in the queue.

   #### count

   ```c++
   usize count() const;
   ```

   Returns number of elements  inside the queue.

   #### empty

   ```c++
   bool empty() const;
   ```

   Checks if the queue is empty or not.



## Code Example

```c++
#include <iostream>
#include <cpprelude/queue_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
 	queue_array<usize> queue;
	queue.enqueue(1);
	queue.enqueue(5);
	usize x = queue.front();
	std::cout <<x;
	queue.dequeue(); // removes 1
  
	return 0;
}
```

