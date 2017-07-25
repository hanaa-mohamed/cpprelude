#pragma once
#include <stdint.h>

//export definitions
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32__)||defined(_WIN64)||defined(WIN64)||defined(__MINGW32__)||defined(__MINGW64__)
    #define OS_WINDOWS
#elif defined(__linux__) || defined(__unix__)
    #define OS_LINUX
#endif

#ifdef OS_WINDOWS
#ifdef COMPILE_DYNAMIC_LIB
    #define API __declspec(dllexport)
#elif STATIC_LIB
	#define API
#else
	#define API __declspec(dllimport)
#endif
#endif

#ifdef OS_LINUX
    #define API
#endif

#define BYTES(amount) amount
#define KILOBYTES(amount) BYTES(amount) * 1024LL
#define MEGABYTES(amount) KILOBYTES(amount) * 1024LL
#define GIGABYTES(amount) MEGABYTES(amount) * 1024LL

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a

namespace cpprelude {
	//type definitions
	using ubyte = unsigned char;
	using byte = char;

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using r32 = float;
	using r64 = double;

	namespace details
	{
		template<u64 ptr_size>
		struct size_type
		{};

		template<>
		struct size_type<8>
		{
			using usize = u64;
			using isize = i64;
		};

		template<>
		struct size_type<4>
		{
			using usize = u32;
			using isize = i32;
		};
	}

	using usize = details::size_type<sizeof(void*)>::usize;
	using isize = details::size_type<sizeof(void*)>::isize;
}
