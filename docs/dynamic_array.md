# dynamic_array

`cpprelude::dynamic_array` provides a dynamic array implementation of a contagious array, which is simply a resizable `slice<T>`

### Meta Interface

#### Template Interface

```c++
template<typename T, typename AllocatorT = global_allocator>
struct dynamic_array;
```

1. **T**: specifics element type
2. **AllocatorT**: specifics the type of the allocator the `dynamic_array` will use

#### Alias Interface

```C++
using iterator = sequential_iterator<T>;
using const_iterator = sequential_iterator<const T>;
using data_type = T;
```

1. **iterator**: provide a type definition of the iterator type of this container
2. **const_iterator**: provide a type definition of the const iterator type of this container
3. **data_type**: provide a type definition of the element type of this container

### Interface
#### Constructor
```C++
dynamic_array(const AllocatorT& allocator = AllocatorT());
dynamic_array(std::initializer_list<T> list, const AllocatorT& allocator = AllocatorT());
dynamic_array(usize count, const AllocatorT& allocator = AllocatorT());
dynamic_array(usize count, const T& fill_value, const AllocatorT& allocator = AllocatorT());
dynamic_array(const dynamic_array<T, AllocatorT>& other);
dynamic_array(const dynamic_array<T, AllocatorT>& other, const AllocatorT& allocator)
dynamic_array(dynamic_array<T, AllocatorT>&& other);
dynamic_array(dynamic_array<T, AllocatorT>&& other, const AllocatorT& allocator);
```

1. A Constructor that builds a dynamic array with the provided allocator
2. A Constructor that accepts an initializer list
3. A Constructor that accepts a known count and constructs a dynamic_array with that count
4. A Constructor that accepts a count and a value that's used to fill the dynamic array
5. A copy constructor
6. A copy constructor with a new allocator for the new dynamic_array
7. A move constructor
8. A move constructor with a new allocator for the new dynamic_array

#### count

```C++
usize count() const;
```

returns the count of elements inside the dynamic array

#### capacity

```C++
usize capacity() const;
```

returns the number of elements this dynamic_array can hold

#### reserve

```C++
void reserve(usize new_count);
```

reserves a memory that can hold the count of elements at the end of the dynamic_array

#### expand_back

```C++
void expand_back(usize additional_count);
void expand_back(usize additional_count, const T& fill_value);
```

1. expands the dynamic array at the end by the count provided
2. expands the dynamic array at the end by the count provided and fill the expanded elements with the fill value

#### shrink_back

```C++
void shrink_back(usize shrinkage_count);
```

shrinks the dynamic array from the back by the count provided

#### shrink_to_fit

```C++
void shrink_to_fit();
```

trims the dynamic array from the back to exactly fit the count of elements inside

#### operator[]

```C++
T& operator[](usize index);
const T& operator[](usize index) const;
```

returns the element at the index position

#### data

```C++
T* data();
const T* data() const;
```

returns a pointer to the start of the memory

#### insert_back

```C++
void insert_back(std::initializer_list<T> list);
void insert_back(const T& value);
void insert_back(T&& value);
```

1. inserts the list at the back of the dynamic array
2. inserts the element value at the back of the dynamic array
3. inserts the element value at the back of the dynamic array

#### remove_back

```C++
void remove_back(usize removal_count = 1);
```

removes the count provided from the back of the dynamic array

#### clear

```c++
void clear();
```

clears the dynamic array elements

#### reset

```C++
void reset();
```

clears the dynamic array elements and frees the memory of the dynamic array

#### empty

```C++
bool empty() const;
```

returns whether the dynamic array is empty or not

#### front

```C++
const_iterator front() const;
iterator front();
```

returns an iterator pointing at the front of the dynamic_array

#### back

```C++
const_iterator back() const;
iterator back();
```

returns an iterator pointing at the back (last element) of the dynamic array

#### begin

```C++
const_iterator beign() const;
const_iterator cbegin() const;
iterator begin();
```

returns an iterator pointing at the start of the dynamic array

#### end

```C++
const_iterator end() const;
const_iterator cend() const;
iterator end();
```

returns an iterator pointing at one-element past the last element of the dynamic array

#### decay

```C++
slice<T> decay();
slice<T> decay_continuous();
```

1. decays the current container to just a slice of memory containing the elements and resets this container
2. just like decay but there's a guarantee that the returned slice will have a contagious memory containing all the elements 

### Example

```C++
#include <iostream>
#include <cpprelude/dynamic_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	dynamic_array<usize> numbers;

	numbers.reserve(10);
  
	for(usize i = 0; i < 10; ++i)
		numbers.insert_back(i+1);

	numbers.insert_back({11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

	for(const auto& number: numbers)
		std::cout << number << std::endl;
	return 0;
}
```

