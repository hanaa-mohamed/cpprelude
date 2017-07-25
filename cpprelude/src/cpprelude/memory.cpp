#include "cpprelude/memory.h"

#include <algorithm>
#include <iostream>

#ifdef OS_WINDOWS
#include <Windows.h>
#endif // OS_WINDOWS

namespace cpprelude
{
	// constexpr usize starting_size = KILOBYTES(64);
	// constexpr usize grow_factor = 2;
	//
	// enum class memory_block_state
	// {
	// 	free,
	// 	occupied,
	// 	sentinel
	// };
	//
	// struct memory_block
	// {
	// 	memory_block *prev, *next;
	// 	slice<ubyte> memory;
	// 	memory_block_state state;
	//
	// 	inline bool
	// 	can_fit(usize size) const
	// 	{
	// 		return (memory.size - sizeof(memory_block)) >= size;
	// 	}
	// };
	//
	// struct gpa_memory
	// {
	// 	memory_block sentinel;
	// 	usize total_size;
	// 	usize allocation_counter;
	//
	// 	gpa_memory()
	// 	{
	// 		sentinel.prev = &sentinel;
	// 		sentinel.next = &sentinel;
	// 		sentinel.state = memory_block_state::sentinel;
	// 		total_size = 0;
	// 		allocation_counter = 0;
	// 	}
	//
	// 	usize
	// 	next_block_size() const
	// 	{
	// 		if(total_size == 0)
	// 			return starting_size;
	// 		else
	// 			return total_size * grow_factor;
	// 	}
	//
	// };
	//
	// static gpa_memory global_memory;
	//
	// static inline memory_block*
	// push_memory_block(gpa_memory& memory, slice<ubyte>& memory_slice)
	// {
	// 	memory_block* header_block = memory_slice.convert<memory_block>();
	//
	// 	header_block->memory = memory_slice;
	// 	header_block->state = memory_block_state::free;
	// 	header_block->next = &memory.sentinel;
	// 	header_block->prev = memory.sentinel.prev;
	// 	memory.sentinel.prev = header_block;
	//
	// 	if(memory.sentinel.next == &memory.sentinel)
	// 		memory.sentinel.next = header_block;
	//
	// 	return header_block;
	// }
	//
	// static inline memory_block*
	// get_more_memory(gpa_memory& memory, usize recommended_size)
	// {
	// 	auto memory_slice = virtual_alloc<ubyte>(std::max(recommended_size, memory.next_block_size()) + sizeof(memory_block));
	//
	// 	return push_memory_block(memory, memory_slice);
	// }
	//
	// slice<ubyte>
	// gpa_alloc(usize count, ubyte alignment)
	// {
	// 	std::cout << sizeof(memory_block) << std::endl;
	//
	// 	auto it = global_memory.sentinel.next;
	// 	while(it != &global_memory.sentinel)
	// 	{
	// 		if(it->can_fit(count))
	// 		{
	// 			std::cout << "ALLOCATION_HERE" << std::endl;
	// 			return slice<ubyte>();
	// 		}
	// 		it = it->next;
	// 	}
	//
	// 	//allocation didn't happen so we need more memory
	// 	auto block = get_more_memory(global_memory, count);
	// 	std::cout << "we got block" << std::endl;
	// 	/*
	// 	 * loop through empty memory blocks
	// 	 * accomodate count
	// 	 * if no memory available then ask for more memory
	// 	 */
	// 	return slice<ubyte>();
	// }
}
