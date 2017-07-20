#include "catch.hpp"
#include <cpprelude/string.h>

using namespace cpprelude;

TEST_CASE("string test", "[string]")
{
	string str;

	SECTION("Case 01")
	{
		string new_str;
		CHECK(str == new_str);

		str = new_str;
		CHECK(str == new_str);

		CHECK(str.empty() == true);
		CHECK(str.count() == 0);
	}

	SECTION("Case 02")
	{
		literal a = "koko"_l;
		literal b = "wawa"_l;
		literal c = "koko"_l;

		CHECK(a.count() == 4);
		CHECK(b.count() == 4);
		CHECK(c.count() == 4);

		CHECK(a.empty() == false);
		CHECK(b.empty() == false);
		CHECK(c.empty() == false);

		CHECK(a != b);
		CHECK(b != c);
		CHECK(a == c);

		c.reset();
		CHECK(c.empty() == true);
		CHECK(c.count() == 0);
	}

	SECTION("Case 03")
	{
		literal a = ""_l;
		literal b = ""_l;

		CHECK(a==b);
		
		CHECK(a.count() == 0);
		CHECK(b.count() == 0);

		CHECK(a.empty() == true);
		CHECK(b.empty() == true);

		CHECK(a.begin() == a.end());
	}

	SECTION("Case 04")
	{
		literal a = "abc"_l;

		CHECK(a.empty() == false);
		CHECK(a.count() == 3);

		CHECK(*a.data() == 'a');
		CHECK(*a.front() == 'a');
		CHECK(*a.begin() == 'a');
		CHECK(*a.back() == 'c');
		CHECK(*a.end() == '\0');

		CHECK(a[0] == 'a');
		CHECK(a[1] == 'b');
		CHECK(a[2] == 'c');
	}

	SECTION("Case 05")
	{
		auto str = string::from_cstring("abc");
		
		for(auto& ch: str)
			CHECK(ch != '\0');
		
		CHECK(str.count() == 3);
		CHECK(str.empty() == false);

		string::dispose(str);
		CHECK(str.count() == 0);
		CHECK(str.empty() == true);

		for(auto& ch: str)
			CHECK(false);
	}

	SECTION("Case 06")
	{
		auto a = string::from_cstring("abc");
		auto b = string::from_cstring("abc");

		CHECK(a.count() == 3);
		CHECK(b.count() == 3);

		CHECK(a.empty() == false);
		CHECK(b.empty() == false);

		CHECK(a == b);

		b[1] = 'd';

		CHECK(a != b);

		auto c = a;

		CHECK(c != b);
		CHECK(c == a);

		string::dispose(c);
		string::dispose(b);
	}

	SECTION("Case 07")
	{
		auto a = string::from_cstring("abc");
		auto b = a;
		CHECK(a == b);

		auto c = string::copy(a);

		CHECK(c == a);
		
		string::dispose(a);

		CHECK(c != a);

		CHECK(c.count() == 3);
		CHECK(c.empty() == false);
		for(auto& ch: c)
			CHECK(ch != '\0');

		string::dispose(c);
	}

	SECTION("Case 08")
	{
		auto a = string::from_cstring("koko");
		auto b = a.view(2);

		CHECK(a != b);
		CHECK(b.count() == 2);
		CHECK(b.empty() == false);

		b = a.view(0);
		CHECK(a == b);
		CHECK(b.count() == 4);
		CHECK(b.empty() == false);

		b = a.view(4);
		CHECK(a!=b);
		CHECK(b.count() == 0);
		CHECK(b.empty() == true);

		b = a.view(0, 2);
		CHECK(a != b);
		CHECK(b.count() == 2);
		CHECK(b.empty() == false);

		for(auto& ch: b)
			CHECK(ch != '\0');

		string::dispose(a);
	}

	SECTION("Case 09")
	{
		string_writer<global_allocator> writer;
		
		CHECK(writer.empty());
		writer.clear();
		CHECK(writer.empty());

		CHECK(writer.output() == string());

		writer.write_back("aaaa"_l);
		auto str = writer.output();
		for(auto& ch: str)
			CHECK(ch == 'a');

		CHECK(writer.empty());
		writer.write_front("aaaa"_l);
		CHECK(writer.empty() == false);
		writer.clear();
		CHECK(writer.empty());
		string::dispose(str);
	}

	SECTION("Case 10")
	{
		local_string<256> str, str2;
		CHECK(str.empty() == false);
		CHECK(str.count() == 255);

		write(str, 1111);
		for(auto& ch: str.view(0,4))
			CHECK(ch == '1');
		u16 num;
		CHECK(read(str, num) == true);
		CHECK(num == 1111);

		str2 = "1111";
		CHECK(str.view(0, 4) == str2.view(0, 4));
	}

	SECTION("Case 11")
	{
		local_string<64> str;
		write(str, 1.123f);
		auto str2 = "1.123"_l;
		CHECK(str.view(0, 5) == str2);
		r32 f;
		CHECK(read(str, f) == true);
		CHECK(f == 1.123f);
	}
}
