#include "cpprelude/memory.h"

#include <cstdlib>

namespace cpprelude
{
	//weak_mem_block 
	weak_mem_block::weak_mem_block(void* ptr_, usize size_)
		:ptr(ptr_), size(size_)
	{}

	weak_mem_block::weak_mem_block()
		:ptr(nullptr), size(0)
	{}

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
		owner_mem_block::~owner_mem_block();

		ptr = other.ptr;
		size = other.size;
		other.ptr = nullptr;
		other.size = 0;
		return *this;
	}

	weak_mem_block
	owner_mem_block::sub_block(usize offset, usize size_)
	{
		if(offset + size_ <= size)
			return weak_mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size_);

		return weak_mem_block();
	}

	weak_mem_block
	owner_mem_block::sub_block(usize offset)
	{
		if(offset < size)
			return weak_mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size-offset);

		return weak_mem_block();
	}

	//mem api
	owner_mem_block
	alloc(usize size, ubyte alignment)
	{
		return owner_mem_block(std::malloc(size), size);
	}

	void
	free(owner_mem_block& block)
	{
		std::free(block.ptr);
		block.ptr = nullptr;
		block.size = 0;
	}
}