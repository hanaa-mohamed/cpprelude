# string

`cpprelude::string` is simply a `string_slice<Char_Type>` which in turn is just a slice of memory `slice<Char_Type>` that stores the string data. Once the string is initialized it no longer can be resized but the data inside it can be altered in any way or form. but resizing isn't enabled.
`cpprelude::string` needs manual memory management if it's dynamically allocated then you need to explicitly dispose it.

### Meta Interface

#### Template Interface

```c++
template<typename T>
struct string_slice;
```

1. **T**: specifics char type.

#### Alias Interface

```C++
using iterator = sequential_iterator<T>;
using const_iterator = sequential_iterator<const T>;
using data_type = T;
```

1. **iterator**: provide a type definition of the iterator type of this container.
2. **const_iterator**: provide a type definition of the const iterator type of this container.
3. **data_type**: provide a type definition of the char type of this container.

### Interface
#### Constructor
```C++
string_slice();
string_slice(const slice<T>& data);
string_slice(slice<T>&& data);
string_slice(T* data, usize str_count);
```

1. Creates an empty string.
2. Creates a string with the provided data however it doesn't copy the slice it takes it as is a.k.a. it will own the slice.
3. Creates a string with the provided data.
4. Creates a string with the provided pointer and count. memory management depends on the nature of the pointer. if it's dynamically allocated then it needs to be disposed.

#### view

```C++
string_slice view(usize offset, usize char_count = 0);
```

Creates a view a.k.a. sub-string of this string starting at `offset` and with the length of `char_count` if `char_count = 0` then the sub-string will extend to the end of the original string.

#### count

```C++
usize count() const;
```

Returns the count of chars inside this string.

#### capacity

```C++
usize capacity() const;
```

Returns the number of the chars this string can hold.

#### reset

```C++
void reset();
```
rests the data this string holds. if this is the last instance of the string then there will be a memory leak. you have to dispose the string exactly once.

#### empty

```C++
bool empty() const;
```

Returns whether the string is empty or not.

#### data

```C++
T* data();
const T* data() const;
```

Returns a pointer to the start of the string.

#### operator[]

```C++
T& operator[](usize index);
const T& operator[](usize index) const;
```

Returns the char at the `index` position.

#### front

```C++
const_iterator front() const;
iterator front();
```

Returns an iterator pointing at the front of the string.

#### back

```C++
const_iterator back() const;
iterator back();
```

Returns an iterator pointing at the back (last char) of the string.

#### begin

```C++
const_iterator beign() const;
const_iterator cbegin() const;
iterator begin();
```

Returns an iterator pointing at the start of the string.

#### end

```C++
const_iterator end() const;
const_iterator cend() const;
iterator end();
```

Returns an iterator pointing at one-char past the last element of the string.

#### decay

```C++
slice<T> decay();
slice<T> decay_continuous();
```

1. Decays the current container to just a slice of memory containing the elements and resets this container.
2. Just like decay but there's a guarantee that the returned slice will have a contagious memory containing all the elements.

### Static Functions

#### from_cstring

```C++
template<typename AllocatorT = global_allocator>
inline static string_slice
from_cstring(const T* str, AllocatorT&& allocator = AllocatorT());

template<typename AllocatorT = global_allocator>
static string_slice
from_cstring(const T* str, usize count, AllocatorT&& allocator = AllocatorT());
```

Creates a string from the provided c-string. note: a string created using these functions needs to be disposed

#### literal

```C++
static string_slice<const T>
literal(const T* str, usize str_count);
```

Creates a literal string from the provided literal value. a literal string cannot be altered and needs no disposing since it doesn't dynamically allocate memory.

#### copy

```C++
template<typename AllocatorT = global_allocator>
static string_slice
copy(const string_slice& str, AllocatorT&& allocator = AllocatorT());
```

Creates a copy of the provided string. since the default behaviour of copying a string `string_slice<char> x = y;` will only copy the head that's pointing to the same data.

#### dispose

```C++
template<typename AllocatorT = global_allocator>
static void
dispose(string_slice& str, AllocatorT&& allocator = AllocatorT());

template<typename AllocatorT = global_allocator>
static void
dispose(string_slice&& str, AllocatorT&& allocator = AllocatorT());
```

Disposes the provided string, and frees its memory.

### type definitions

```C++
template<typename T>
using literal_slice = string_slice<const T>;

using string = string_slice<char>;
using literal = literal_slice<char>;
using ustring = string_slice<wchar_t>;
using uliteral = literal_slice<wchar_t>;
```

### local_string

Provides a convenient way to create a string with a static size that's allocated on the stack. this string doesn't need to be disposed by the end of its usage. it's not advised to move this string around too much since it's not that efficient.

```C++
template<typename T, usize slice_size>
struct local_string_slice: string_slice<T>;
```

```C++
template<usize slice_size>
using local_string = local_string_slice<char, slice_size>;

template<usize slice_size>
using local_ustring = local_string_slice<wchar_t, slice_size>;
```

### User defined literal

Provides a convenient way to create a literal or a string without the fuss of calling the static functions it's advised to use this method to create strings.

```C++
//auto string_literal = "this is a literal string"_l;
//auto normal_string = "this is a normal string"_s;
template<usize slice_size>
using local_string = local_string_slice<char, slice_size>;

template<usize slice_size>
using local_ustring = local_string_slice<wchar_t, slice_size>;
```

### String conversion interface

Provides a simple functions to convert from basic types to string and vice versa.

#### write
```C++
template<typename T, typename FirstT, typename ... ArgsT>
inline static i32
write(string_slice<T>& str, FirstT&& first_arg, ArgsT&&... args);

template<typename T, typename FirstT, typename ... ArgsT>
inline static i32
write(string_slice<T>&& str, FirstT&& first_arg, ArgsT&&... args);
```

Writes the provided values into the string.

#### read

```C++
template<typename T>
inline static bool
read(const string_slice<T>& str, i16& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, i16& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, i32& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, i32& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, i64& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, i64& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, u16& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, u16& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, u32& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, u32& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, u64& result, int base = 10);

template<typename T>
inline static bool
read(const literal_slice<T>& str, u64& result, int base = 10);

template<typename T>
inline static bool
read(const string_slice<T>& str, r32& result);

template<typename T>
inline static bool
read(const literal_slice<T>& str, r32& result);

template<typename T>
inline static bool
read(const string_slice<T>& str, r64& result);

template<typename T>
inline static bool
read(const literal_slice<T>& str, r64& result);
```

Parses a basic_type value from the provided string.

### Example

```C++
#include <iostream>
#include <cpprelude/string.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	literal literal_string = "this is a literal string"_l;
	std::cout << literal_string << std::endl;

	string normal_string = "this is a normal string"_s;
	std::cout << normal_string << std::endl;
	string::dispose(normal_string);

	local_string<256> local_str = "this is a local string";
	std::cout << local_str << std::endl;

	for(const auto& ch: local_str)
		std::cout << ch << std::endl;
	return 0;
}
```
