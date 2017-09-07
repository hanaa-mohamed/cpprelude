# is_sorted

`cpprelude::is_sorted` a function that checks whether this sequence of elements is sorted ascending or descending or according to the `Comparator` parameter.

### Function signature

```c++
template<typename iterator_type, typename Comparator = tmp::default_less_than<typename iterator_type::data_type>>
bool 
is_sorted(iterator_type begin_it, usize count, 
               Comparator compare = Comparator())
```

This function checks whether the given range is sorted or not. It compares elements using operator < by default. Otherwise it can be specified using the 3rd parameter.

#### Parameters

1. `begin_it`: is an iterator of a template type `iterator_type`. It points to the first element in the range of data.
2. `count`: is a number of `usize` type that specifies number of elements starting from iterator `start`.
3. `compare`: is a function that accepts two elements. It compares them and return true or false. This parameter is by default uses `tmp::default_less_than`.

#### Return value

Returns `true` if the range is sorted else returns `false`.

#### Complexity

Linear.

## Code Example

```c++
#include <iostream>
#include <cpprelude/algorithms.h>
#include <cpprelude/dynamic_array.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
	dynamic_array<int> array;
  	// input is sorted
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);
  	//output is true
	cout<< is_sorted(array.begin(), array.count());
  
	shuffle(array.begin(), array.count());
  	//output is false
	cout<< is_sorted(array.begin(), array.count());
	return 0;
}
```

