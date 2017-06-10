#include "cpprelude/allocator.h"

namespace cpprelude
{
	linear_allocator::linear_allocator()
	{}
	
	linear_allocator::linear_allocator(const mem_block& memory)
		:_memory(memory), _alloc_head(0), _alloc_count(0)
	{}

	linear_allocator::linear_allocator(mem_block&& memory)
		:_memory(tmp::move(memory)), _alloc_head(0), _alloc_count(0)
	{}

	linear_allocator::~linear_allocator()
	{
		_alloc_head = 0;
		_alloc_count = 0;
	}

	owner_mem_block
	linear_allocator::alloc(usize size, ubyte alignment)
	{
		if(size > 0 && _alloc_head + size <= _memory.size)
		{
			++_alloc_count;
			auto result = owner_mem_block(reinterpret_cast<ubyte*>(_memory.ptr)+_alloc_head, size);

			_alloc_head += size;

			return result;
		}

		return owner_mem_block();
	}

	void
	linear_allocator::realloc(owner_mem_block& block, usize size)
	{
		void* memory_end = reinterpret_cast<ubyte*>(_memory.ptr) + _memory.size;

		//if i own this block
		if(block.ptr >= _memory.ptr && block.ptr < memory_end)
		{
			//if the block is the latest block allocated then we can just expand it
			void* latest_block = reinterpret_cast<ubyte*>(_memory.ptr) + _alloc_head - block.size;

			//if this is the last allocated block then move the head only
			if(block.ptr == latest_block)
			{
				_alloc_head += size - block.size;
				block.size = size;
				return;
			}

			//else just allocate and copy data
			owner_mem_block new_block = linear_allocator::alloc(size);
			usize limit = block.size < size ? block.size : size;

			//copy the data from the old to the new
			for(usize i = 0; i < limit; ++i)
				*new_block.template at<ubyte>(i) = *block.template at<ubyte>(i);

			//now free the old
			linear_allocator::free(block);

			//move the new_block to the old block
			block = tmp::move(new_block);
		}

	}

	void
	linear_allocator::free(owner_mem_block& block)
	{
		void* memory_end = reinterpret_cast<ubyte*>(_memory.ptr) + _memory.size;

		if(block.ptr >= _memory.ptr && block.ptr < memory_end)
		{
			void* latest_block = reinterpret_cast<ubyte*>(_memory.ptr) + _alloc_head - block.size;

			//if this is the last allocated block then move the head back
			if(block.ptr == latest_block)
				_alloc_head -= block.size;

			block.ptr = nullptr;
			block.size = 0;
			--_alloc_count;

			//if no other allocations exist then free the whole stack
			if(_alloc_count == 0)
				_alloc_head = 0;
		}
	}

	void
	linear_allocator::free(owner_mem_block&& block)
	{
		void* memory_end = reinterpret_cast<ubyte*>(_memory.ptr) + _memory.size;

		if(block.ptr >= _memory.ptr && block.ptr < memory_end)
		{
			void* latest_block = reinterpret_cast<ubyte*>(_memory.ptr) + _alloc_head - block.size;

			//if this is the last allocated block then move the head back
			if(block.ptr == latest_block)
				_alloc_head -= block.size;

			block.ptr = nullptr;
			block.size = 0;
			--_alloc_count;

			//if no other allocations exist then free the whole stack
			if(_alloc_count == 0)
				_alloc_head = 0;
		}
	}

	owner_mem_block
	global_allocator::alloc(usize size, ubyte alignment)
	{
		return cpprelude::alloc(size, alignment);
	}

	void
	global_allocator::realloc(owner_mem_block& block, usize size)
	{
		return cpprelude::realloc(block, size);
	}

	void
	global_allocator::free(owner_mem_block& block)
	{
		cpprelude::free(block);
	}

	void
	global_allocator::free(owner_mem_block&& block)
	{
		cpprelude::free(block);
	}

	linear_allocator
	make_arena_allocator(usize arena_size, owner_mem_block& mem_block)
	{
		mem_block = virtual_alloc(arena_size);
		return linear_allocator(mem_block.sub_block());
	}

	linear_allocator
	make_stack_allocator(ubyte* ptr, usize size)
	{
		return linear_allocator(mem_block(ptr, size));
	}
}
