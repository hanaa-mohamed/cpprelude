#include "cpprelude/memory.h"
#ifdef OS_WINDOWS
#include <Windows.h>
#endif // OS_WINDOWS

#include <cstdlib>

namespace cpprelude
{
	//mem_block 
	mem_block::mem_block(void* ptr_, usize size_)
		:ptr(ptr_), size(size_)
	{}

	mem_block::mem_block()
		:ptr(nullptr), size(0)
	{}

	mem_block::~mem_block()
	{
		ptr = nullptr;
		size = 0;
	}

	bool
	mem_block::operator==(const mem_block& other) const
	{
		return ptr == other.ptr && size == other.size;
	}

	bool
	mem_block::operator!=(const mem_block& other) const
	{
		return !operator==(other);
	}

	//owner_mem_block
	owner_mem_block::owner_mem_block(void* ptr_, usize size_)
		:ptr(ptr_), size(size_)
	{}

	owner_mem_block::owner_mem_block()
		:ptr(nullptr), size(0)
	{}

	owner_mem_block::owner_mem_block(owner_mem_block&& other)
		:ptr(other.ptr), size(other.size)
	{
		other.ptr = nullptr;
		other.size = 0;
	}

	owner_mem_block::~owner_mem_block()
	{
		if(ptr != nullptr && size > 0)
			free(*this);
	}

	owner_mem_block&
	owner_mem_block::operator=(owner_mem_block&& other)
	{
		if(ptr != nullptr && size > 0)
			free(*this);

		ptr = other.ptr;
		size = other.size;
		other.ptr = nullptr;
		other.size = 0;
		return *this;
	}

	bool
	owner_mem_block::operator==(const owner_mem_block& other) const
	{
		return ptr == other.ptr && size == other.size;
	}

	bool
	owner_mem_block::operator!=(const owner_mem_block& other) const
	{
		return !operator==(other);
	}

	mem_block
	owner_mem_block::sub_block(usize offset, usize size_) const
	{
		if(offset + size_ <= size)
			return mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size_);

		return mem_block();
	}

	mem_block
	owner_mem_block::sub_block(usize offset) const
	{
		if(offset < size)
			return mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size-offset);

		return mem_block();
	}

	mem_block
	owner_mem_block::release()
	{
		auto result = sub_block(0);
		ptr = nullptr;
		size = 0;
		return result;
	}

	void
	swap(owner_mem_block& a, owner_mem_block& b)
	{
		owner_mem_block temp(tmp::move(b));
		b = tmp::move(a);
		a = tmp::move(temp);
	}

	//mem api
	owner_mem_block
	virtual_alloc(usize size)
	{
		if (size == 0)
			return owner_mem_block();

#if defined(OS_WINDOWS)
		void* ptr = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#elif defined(OS_LINUX)
		void* ptr = std::malloc(size);
#endif

		return owner_mem_block(ptr, ptr ? size : 0);
	}

	void
	virtual_free(owner_mem_block& block)
	{
#if defined(OS_WINDOWS)
		VirtualFree(block.ptr, block.size, MEM_RELEASE);

		block.ptr = nullptr;
		block.size = 0;
#elif defined(OS_LINUX)
		free(block);
#endif
	}

	void
	virtual_free(owner_mem_block&& block)
	{
#if defined(OS_WINDOWS)
		VirtualFree(block.ptr, block.size, MEM_RELEASE);

		block.ptr = nullptr;
		block.size = 0;
#elif defined(OS_LINUX)
		free(block);
#endif
	}

	owner_mem_block
	alloc(usize size, ubyte alignment)
	{
		if (size == 0)
			return owner_mem_block();
		void* ptr = std::malloc(size);
		return owner_mem_block(ptr, ptr ? size : 0);
	}

	void
	realloc(owner_mem_block& block, usize size)
	{
		if(size == 0)
		{
			free(block);
			return;
		}

		block.ptr = std::realloc(block.ptr, size);
		block.size = size;
	}

	void
	free(owner_mem_block& block)
	{
		if(block.ptr != nullptr)	
			std::free(block.ptr);

		block.ptr = nullptr;
		block.size = 0;
	}

	void
	free(owner_mem_block&& block)
	{
		if(block.ptr != nullptr)	
			std::free(block.ptr);

		block.ptr = nullptr;
		block.size = 0;
	}
}
