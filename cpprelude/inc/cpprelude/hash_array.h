#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"
#include "cpprelude/allocator.h"
#include "cpprelude/string.h"
#include "cpprelude/memory.h"
#include "cpprelude/iterator.h"

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
			operator()(TYPE value) const\
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

	API usize
	hash_bytes(const void* ptr, usize len, usize seed = 0xc70f6907UL);

	template<typename T>
	struct hash<string_slice<T>>
	{
		inline usize
		operator()(const string_slice<T>& str) const
		{
			return hash_bytes(str.data(), str.count() * sizeof(T));
		}
	};

	template<typename T>
	struct hash<slice<T>>
	{
		inline usize
		operator()(const slice<T>& data) const
		{
			return hash_bytes(data.ptr, data.size);
		}
	};

	template<>
	struct hash<r32>
	{
		inline usize
		operator()(r32 value) const
		{
			return value != 0.0f ? hash_bytes(&value, sizeof(r32)) : 0;
		}
	};

	template<>
	struct hash<r64>
	{
		inline usize
		operator()(r64 value) const
		{
			return value != 0.0 ? hash_bytes(&value, sizeof(r64)) : 0;
		}
	};

	template<typename keyType,
			 typename valueType,
			 typename hashType = hash<keyType>,
			 typename AllocatorT = global_allocator>
	struct hash_array
	{
		using key_type = keyType;
		using value_type = valueType;
		using hash_type = hashType;
		using iterator = hash_array_iterator<keyType, valueType>;
		using const_iterator = const_hash_array_iterator<keyType, valueType>;

		dynamic_array<key_type, AllocatorT> _keys;
		dynamic_array<value_type, AllocatorT> _values;
		dynamic_array<u8, AllocatorT> _flags;
		hash_type _hasher;
		usize _count;

		hash_array(const AllocatorT& allocator = AllocatorT())
			:_keys(allocator), _values(allocator), _flags(allocator), _count(0)
		{
			constexpr usize starting_count = 128;
			
			_keys.expand_back(starting_count);
			_values.expand_back(starting_count);
			_flags.expand_back(starting_count, 0);
		}

		bool
		insert(const key_type& key, const value_type& value)
		{
			if(_count > capacity() / 2)
				reserve(capacity() * 2);

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return false;
			
			_keys[index] = key;
			_values[index] = value;

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return true;
		}

		bool
		insert(key_type&& key, const value_type& value)
		{
			if(_count > capacity() / 2)
				reserve(capacity() * 2);

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return false;
			
			_keys[index] = tmp::move(key);
			_values[index] = value;

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return true;
		}

		bool
		insert(const key_type& key, value_type&& value)
		{
			if(_count > capacity() / 2)
				reserve(capacity() * 2);

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return false;
			
			_keys[index] = key;
			_values[index] = tmp::move(value);

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return true;
		}

		bool
		insert(key_type&& key, value_type&& value)
		{
			if(_count > capacity() / 2)
				reserve(capacity() * 2);

			auto index = _find_position(key);

			//we couldn't find a position
			if(index == capacity())
				return false;
			
			_keys[index] = tmp::move(key);
			_values[index] = tmp::move(value);

			//if this cell is empty mark it full and increment _count
			if(_flags[index] == 0)
			{
				_flags[index] = 1;
				++_count;
			}

			return true;
		}

		value_type&
		operator[](const key_type& key)
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = key;
				
				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		const value_type&
		operator[](const key_type& key) const
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = key;
				
				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		value_type&
		operator[](key_type&& key)
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = tmp::move(key);
				
				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		const value_type&
		operator[](key_type&& key) const
		{
			auto index = _find_position(key);

			//if not found then create and init one
			if(_flags[index] == 0)
			{
				_keys[index] = tmp::move(key);
				
				_flags[index] = 1;
				++_count;
			}

			return _values[index];
		}

		bool
		remove(const key_type& key)
		{
			auto index = _find_position(key);

			//if not found then don't remove
			if(_flags[index] == 0)
				return false;

			_flags[index] = 0;
			_keys[index].~key_type();
			_values[index].~value_type();
			--_count;
			return true;
		}

		

		bool
		empty() const
		{
			return _count == 0;
		}

		usize
		capacity() const
		{
			return _keys.count();
		}

		void
		reserve(usize new_count)
		{
			_keys.expand_back(new_count);
			_values.expand_back(new_count);
			_flags.expand_back(new_count, 0);
		}

		void
		clear()
		{
			auto flag_it = _flags.begin();
			auto key_it = _keys.begin();
			auto value_it = _values.begin();

			usize count = _flags.count();
			while(count--)
			{
				if(*flag_it != 0)
				{
					*flag_it = 0;
					key_it->~key_type();
					value_it->~value_type();
				}

				flag_it = next(flag_it);
				key_it = next(key_it);
				value_it = next(value_it);
			}

			_count = 0;
		}

		iterator
		begin()
		{
			iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());
			if(*result._flag_it == 0)
				++result;
			
			return result;
		}

		const_iterator
		begin() const
		{
			const_iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());

			if(*result._flag_it == 0)
				++result;

			return result;
		}

		const_iterator
		cbegin() const
		{
			const_iterator result(_keys.cbegin(), _values.begin(), _flags.begin(), capacity());

			if(*result._flag_it == 0)
				++result;

			return result;
		}

		iterator
		end()
		{
			return iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}

		const_iterator
		end() const
		{
			return const_iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}

		const_iterator
		cend() const
		{
			return const_iterator(_keys.cend(), _values.end(), _flags.end(), 0);
		}
		
		usize
		_find_position(const key_type& key) const
		{
			usize hash_value = _hasher(key);
			usize cap = capacity();
			usize index = hash_value % cap;
			usize it = index;

			//linear probing goes here
			while(true)
			{
				//this is an empty spot then there's no data here we return it
				if(_flags[it] == 0)
					return it;

				//this means that the position is occupied
				//check if it's the same key then we return it also
				if(_keys[it] == key)
					return it;

				//this means it position is not empty nor does it has our key
				++it;
				it %= cap;

				//if we went a full circle then we just return cap to signal
				//that we didn't find anything
				if(it == index)
					return cap;
			}

			return cap;
		}

	};
}
