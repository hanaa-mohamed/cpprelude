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

	weak_mem_block::~weak_mem_block()
	{
		ptr = nullptr;
		size = 0;
	}

	bool
	weak_mem_block::operator==(const weak_mem_block& other) const
	{
		return ptr == other.ptr && size == other.size;
	}

	bool
	weak_mem_block::operator!=(const weak_mem_block& other) const
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

	weak_mem_block
	owner_mem_block::sub_block(usize offset, usize size_) const
	{
		if(offset + size_ <= size)
			return weak_mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size_);

		return weak_mem_block();
	}

	weak_mem_block
	owner_mem_block::sub_block(usize offset) const
	{
		if(offset < size)
			return weak_mem_block(reinterpret_cast<ubyte*>(ptr)+offset, size-offset);

		return weak_mem_block();
	}

	weak_mem_block
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
	alloc(usize size, ubyte alignment)
	{
		if(size == 0)
			return owner_mem_block();
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