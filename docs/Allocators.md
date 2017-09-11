# Allocators

Allocators are object that's wrapping a memory + memory allocation algorithms they have a standard interface consisting of 3 functions. you can create your own allocator and pass it to the containers.

### global_allocator
#### alloc
```C++
template<typename T>
slice<T>
alloc(usize count = 1, ubyte alignment = 4);
```

Allocates a slice of type T that can hold up to the provided `count` with the provided alignment.

#### free
```C++
template<typename T>
void
free(slice<T>& slice_);

template<typename T>
void
free(slice<T>&& slice_)
```

Frees an allocated slice of type T given that it's allocated using this allocator. also resets the slice to an invalid state which points to nullptr.

#### realloc
```C++
template<typename T>
void
realloc(slice<T>& slice_, usize count);

template<typename T>
void
realloc(slice<T>&& slice_, usize count);
```

Reallocates the provided slice to fit the count.
* if count is less than the count of the provided slice then it will be shrunk.
* if count is more than the count of the provided slice then it will be enlarged.
* if the provided slice is an empty slice then realloc should behave exactly like alloc

### linear_allocator
Provides a linear -stack like- allocation algorithm. the provided memory to allocate from should be freed explicitly.

#### Constructor
```C++
linear_allocator();
template<typename T>
linear_allocator(slice<T> memory);
```

1. Creates an empty linear allocator
2. Creates a linear allocator with the provided memory to allocate from

#### alloc
```C++
template<typename T>
slice<T>
alloc(usize count = 1, ubyte alignment = 4);
```

If there's a space available then it moves the allocation head and returns a slice pointing to the allocated memory. if there's no space then it will return an empty slice.

#### free

```C++
template<typename T>
void
free(slice<T>& slice_);

template<typename T>
void
free(slice<T>&& slice_);
```

If this is the latest allocation then it will decrement the allocation header. if not then it will only decrease the allocation count and if the allocation count is 0 it will reset the allocation head back to the beginning.

#### realloc

```C++
template<typename T>
void
realloc(slice<T>&& slice_, usize count);
```

Reallocates the provided slice. if the slice is the latest slice allocated then it will only move the allocation head accordingly. if it's not then it will try to allocate a new slice and copy the data to the new slice.

### Example

```C++
#include <iostream>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/allocator.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	auto memory = virtual_alloc<ubyte>(MEGABYTES(1));
	auto allocator = linear_allocator(memory);

	dynamic_array<usize, linear_allocator> array(allocator);

	for(usize i = 0; i < 10; ++i)
		array.insert_back(i);

	for(const auto& element: array)
		std::cout << element << std::endl;

	virtual_free(memory);
	return 0;
}
```
