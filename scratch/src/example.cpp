// #include <iostream>
// #include <cpprelude/hash_array.h>
// #include <cpprelude/string.h>
// using namespace cpprelude;
//
// int
// main(int argc, char** argv)
// {
// 	hash_array<literal, usize> array;
//
// 	array["a"_l] = 0;
// 	array["b"_l] = 1;
// 	array["c"_l] = 2;
//
// 	for(const auto& key: array.keys())
// 		std::cout << key << std::endl;
//
// 	for(const auto& value: array.values())
// 		std::cout << value << std::endl;
//
// 	if(array.lookup("d"_l) == array.end())
// 		std::cout << "d doesn't exist" << std::endl;
//
// 	if(array.lookup("b"_l) != array.end())
// 		std::cout << "b does exist" << std::endl;
//
// 	for(auto it = array.cbegin(); it != array.cend(); ++it)
// 		std::cout << "key: " << it.key() << ", value: " << it.value() << std::endl;
// 	return 0;
// }
