#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/tmp.h"

#include <cstdlib>
#include <iostream>

#ifdef OS_WINDOWS
#include <Windows.h>
#endif // OS_WINDOWS

namespace cpprelude
{
	template<typename T>
	struct slice
	{
		T* ptr;
		usize size;

		slice(T* ptr_, usize size_)
			:ptr(ptr_), size(size_)
		{}

		slice()
			:ptr(nullptr), size(0)
		{}

		slice(const slice& other) = default;

		slice&
		operator=(const slice& other) = default;

		slice(slice&& other)
			:ptr(other.ptr), size(other.size)
		{
			other.ptr = nullptr;
			other.size = 0;
		}

		slice&
		operator=(slice&& other)
		{
			ptr = other.ptr;
			size = other.size;
			other.ptr = nullptr;
			other.size = 0;
			return *this;
		}

		bool
		operator==(const slice& other) const
		{
			return ptr == other.ptr && size == other.size;
		}

		bool
		operator!=(const slice& other) const
		{
			return !operator==(other);
		}

		operator T*()
		{
			return ptr;
		}

		const T&
		operator[](const usize& index) const
		{
			return ptr[index];
		}

		T&
		operator[](const usize& index)
		{
			return ptr[index];
		}

		usize
		count() const
		{
			return size / sizeof(T);
		}

		bool
		valid() const
		{
			return ptr != nullptr && size > 0;
		}

		template<typename R>
		slice<R>
		convert() const
		{
			return slice<R>(reinterpret_cast<R*>(ptr), size);
		}

		slice<T>
		cut(usize start = 0, usize count = 0) const
		{
			if(count == 0)
				count = size / sizeof(T);

			return slice<T>(ptr+start, count * sizeof(T));
		}

		slice<T>
		cut_bytes(usize offset = 0, usize new_size = 0) const
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<T>(reinterpret_cast<T*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}

		template<typename R>
		slice<R>
		cut_bytes(usize offset = 0, usize new_size = 0) const
		{
			if(new_size == 0)
				new_size = size - offset;

			return slice<R>(reinterpret_cast<R*>(reinterpret_cast<ubyte*>(ptr)+offset), new_size);
		}
	};

	template<typename T>
	slice<T>
	make_slice(T* ptr, usize count = 1)
	{
		return slice<T>(ptr, count*sizeof(T));
	}

	template<typename T>
	slice<T>
	virtual_alloc(usize count = 1)
	{
		if(count == 0)
			return slice<T>();

	#if defined(OS_WINDOWS)
		T* ptr = reinterpret_cast<T*>(VirtualAlloc(NULL, count*sizeof(T), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE));
	#elif defined(OS_LINUX)
		T* ptr = reinterpret_cast<T*>(std::malloc(count * sizeof(T)));
	#endif

		return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	}

	template<typename T>
	void
	virtual_free(slice<T>& slice_)
	{
		virtual_free(tmp::move(slice_));
	}

	template<typename T>
	void
	virtual_free(slice<T>&& slice_)
	{
	#if defined(OS_WINDOWS)
		VirtualFree(slice_.ptr, slice_.size, MEM_RELEASE);
	#elif defined(OS_LINUX)
		std::free(slice_.ptr);
	#endif

		slice_.ptr = nullptr;
		slice_.size = 0;
	}

	template<typename T>
	slice<T>
	alloc(usize count = 1, ubyte alignment = 4)
	{
		if(count == 0)
			return slice<T>();
		T* ptr = reinterpret_cast<T*>(std::malloc(count * sizeof(T)));
		return slice<T>(ptr, ptr ? count * sizeof(T) : 0);
	}

	template<typename T>
	void
	free(slice<T>& slice_)
	{
		free(tmp::move(slice_));
	}

	template<typename T>
	void
	free(slice<T>&& slice_)
	{
		if (slice_.ptr != nullptr)
			std::free(slice_.ptr);

		slice_.ptr = nullptr;
		slice_.size = 0;
	}

	template<typename T>
	void
	realloc(slice<T>& slice_, usize count)
	{
		realloc(tmp::move(slice_), count);
	}

	template<typename T>
	void
	realloc(slice<T>&& slice_, usize count)
	{
		if(count == 0)
		{
			free(tmp::move(slice_));
			return;
		}

		slice_.ptr = reinterpret_cast<T*>(std::realloc(slice_.ptr, count*sizeof(T)));
		slice_.size = count * sizeof(T);
	}

	// API slice<ubyte>
	// gpa_alloc(usize count, ubyte alignment = 4);
}
