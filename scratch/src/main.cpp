#include "benchmark.h"
#include <stdio.h>
#include <cpprelude/hash_array.h>
#include <cpprelude/fmt.h>
#include <cpprelude/bufio.h>
#include <cpprelude/stream.h>
#include <unordered_map>
using namespace cpprelude;

// struct screamer {
// 	screamer()
// 	{
// 		printf("screamer()\n");
// 	}

// 	~screamer()
// 	{
// 		printf("~screamer()\n");
// 	}

// 	screamer(const screamer& other)
// 	{
// 		printf("copy screamer()\n");
// 	}

// 	screamer(screamer&& other)
// 	{
// 		printf("move screamer()\n");
// 	}

// 	screamer&
// 		operator=(const screamer& other)
// 	{
// 		printf("copy screamer::operator=()\n");
// 		return *this;
// 	}

// 	screamer&
// 		operator=(screamer&& other)
// 	{
// 		printf("move screamer::operator=()\n");
// 		return *this;
// 	}
// };

int
main(int argc, char** argv)
{
	auto stream = view_string_as_memory_stream("123 -3456 Mostafa Saad"_cs);
	buf_reader reader(stream);
	u8 se=0, se2=0;
	string first_name, last_name;
	auto d = vscans(reader, se, se2, first_name, last_name);

	d = scan(se, se2, first_name, last_name);
	println(se, ", ", se2, ", ", first_name, ", ", last_name);
	// using namespace cpprelude;
	// {
	// 	hash_array<usize, screamer> as;
	// 	as.reserve(10);
	// 	as[1];
	// 	as[1] = screamer();
	// }
	// {
	// 	std::unordered_map<int, screamer> as;
	// 	as.reserve(10);
	// 	as[1];
	// 	as[1] = screamer();
	// }
	do_benchmark();
	return 0;
}
