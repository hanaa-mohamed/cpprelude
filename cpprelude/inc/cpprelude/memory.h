#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

namespace cpprelude
{
	struct API mem_block
	{
		void* ptr;
		usize size;

		mem_block(void* ptr, usize size);
	    mem_block();

		bool
		operator==(const mem_block& other) const;

		bool
		operator!=(const mem_block& other) const;

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

		template<typename T>
		T*
		decay()
		{
			auto result_ptr = reinterpret_cast<T*>(ptr);
			ptr = nullptr;
			size = 0;
			return result_ptr;
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

		mem_block
		sub_block(usize offset, usize size) const;

		mem_block
		sub_block(usize offset = 0) const;

		mem_block
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

		template<typename T>
		T*
		decay()
		{
			auto result_ptr = reinterpret_cast<T*>(ptr);
			ptr = nullptr;
			size = 0;
			return result_ptr;
		}
	};

	template<typename T>
	struct handle
	{
		T* value_ptr;

		handle()
			:value_ptr(nullptr)
		{}

		handle(T* ptr)
			:value_ptr(ptr)
		{}

		handle(const handle&) = default;

		handle(handle&& other)
			:value_ptr(other.value_ptr)
		{ other.value_ptr = nullptr; }

		handle&
		operator=(const handle&) = default;

		handle&
		operator=(handle&& other)
		{
			value_ptr = other.value_ptr;
			other.value_ptr = nullptr;

			return *this;
		}
		
		template<typename ... ArgT>
		void
		construct(T* ptr, ArgT&& ... args)
		{
			value_ptr = ptr;
			new (value_ptr) T(tmp::forward<ArgT>(args)...);
		}

		template<typename ... ArgT>
		void
		construct(owner_mem_block&& block, ArgT&& ... args)
		{
			value_ptr = block.template decay<T>();
			new (value_ptr) T(tmp::forward<ArgT>(args)...);
		}

		void
		destroy()
		{
			value_ptr->~T();
		}

		T*
		operator->()
		{
			return value_ptr;
		}

		const T*
		operator->() const
		{
			return value_ptr;
		}

		T&
		operator*()
		{
			return *value_ptr;
		}

		const T&
		operator*() const
		{
			return *value_ptr;
		}

		operator
		bool()
		{
			return valid();
		}

		bool
		valid() const
		{
			return value_ptr != nullptr;
		}

		bool
		operator==(const handle& other) const
		{
			return value_ptr == other.value_ptr;
		}

		bool
		operator!=(const handle& other) const
		{
			return ~operator==(other);
		}

		template<typename R = T>
		R*
		decay()
		{
			return reinterpret_cast<R*>(value_ptr);
		}
	};


	API owner_mem_block
	virtual_alloc(usize size);

	API void
	virtual_free(owner_mem_block& block);

	API void
	virtual_free(owner_mem_block&& block);

	API owner_mem_block
	alloc(usize size, ubyte alignment = 4);

	API void
	realloc(owner_mem_block& block, usize size);

	API void
	free(owner_mem_block& block);

	API void
	free(owner_mem_block&& block);

	API void
	swap(owner_mem_block& a, owner_mem_block& b);

	template<typename T>
	owner_mem_block
	resurrect(T* ptr, usize size = sizeof(T))
	{
		return owner_mem_block(ptr, size);
	}

	template<typename T>
	owner_mem_block
	resurrect(const handle<T>& handle_, usize size = sizeof(T))
	{
		return owner_mem_block(handle_.value_ptr, size);
	}
}
