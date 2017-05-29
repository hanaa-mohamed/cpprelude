#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	struct API linear_allocator
	{
		mem_block _memory;
		usize _alloc_head, _alloc_count;

		explicit linear_allocator(const mem_block& memory);
		explicit linear_allocator(mem_block&& memory);
		~linear_allocator();

		owner_mem_block
		alloc(usize size, ubyte alignment = 4);

		void
		realloc(owner_mem_block& block, usize size);

		void
		free(owner_mem_block& block);

		void
		free(owner_mem_block&& block);
	};

	struct API global_allocator
	{
		owner_mem_block
		alloc(usize size, ubyte alignment = 4);

		void
		realloc(owner_mem_block& block, usize size);

		void
		free(owner_mem_block& block);

		void
		free(owner_mem_block&& block);
	};

	API linear_allocator
	make_arena_allocator(usize arena_size, owner_mem_block& mem_block);

	API linear_allocator
	make_stack_allocator(ubyte* ptr, usize size);

}
