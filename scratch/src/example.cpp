// #include <iostream>
// #include <cpprelude/dynamic_array.h>
// #include <cpprelude/allocator.h>
// #include <cpprelude/tree_map.h>
// #include <cpprelude/tmp.h>
// #include <cpprelude/string.h>
// using namespace cpprelude;
//
// int
// main(int argc, char** argv)
// {
// 	auto memory = virtual_alloc<ubyte>(MEGABYTES(1));
// 	auto allocator = linear_allocator(memory);
//
// 	{
// 		tree_map<string, usize, tmp::default_less_than<details::pair_node<string, usize>>, linear_allocator> dic(allocator);
//
// 		dic.insert("mostafa"_s, 234);
// 		dic.insert("mosta"_s, 234);
// 		dic.insert(""_s, 234);
//
// 		for(auto it = dic.cbegin(); it != dic.cend(); ++it)
// 		{
// 			std::cout << it.key() << std::endl;
// 			std::cout << it.value() << std::endl;
// 		}
// 	}
//
// 	virtual_free(memory);
// 	return 0;
// }
