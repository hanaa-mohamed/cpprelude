#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

namespace cpprelude
{
	struct API weak_mem_block
	{
		void* ptr;
		usize size;

		weak_mem_block(void* ptr, usize size);
		weak_mem_block();
		~weak_mem_block();

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
			return reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset);
		}

		template<typename T>
		const T*
		as(usize offset = 0) const
		{
			return reinterpret_cast<const T*>(reinterpret_cast<ubyte*>(ptr)+offset);
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
		sub_block(usize offset, usize size) const;

		weak_mem_block
		sub_block(usize offset = 0) const;

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
			return reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset);
		}

		template<typename T>
		const T*
		as(usize offset = 0) const
		{
			return reinterpret_cast<const T*>(reinterpret_cast<ubyte*>(ptr)+offset);
		}
	};

	template<typename T>
	struct handle
	{
		owner_mem_block memory_block;

		template<typename ... ArgT>
		void
		construct(owner_mem_block&& block, ArgT&& ... args)
		{
			memory_block = tmp::move(block);
			new (memory_block.ptr) T(args...);
		}

		void
		destroy()
		{
			memory_block.template as<T>()->~T();
		}

		T*
		operator->()
		{
			return memory_block.template as<T>();
		}

		const T*
		operator->() const
		{
			return memory_block.template as<const T>();
		}

		T&
		operator*()
		{
			return *memory_block.template as<T>();
		}

		const T&
		operator*() const
		{
			return *memory_block.template as<T>();
		}

		template<typename R>
		R*
		as()
		{
			return memory_block.template as<R>();
		}

		template<typename R>
		const R*
		as() const
		{
			return memory_block.template as<const R>();
		}

		operator
		owner_mem_block&()
		{
			return &memory_block;
		}

		operator
		bool()
		{
			return valid();
		}

		bool
		valid() const
		{
			return memory_block.ptr != nullptr && memory_block.size > 0;
		}
	};


	API owner_mem_block
	virtual_alloc(usize size);

	API void
	virtual_free(owner_mem_block& block);

	API owner_mem_block
	alloc(usize size, ubyte alignment = 4);

	API void
	realloc(owner_mem_block& block, usize size);

	API void
	free(owner_mem_block& block);

	API void
	swap(owner_mem_block& a, owner_mem_block& b);
}
