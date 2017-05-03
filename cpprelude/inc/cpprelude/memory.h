#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	struct API weak_mem_block
	{
		void* ptr;
		usize size;

		weak_mem_block(void* ptr, usize size);
		weak_mem_block();

		bool
		operator==(const weak_mem_block& other) const;

		bool
		operator!=(const weak_mem_block& other) const;

		template<typename T>
		T*
		at(usize index)
		{
			return as<T>(index*sizeof(T));
		}

		template<typename T>
		const T*
		at(usize index) const
		{
			return as<T>(index*sizeof(T));
		}

		template<typename T>
		T*
		as(usize offset = 0)
		{
			if(ptr != nullptr && size > 0 && offset + sizeof(T) <= size)
				return reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset);

			return nullptr;
		}

		template<typename T>
		const T*
		as(usize offset = 0) const
		{
			if(ptr != nullptr && size > 0 && offset + sizeof(T) <= size)
				return reinterpret_cast<const T*>(reinterpret_cast<ubyte*>(ptr)+offset);

			return nullptr;
		}
	};

	struct API owner_mem_block
	{
		void* ptr;
		usize size;

		owner_mem_block(void* ptr, usize size);

		owner_mem_block();

		owner_mem_block(const owner_mem_block&) = delete;

		owner_mem_block(owner_mem_block&& other);

		~owner_mem_block();

		owner_mem_block&
		operator=(const owner_mem_block&) = delete;

		owner_mem_block&
		operator=(owner_mem_block&& other);

		bool
		operator==(const owner_mem_block& other) const;

		bool
		operator!=(const owner_mem_block& other) const;

		weak_mem_block
		sub_block(usize offset, usize size);

		weak_mem_block
		sub_block(usize offset = 0);

		weak_mem_block
		release();

		template<typename T>
		T*
		at(usize index)
		{
			return as<T>(index*sizeof(T));
		}

		template<typename T>
		const T*
		at(usize index) const
		{
			return as<T>(index*sizeof(T));
		}

		template<typename T>
		T*
		as(usize offset = 0)
		{
			if(ptr != nullptr && size > 0 && offset + sizeof(T) <= size)
				return reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset);

			return nullptr;
		}

		template<typename T>
		const T*
		as(usize offset = 0) const
		{
			if(ptr != nullptr && size > 0 && offset + sizeof(T) <= size)
				return reinterpret_cast<const T*>(reinterpret_cast<ubyte*>(ptr)+offset);

			return nullptr;
		}
	};

	API owner_mem_block
	alloc(usize size, ubyte alignment = 4);

	API void
	free(owner_mem_block& block);
}