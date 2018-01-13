#include "benchmark.h"
#include <stdio.h>
#include "cpprelude/hash_array.h"
#include <unordered_map>

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
