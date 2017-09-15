# Priority_queue

`cpprelude::priority_queue`  is a queue container that its first output is always the most important value in the container. It acts as a heap. By default it is a min-heap, but it can be set to max-heap by using comparator argument. 

### Meta interface

#### Template interface

```c++
template<typename T, typename Comparator = tmp::default_less_than<T>, typename AllocatorT = global_allocator>
struct priority_queue;
```

1. **T**: specifies the type of queue elements.

2. **Comparator**: determines the comparing operator which will set this queue to always output its first element to the minimum or always to the maximum or as the user specifies. By default it is a min-queue. 

3. **AllocatorT**: specifies how to allocate in memory by default it is `global_allocator`.

   ​

   **Code example**

   ```c++
   #include <iostream>
   #include <cpprelude/priority_queue.h>
   using namespace cpprelude;

   int
   main(int argc, char** argv)
   {
     // A queue with usize elements that outputs always minimum values first and uses global_allocator as a default allocator.
   	priority_queue<usize> usize_queue_1;
     // A queue with usize elements that outputs always maximum values first and uses global_allocator as a default allocator.
     	auto fun = [](const usize& x, const usize& y) {
   			return x > y;
   		};
   	priority_queue<usize> usize_queue_2(fun);
       // A queue with usize elements that outputs always maximum values first and uses linear_allocator.
   	priority_queue<usize> usize_queue_3 
         (fun, cpprelude::linear_allocator);
     
   	return 0;
   }
   ```

   #### Using interface

   ```C++
   using data_type = T;
   ```

   **data_type**: provides a type definition of the element type of this container.

   ​

   ### Interface

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

   Removes the most important value in the queue first and returns `bool` to check if it is a successful operation or not. 

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

   #### decay

   ```c++
   slice<T> decay();
   slice<T> decay_continuous();
   ```

   1. Decays the current container to just a slice of memory containing the elements and resets this container.
   2. Just like decay but there's a guarantee that the returned slice will have a contagious memory containing all the elements.



## Code Example

```c++
#include <iostream>
#include <cpprelude/priority_queue.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
 	priority_queue<usize> usize_queue;
	usize_queue.enqueue(1);
	usize_queue.enqueue(5);
	usize x = usize_queue.front();
	std::cout <<x;
	usize_queue.dequeue(); // removes 1
  
	return 0;
}
```

