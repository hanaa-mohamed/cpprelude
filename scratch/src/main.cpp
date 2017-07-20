#include <iostream>
//#include <vld.h>
#include "benchmark.h"
#include <cpprelude/dynamic_array.h>
#include <cpprelude/slinked_list.h>
#include <cpprelude/bucket_list.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/algorithm.h>
#include <cpprelude/bucket_array.h>
#include <cpprelude/tmp.h>
#include <cpprelude/string.h>
#include <cpprelude/hash_array.h>
#include <vector>
#include <cstdlib>
#include <typeinfo>
using namespace cpprelude;

struct screamer
{
	screamer()
	{
		std::cout << "ctor" << std::endl;
	}

	~screamer()
	{
		std::cout << "dtor" << std::endl;
	}

	screamer(const screamer& other)
	{
		std::cout << "copy ctor" << std::endl;
	}

	screamer(screamer&& other)
	{
		std::cout << "move ctor" << std::endl;
	}

	screamer&
	operator=(const screamer& other)
	{
		std::cout << "= copy" << std::endl;
		return *this;
	}

	screamer&
	operator=(screamer&& other)
	{
		std::cout << "= move" << std::endl;
		return *this;
	}
};

void
vec()
{
	std::vector<screamer> vec;
	for(int i = 0; i < 2; ++i)
		vec.push_back(screamer());
}

void
darr()
{
	cpprelude::dynamic_array<screamer> darr;
	for(int i = 0; i < 2; ++i)
		darr.insert_back(screamer());
}

//shuffle_test
void
shuffle_dynamic_array()
{
	cpprelude::dynamic_array<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";

	std::cout << "\nsorted:\n";

	cpprelude::insertion_sort(array.begin(), array.count());
	for (auto& n : array)
		std::cout << n << " ";
}

