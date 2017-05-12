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

	template<typename T>
	struct weak_handle
	{
		weak_mem_block memory_block;

		weak_handle(weak_mem_block block)
			:memory_block(tmp::move(block))
		{}

		operator weak_mem_block&()
		{
			return &memory_block;
		}

		T*
		operator->()
		{
			return memory_block.template as<T>();
		}

		const T*
		operator->() const
		{
			return memory_block.template as<T>();
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
	};

	template<typename T>
	struct handle
	{
		owner_mem_block memory_block;

		template<typename ... TArgs>
		handle(owner_mem_block&& block, TArgs&& ... args)
			:memory_block(tmp::move(block))
		{
			new (memory_block.template as<T>()) T(args...);
		}

		operator owner_mem_block&()
		{
			return &memory_block;
		}

		T*
		operator->()
		{
			return memory_block.template as<T>();
		}

		const T*
		operator->() const
		{
			return memory_block.template as<T>();
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

		bool
		valid() const
		{
			return memory_block.ptr != nullptr && memory_block.size > 0;
		}

		weak_handle<T>
		to_weak()
		{
			return weak_handle<T>(memory_block.sub_block());
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
	};

	API owner_mem_block
	alloc(usize size, ubyte alignment = 4);

	API void
	free(owner_mem_block& block);

	API void
	swap(owner_mem_block& a, owner_mem_block& b);

	template<typename T, typename ... TArgs>
	handle<T>
	alloc(TArgs&& ... args)
	{
		return handle<T>(alloc(sizeof(T)), args...);
	}

	template<typename T>
	void
	free(handle<T>& handle_)
	{
		handle_._memory_block = tmp::move(owner_mem_block());
	}
}