#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/allocator.h"

namespace cpprelude
{
	namespace details
	{
		//MurmurHashUnaligned2
		template<usize ptr_size>
		struct _hash_bytes
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static_assert(ptr_size == 0, "there's no hash function defined for this architecture pointer size");
			}
		};

		//4 byte pointer size architectures
		template<>
		struct _hash_bytes<4>
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				const usize m = 0x5bd1e995;
				usize hash = seed ^ len;
				const byte* buffer = static_cast<const byte*>(ptr);

				while(len >= 4)
				{
					usize k = *reinterpret_cast<const usize*>(buffer);
					k *= m;
					k ^= k >> 24;
					k *= m;
					hash *= m;
					hash ^= k;
					buffer += 4;
					len -= 4;
				}

				if(len == 3)
				{
					hash ^= static_cast<ubyte>(buffer[2]) << 16;
					--len;
				}
				
				if(len == 2)
				{
					hash ^= static_cast<ubyte>(buffer[1]) << 8;
					--len;
				}
				
				if(len == 1)
				{
					hash ^= static_cast<ubyte>(buffer[0]);
					hash *= m;
					--len;
				}

				hash ^= hash >> 13;
				hash *= m;
				hash ^= hash >> 15;
				return hash;
			}
		};

		//8 byte pointer size architectures
		template<>
		struct _hash_bytes<8>
		{
			inline usize
			load_bytes(const byte* p, isize n) const
			{
				usize result = 0;
				--n;
				do
					result = (result << 8) + static_cast<ubyte>(p[n]);
				while(--n >= 0);
				return result;
			}

			inline usize
			shift_mix(usize v) const
			{
				return v ^ (v >> 47);
			}
			
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static const usize mul = (static_cast<usize>(0xc6a4a793UL) << 32UL) + static_cast<usize>(0x5bd1e995UL);

				const byte* const buffer = static_cast<const byte*>(ptr);

				const i32 len_aligned = len & ~0x7;
				const byte* const end = buffer + len_aligned;

				usize hash = seed ^ (len * mul);
				for(const byte* p = buffer;
					p != end;
					p += 8)
				{
					const usize data = shift_mix((*reinterpret_cast<const usize*>(p)) * mul) * mul;
					hash ^= data;
					hash *= mul;
				}

				if((len & 0x7) != 0)
				{
					const usize data = load_bytes(end, len & 0x7);
					hash ^= data;
					hash *= mul;
				}

				hash = shift_mix(hash) * mul;
				hash = shift_mix(hash);
				return hash;
			}
		};
	}

	template<typename T>
		struct hash
		{
			inline usize
			operator()(T value) const
			{
				static_assert(sizeof(T) == 0, "there's no hash function defined for this type");
			}
		};

		//pointer hash
		template<>
		struct hash<void*>
		{
			inline usize
			operator()(void* ptr) const
			{
				return reinterpret_cast<usize>(ptr);
			}
		};

#define trivial_hash(TYPE)\
		template<>\
		struct hash<TYPE>\
		{\
			inline usize\
			operator()(TYPE value)\
			{\
				return static_cast<usize>(value);\
			}\
		}

		trivial_hash(byte);

		trivial_hash(i8);
		trivial_hash(i16);
		trivial_hash(i32);
		trivial_hash(i64);

		trivial_hash(u8);
		trivial_hash(u16);
		trivial_hash(u32);
		trivial_hash(u64);

#undef trivial_hash

	API inline usize
	hash_bytes(const void* ptr, usize len, usize seed);
	
	template<typename key_type,
			 typename value_type,
			 typename hash_func_type = hash<key_type>,
			 typename AllocatorT = global_allocator>
	struct hash_array
	{
	};
}
