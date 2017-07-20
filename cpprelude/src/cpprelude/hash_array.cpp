#include "cpprelude/hash_array.h"

namespace cpprelude
{
	usize
	hash_bytes(const void* ptr, usize len, usize seed)
	{
		details::_hash_bytes<sizeof(void*)> hasher;
		return hasher(ptr, len, seed);
	}
}
