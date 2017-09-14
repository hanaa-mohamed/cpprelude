# Stack_list

`cpprelude::stack_list`  is a container that its last inserted element is its first output in the container a.k.a. LIFO principle. This stack is implemented using single-linked list. 

### Meta interface

#### Template interface

```c++
template<typename T, typename AllocatorT = global_allocator>
struct stack_list;
```

1. **T**: specifies the type of stack elements.

2. **AllocatorT**: specifies how to allocate in memory by default it is `global_allocator`.

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
   stack_list(const AllocatorT& allocator = AllocatorT());
   stack_list(const stack_array&);
   stack_list(const stack_array& other, const AllocatorT& allocator);
   stack_list(stack_array&&);
   stack_list(stack_array&& other, const AllocatorT& allocator);
   ```

   1. A constructor that builds the container with the provided allocator.
   2. A copy constructor.
   3. A copy constructor that accepts another allocator.
   4. A move constructor.
   5. A move constructor that accepts another allocator.

   #### Member data

   ```c++
   slinked_list<T, AllocatorT> _list;
   ```

   The underneath used container single-linked list that holds stack elements.

   #### push

   ```C++
   void push(const T& item);
   void push(T&& item);
   ```

   1. adds another element to the stack. It guaranties no modification will happen to the passed `item`.
   2. adds another element to the stack with moving the `item`.

   ​

   #### pop

   ```C++
   bool pop();
   ```

   Removes the last element inserted and returns `bool` to check if it is a successful operation or not. 

   #### top

   ```C++
   const T& top() const;
   T& top();
   ```

   1. returns const reference of the first element in the stack.
   2. returns reference of the first element in the stack.

   #### count

   ```c++
   usize count() const;
   ```

   Returns number of elements  inside the stack.

   #### empty

   ```c++
   bool empty() const;
   ```

   Checks if the stack is empty or not.



## Code Example

```c++
#include <iostream>
#include <cpprelude/stack_list.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
  //initializes the stack with initial size
  stack_list<usize> arr(5);

  //Adding 100 elements
  for (usize i = 0; i<100; i++)
    arr.push(i);
 //Removing elements untill it reaches number 50 on the top of the stack
  while (arr.top() != 50)
    arr.pop();
  
	return 0;
}
```

