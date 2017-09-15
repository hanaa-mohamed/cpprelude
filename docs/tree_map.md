# Tree_map

`cpprelude::tree_map`  is an associative container that stores elements formed of unique key and a mapped value. 

### Meta interface

#### Template interface

```c++
template<typename KeyType, 
		typename ValueType,
		typename ComparatorType = tmp::default_less_than<details::pair_node<KeyType,ValueType>>,
		typename AllocatorT = global_allocator>
using tree_map = red_black_map<KeyType, ValueType, ComparatorType, AllocatorT>;
```

This container is build on `red_black_map` data structure which inherits from `red_black_tree`.

1. **KeyType**: specifies the type of the key.

2. **ValueType**: specifies the type of the value mapped to the key.

3. **ComparatorType**: a function that determines the comparing operator between keys in the `pair_node`. By default this operator is set to < .

4. **AllocatorT**: specifies how to allocate in memory by default it is `global_allocator`.

   ​

   #### Alias Interface

   ```C++
   using key_type = KeyType;
   using value_type = ValueType;
   using data_type = details::pair_node<key_type, value_type>;
   using node_type = details::red_black_tree_node<data_type>;
   using iterator = red_black_tree_pair_iterator<key_type, value_type>;
   using const_iterator = red_black_tree_const_pair_iterator<key_type, value_type>;
   using color_type = typename node_type::color_type;
   ```

   1. **key_type**:  provides a type definition of the key type of this container.
   2. **value_type**:  provides a type definition of the value type of this container.
   3. **data_type**: provides a type definition of the `pair_node` type of this container.
   4. **node_type**: provides a type definition of the node type that this container uses.
   5. **iterator**: provides a type definition of the iterator type of this container.
   6. **const_iterator**: provides a type definition of the const iterator type of this container.
   7. **color_type**: provides a type definition of the color node.

   ### Interface

   #### Constructor

   ```c++
   red_black_map(const ComparatorType& compare_function = ComparatorType(), 
                  const AllocatorT& allocator = AllocatorT());
   red_black_map(std::initializer_list<T> list,
                  const ComparatorType& compare_function = ComparatorType(),
                  const AllocatorT& allocator = AllocatorT());
   red_black_map(const red_black_map& other);
   red_black_map(const red_black_map& other,
                  const ComparatorType& compare_function);
   red_black_map(const red_black_map& other,
   			const AllocatorT& allocator);
   red_black_map(const red_black_map& other,
   			const AllocatorT& allocator,
   			const ComparatorType& compare_function);
   red_black_map(red_black_map&& other);
   red_black_map(red_black_map&& other, 
                  const ComparatorType& compare_function);
   red_black_tree(red_black_tree&& other, const AllocatorT& allocator);
   red_black_tree(red_black_tree&& other, 
                  const AllocatorT& allocator,
                  const ComparatorType& compare_function); 
   ```

   1. A constructor that builds the `red_black_map` a.k.a `tree_map` with the provided function comparison and allocator.
   2. A constructor that initializes `red_black_map` with the provided `initializer_list` using the provided `compare_function`  to compare between elements and `allocator` to allocate nodes.
   3. A copy constructor.
   4. A copy constructor that accepts another comparator function.
   5. A copy constructor that accepts another allocator.
   6. A copy constructor that accepts another comparator function and another allocator.
   7. A move constructor.
   8. A move constructor that accepts another comparator function.
   9. A move constructor that accepts another allocator.
   10. A move constructor that accepts another comparator function and another allocator.

   #### operator[]

   ```c++
   value_type& operator[](const key_type& key);
   const value_type& operator[](const key_type& key) const;
   ```

   Returns the value mapped with the given `key`.

   #### insert

   ```c++
   iterator insert(const key_type& key);
   iterator insert(key_type&& key);
   iterator insert(const key_type& key, const value_type& value);
   iterator insert(key_type&& key, const value_type& value);
   iterator insert(const key_type& key, value_type&& value);
   iterator insert(key_type&& key, value_type&& value);
   ```

   1. adds another `pair_node` to the map with a given key and no value mapped to it. It guaranties no modification will happen to the passed `key`.
   2. adds another `pair_node `to the map with moving the `key` and no value mapped to it.
   3. adds another `pair_node` to the map with a given key and a given value mapped to it. It guaranties no modification will happen to the passed `key` or `value`.
   4. adds another `pair_node` to the map with a moving key and a given value mapped to it. It guaranties no modification will happen to the passed  `value`.
   5. adds another `pair_node` to the map with a given key and a moving value mapped to it. It guaranties no modification will happen to the passed  `key`.
   6. adds another `pair_node` to the map with a moving key and a moving value mapped to it. 

   #### remove

   ```c++
   void remove(const key_type& key);
   void remove(key_type&& key);
   ```

    Removes node that has the `key`from the map. 

   ​

   #### lookup

   ```c++
   iterator lookup(const key_type& key);
   iterator lookup(key_type&& key);
   const_iterator lookup(const key_type& key) const;
   const_iterator lookup(key_type&& key) const;
   ```

   Looks up for the node that has `key`and returns `iterator` or `const_iterator` that points to it. 

   #### begin

   ```c++
   const_iterator beign() const;
   const_iterator cbegin() const;
   iterator begin();
   ```

   Returns an iterator pointing at the min node in the container.

   #### end

   ```c++
   const_iterator end() const;
   const_iterator cend() const;
   iterator end();
   ```

   Returns a nullptr iterator.

   ​

   #### Code Example


```c++
#include <iostream>
#include <cpprelude/tree_map.h>
using namespace cpprelude;

int
main(int argc, char** argv)
{
    	// Creating an empty set.
		tree_map<usize, std::string> tree;

		//inserting some pairs
		tree.insert(3, "33");
		tree.insert(2, "22");
		//this one has no value
		tree.insert(1);
		
		//using [] to change value at key 1
		tree[1] = "44";
		//searching for that key and printing its value
		auto h = tree.lookup(1);
		std::cout << h.value() << std::endl; 
  
		//Lambda function that appends the given data to the value 
  		//then it prints the the key and the value.
		auto insert = [](tree_map<usize, std::string>::iterator it, std::string* data) {
			it->append(*data);
			std::cout << it.key() << " " << it.value() << std::endl;
		};
  
		//Applying insert-lambda function by calling inorder_traverse
		std::string aa = "AA";
		tree.inorder_traverse(insert, &aa);
	return 0;
}
```

