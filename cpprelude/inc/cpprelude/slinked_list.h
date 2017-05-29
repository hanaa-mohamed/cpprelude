#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/iterator.h"
#include "cpprelude/allocator.h"
#include <initializer_list>
#include <iterator>

namespace cpprelude
{
	
	/*
	 * data is layouted this way
	 * owner_mem_block = [ sizeof(owner_mem_block) | T ]
	 * this has two parts a next owner_mem_block which has the next element
	 * and a T which houses the data
	 */
	template<typename T, typename AllocatorT = global_allocator>
	struct slinked_list
	{
		using iterator = forward_iterator<T>;
		using const_iterator = forward_iterator<const T>;
		using data_type = T;

		owner_mem_block _head;
		usize _count;
		AllocatorT _allocator;

		slinked_list(const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{}

		slinked_list(std::initializer_list<T> list, const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		slinked_list(usize count, const T& fill_value, const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			for(usize i = 0; i < count; ++i)
				insert_front(fill_value);
		}

		slinked_list(const slinked_list<T>& other)
			:_count(0), _allocator(other._allocator)
		{
			//pointer to the other data
			auto other_it = &other._head;
			//have to pointers to iterate
			auto it = &_head;
			for(usize i = 0; i < other._count; ++i)
			{
				//allocate a node
				*it = _allocator.alloc(sizeof(owner_mem_block) + sizeof(T));
				//init the next field
				new (it->template as<owner_mem_block>()) owner_mem_block();
				//copy the content from the other list
				new (it->template as<T>(sizeof(owner_mem_block))) T(*other_it->template as<T>(sizeof(owner_mem_block)));

				//get the next node in both lists
				it = it->template as<owner_mem_block>();
				other_it = other_it->template as<owner_mem_block>();
				++_count;
			}
		}

		slinked_list(const slinked_list<T>& other, const AllocatorT& allocator)
			:_count(0), _allocator(allocator)
		{
			//pointer to the other data
			auto other_it = &other._head;
			//have to pointers to iterate
			auto it = &_head;
			for(usize i = 0; i < other._count; ++i)
			{
				//allocate a node
				*it = _allocator.alloc(sizeof(owner_mem_block) + sizeof(T));
				//init the next field
				new (it->template as<owner_mem_block>()) owner_mem_block();
				//copy the content from the other list
				new (it->template as<T>(sizeof(owner_mem_block))) T(*other_it->template as<T>(sizeof(owner_mem_block)));

				//get the next node in both lists
				it = it->template as<owner_mem_block>();
				other_it = other_it->template as<owner_mem_block>();
				++_count;
			}
		}

		slinked_list(slinked_list<T>&& other)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _allocator(tmp::move(other._allocator))
		{
			other._count = 0;
		}

		slinked_list(slinked_list<T>&& other, const AllocatorT& allocator)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _allocator(allocator)
		{
			other._count = 0;
		}

		~slinked_list()
		{
			reset();
		}

		slinked_list<T>&
		operator=(const slinked_list<T>& other)
		{
			reset();
			
			_allocator = other._allocator;
			//pointer to the other data
			auto other_it = &other._head;
			//have to pointers to iterate
			auto it = &_head;
			for(usize i = 0; i < other._count; ++i)
			{
				//allocate a node
				*it = _allocator.alloc(sizeof(owner_mem_block) + sizeof(T));
				//init the next field
				new (it->template as<owner_mem_block>()) owner_mem_block();
				//copy the content from the other list
				new (it->template as<T>(sizeof(owner_mem_block))) T(*other_it->template as<T>(sizeof(owner_mem_block)));

				//get the next node in both lists
				it = it->template as<owner_mem_block>();
				other_it = other_it->template as<owner_mem_block>();
				++_count;
			}

			return *this;
		}

		slinked_list<T>&
		operator=(slinked_list<T>&& other)
		{
			reset();
			_count = other._count;
			_head = tmp::move(other._head);
			_allocator = tmp::move(other._allocator);
			other._count = 0;

			return *this;
		}

		usize
		count() const
		{
			return _count;
		}

		void
		expand_front(usize additional_count, const T& fill_value)
		{
			auto new_count = _count + additional_count;
			for(usize i = _count; i < new_count; ++i)
				insert_front(fill_value);
		}

		void
		shrink_front(usize shrinkage_count)
		{
			remove_front(shrinkage_count);
		}

		T&
		operator[](usize index)
		{
			auto it = &_head;
			while(index--)
			{
				it = it->template as<owner_mem_block>();
			}

			return *(it->template as<T>(sizeof(owner_mem_block)));
		}

		T
		operator[](usize index) const
		{
			auto it = &_head;
			while(index--)
			{
				it = it->template as<owner_mem_block>();
			}

			return *(it->template as<T>(sizeof(owner_mem_block)));
		}

		void
		insert_front(std::initializer_list<T> list)
		{
			auto it = list.end();
			it = std::prev(it);
			for(usize i = 0; i < list.size(); ++i)
			{
				insert_front(*it);
				it = std::prev(it);
			}
		}

		void
		insert_front(const T& value)
		{
			auto new_memory_block = _allocator.alloc(sizeof(owner_mem_block) + sizeof(T));
			//move the current head to the next field in memory
			new (new_memory_block.template as<owner_mem_block>()) owner_mem_block(tmp::move(_head));
			//move the value to it's corresponding field
			new (new_memory_block.template as<T>(sizeof(owner_mem_block))) T(value);

			//now hold the new_memory_block with the now empty _head handle
			_head = tmp::move(new_memory_block);
			++_count;
		}

		void
		insert_front(T&& value)
		{
			auto new_memory_block = _allocator.alloc(sizeof(owner_mem_block) + sizeof(T));
			//move the current head to the next field in memory
			new (new_memory_block.template as<owner_mem_block>()) owner_mem_block(tmp::move(_head));
			//move the value to it's corresponding field
			new (new_memory_block.template as<T>(sizeof(owner_mem_block))) T(tmp::move(value));

			//now hold the new_memory_block with the now empty _head handle
			_head = tmp::move(new_memory_block);
			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			auto it = tmp::move(_head);
			for (usize i = 0; i < removal_count; ++i)
			{
				auto next_block = tmp::move(*it.template as<owner_mem_block>());

				it.template as<T>(sizeof(owner_mem_block))->~T();

				//free the current block
				_allocator.free(it);

				it = tmp::move(next_block);
				--_count;
			}
			_head = tmp::move(it);
		}

		void
		reset()
		{
			auto it = tmp::move(_head);
			while(_count)
			{
				auto next_block = tmp::move(*it.template as<owner_mem_block>());

				it.template as<T>(sizeof(owner_mem_block))->~T();

				//free the current block
				_allocator.free(it);
				
				it = tmp::move(next_block);
				--_count;
			}
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		forward_iterator<const T>
		front() const
		{
			return forward_iterator<const T>(_head.sub_block());
		}

		forward_iterator<T>
		front()
		{
			return forward_iterator<T>(_head.sub_block());
		}

		forward_iterator<const T>
		begin() const
		{
			return forward_iterator<const T>(_head.sub_block());
		}

		forward_iterator<T>
		begin()
		{
			return forward_iterator<T>(_head.sub_block());
		}

		forward_iterator<const T>
		end() const
		{
			return forward_iterator<const T>();
		}

		forward_iterator<T>
		end()
		{
			return forward_iterator<T>();
		}
	};
}