void
shuffle_slinked_list()
{
	cpprelude::slinked_list<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_front(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";
}

void
shuffle_dlinked_list()
{
	cpprelude::dlinked_list<int> array;
	for(int i = 0; i < 10; ++i)
		array.insert_back(i);

	cpprelude::shuffle(array.begin(), array.count());
	for(auto& n: array)
		std::cout << n << " ";

	std::cout << "\nsorted:\n";

	cpprelude::quick_sort(array.begin(), array.count());
	for (auto& n : array)
		std::cout << n << " ";

}

void
shuffle_test()
{
	shuffle_dynamic_array();
	std::cout << std::endl;
	shuffle_slinked_list();
	std::cout << std::endl;
	shuffle_dlinked_list();
	std::cout << std::endl;
}

void
quick_select_test()
{
	cpprelude::dlinked_list<int> array;
	for (int i = 0; i < 10; ++i)
		array.insert_back(i);
	
	for (auto& n : array)
		std::cout << n << " ";
	std::cout << "\nquick_select(4): " << *quick_select(array.begin(), array.count(), 4) << std::endl;
}

template<typename T>
usize
do_hash(const T& value)
{
	cpprelude::hash<T> hasher;
	return hasher(value);
}

void func_const(const cpprelude::dynamic_array<int>& art)
{
	for(cpprelude::dynamic_array<int>::const_iterator it = art.begin();
		it != art.end();
		++it)
	{
		std::cout << *it << std::endl;
	}
}

void
scratch()
{
	vec();
	std::cout <<"============================================================"<< std::endl;
	darr();

	std::cout <<"============================================================"<< std::endl;

	cpprelude::bucket_list<cpprelude::usize, 16> array;
	for(cpprelude::usize i = 0; i < 16; i++)
		array[i] = i;
	array.expand();
	for(cpprelude::usize i = 16; i < 32; i++)
		array[i] = i;

	cpprelude::dlinked_list<int> arr;
	arr.insert_back({0, 1, 2, 4, 3});

	for(int i = 0; i < arr.count(); ++i)
	{
		auto result = cpprelude::quick_select(arr.begin(), arr.count(), i);
		std::cout << "result("<<i<<") = " << *result << std::endl;
		for(auto n: arr)
			std::cout << n << ", ";
		std::cout << "\n";
		std::cout << "\n";
	}

	//cpprelude::bucket_array<int, 5> koko;

	// std::cout << "bucket_array content: \n";
	// for(int& x: koko)
	// {
	// 	std::cout << x << " ";
	// }

	// std::cout << "\n";

	// for(int i = 0; i < 30; ++i)
	// 	koko.insert_back(i);

	// std::cout << "bucket_array content: \n";
	// for(int& x: koko)
	// {
	// 	std::cout << x << " ";
	// }

	// std::cout << "\n";

	std::cout << "hash(1): " << do_hash(1) << std::endl;
	std::cout << "hash(1.5f): " << do_hash(1.5f) << std::endl;
	std::cout << "hash(1.5f) % 150: " << do_hash(1.5f) % 150 << std::endl;
	std::cout << "hash(1.56f): " << do_hash(1.56f) << std::endl;
	std::cout << "hash(1.56f) % 150: " << do_hash(1.56f) % 150 << std::endl;
	std::cout << "hash(1.567): " << do_hash(1.567) << std::endl;
	std::cout << "hash('a'): " << do_hash('a') << std::endl;
	std::cout << "hash('b'): " << do_hash('b') << std::endl;
	std::cout << "hash(\"mostafa\"): " << do_hash("mostafa"_l) << std::endl;
	std::cout << "hash(\"mostafa\") % 150: " << do_hash("mostafa"_l) % 150 << std::endl;
	auto str = cpprelude::string::from_cstring("mostafa");
	std::cout << "hash(\"mostafa\"): " << do_hash(str) << std::endl;
	std::cout << "hash(\"moustafa\"): " << do_hash("moustafa"_l) << std::endl;
	std::cout << "hash(\"moustapha\"): " << do_hash("moustapha"_l) << std::endl;
	cpprelude::string::dispose(str);

	cpprelude::hash_array<int, char> h_table;
	char ch = 'a';

	for(int i=0;i<26;++i)
		h_table.insert(i, ch++);

	for(auto it = h_table.begin();
		it != h_table.end();
		++it)
	{
		std::cout << "key: " << *it << ", value: " << it.value() << std::endl;
	}

	
	for(auto it = h_table.cbegin();
		it != h_table.end();
		++it)
	{
		std::cout << "key: " << *it << ", value: " << it.value() << std::endl;
	}

	std::cout << "key range: " << std::endl;
	for(auto& key: h_table.keys())
	{
		std::cout << key << std::endl;
	}

	std::cout << "value range: " << std::endl;
	for(const auto& value: h_table.values())
	{
		std::cout << value << std::endl;
	}

	std::cout << "const value range: " << std::endl;
	for(auto& value: h_table.cvalues())
	{
		std::cout << value << std::endl;
	}

	std::cout << "find 5: " << h_table.lookup(5).value() << std::endl;
	std::cout << "find 23: " << h_table.lookup(23).value() << std::endl;
	std::cout << "find 0: " << h_table.lookup(0).value() << std::endl;

	cpprelude::dynamic_array<int> art;
	art.insert_back(4);

	for(const auto& x: art)
		std::cout << x << std::endl;

	func_const(art);
	
	cpprelude::hash_set<usize> ss;
	ss.insert(3);
}

void
test_string_conversion()
{
	cpprelude::local_string<256> str;

	cpprelude::byte byte_v = 67;
	cpprelude::write(str, byte_v);
	std::cout << str << std::endl;

	cpprelude::ubyte ubyte_v = 68;
	cpprelude::write(str, ubyte_v);
	std::cout << str << std::endl;

	cpprelude::i8 i8_v = -69;
	cpprelude::write(str, i8_v);
	std::cout << str << std::endl;

	cpprelude::u8 u8_v = 70;
	cpprelude::write(str, u8_v);
	std::cout << str << std::endl;

	cpprelude::i16 i16_v = -71;
	cpprelude::write(str, i16_v);
	std::cout << str << std::endl;

	cpprelude::u16 u16_v = 72;
	cpprelude::write(str, u16_v);
	std::cout << str << std::endl;

	cpprelude::i32 i32_v = -73;
	cpprelude::write(str, i32_v);
	std::cout << str << std::endl;

	cpprelude::u32 u32_v = 74;
	cpprelude::write(str, u32_v);
	std::cout << str << std::endl;

	cpprelude::i64 i64_v = -75;
	cpprelude::write(str, i64_v);
	std::cout << str << std::endl;

	cpprelude::u64 u64_v = 76;
	cpprelude::write(str, u64_v);
	std::cout << str << std::endl;

	cpprelude::r32 r32_v = -77.123456;
	cpprelude::write(str, r32_v);
	std::cout << str << std::endl;

	cpprelude::r64 r64_v = 78.123456789012;
	cpprelude::write(str, r64_v);
	std::cout << str << std::endl;

	cpprelude::show(str, str.data());
	std::cout << str << std::endl;

	std::cout << cpprelude::write(str, 1, " ", 2.0f, " ", 3.0, " ", 4u) << std::endl;

	std::cout << str << std::endl;
	
	auto str_literal = "-123456587646457687"_l;
	i32 num;
	auto res = cpprelude::read(str_literal, num);
	std::cout << res << std::endl;
	std::cout << num << std::endl;	
}

void
printt(int x)
{
	std::cout << "int: " << x << std::endl;
}

void
printt(float x)
{
	std::cout << "float: " << x << std::endl;
}

template<typename T>
void
printt(T&& x)
{
	std::cout << "unknown: " << typeid(T).name()  << std::endl;
}

template<typename T, typename ... ArgsT>
int
printt(int x, T&& t, ArgsT&&... args)
{
	printt(cpprelude::tmp::forward<T>(t));
	std::cout << "--" << std::endl;
	printt(x, args...);
	return x;
}

int
main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	benchmark();
	shuffle_test();
	quick_select_test();
	scratch();
	//std::cout << printt(-1, 1, 2.0f, 3.0, "koko") << std::endl;
	//test_string_conversion();
	return 0;
}
