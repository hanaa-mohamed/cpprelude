// #include <iostream>
// #include <cpprelude/hash_array.h>
// #include <cpprelude/string.h>
// using namespace cpprelude;
//
// struct v2
// {
//     i32 x, y;
//
//     bool
//     operator==(const v2& other) const
//     {
//         return x == other.x && y == other.y;
//     }
// };
//
// struct v2_hasher
// {
//     inline usize
//     operator()(const v2& point) const
//     {
//         hash<i32> hasher;
//         return hasher(point.x) ^ (hasher(point.y) << 1);
//     }
// };
//
// int
// main(int argc, char** argv)
// {
//     hash_array<v2, usize, v2_hasher> another_array;
//
//     v2 point;
//     point.x = 4;
//     point.y = 2;
//     another_array[point] = 50;
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
//
// 	for(const auto& key: array)
// 		std::cout << key << std::endl;
//
// 	for(auto it = array.cbegin(); it != array.cend(); ++it)
// 		std::cout << "key: " << *it << std::endl;
// 	return 0;
// }
