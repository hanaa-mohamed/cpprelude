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
		using const_iterator = const forward_iterator<T>;
		using data_type = T;
		using node_type = details::single_node<T>;

		handle<node_type> _head;
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
			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				it->construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
				++_count;
			}
		}

		slinked_list(const slinked_list<T>& other, const AllocatorT& allocator)
			:_count(0), _allocator(allocator)
		{
			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				it->construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
				++_count;
			}
		}

		slinked_list(slinked_list<T>&& other)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _allocator(tmp::move(other._allocator))
		{
			other._count = 0;
			other._head.value_ptr = nullptr;
		}

		slinked_list(slinked_list<T>&& other, const AllocatorT& allocator)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _allocator(allocator)
		{
			other._count = 0;
			other._head.value_ptr = nullptr;
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
			
			auto* other_it = &other._head;
			auto* it = &_head;

			for(usize i = 0;
				i < other._count;
				++i)
			{
				it->construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

				//copy the data
				new (&(*it)->data) T((*other_it)->data);

				it = &(*it)->next;
				other_it = &(*other_it)->next;
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
			other._head.value_ptr = nullptr;

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
			auto it = _head;
			while(index--)
			{
				it = it->next;
			}
			return it->data;
		}

		T
		operator[](usize index) const
		{
			auto it = _head;
			while(index--)
			{
				it = it->next;
			}
			return it->data;
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
			handle<node_type> new_node;
			new_node.construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

			//copy the data value
			new (&new_node->data) T(value);

			//move the current head as the next to the new node
			new_node->next = tmp::move(_head);

			//set the new node as the new head
			_head = new_node;

			++_count;
		}

		void
		insert_front(T&& value)
		{
			handle<node_type> new_node;
			new_node.construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

			//copy the data value
			new (&new_node->data) T(value);

			//move the current head as the next to the new node
			new_node->next = tmp::move(_head);

			//set the new node as the new head
			_head = new_node;

			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count == 0)
				return;

			auto it = _head;
			for(usize i = 0;
				i < removal_count;
				++i)
			{
				//move the next node to the tmp
				auto next_node = tmp::move(it->next);

				//destroy the value
				it.destroy();

				//free the memory
				_allocator.free(resurrect<node_type>(it.value_ptr));

				it = next_node;
				--_count;

			}
			_head = it;
		}

		void
		reset()
		{
			auto it = _head;
			while(_count)
			{
				//move the next node to the tmp
				auto next_node = it->next;

				//destroy the value
				it.destroy();

				//free the memory
				_allocator.free(resurrect<node_type>(it.value_ptr));

				it = next_node;
				--_count;
			}
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return const_iterator(_head);
		}

		iterator
		front()
		{
			return iterator(_head);
		}

		const_iterator
		begin() const
		{
			return const_iterator(_head);
		}

		iterator
		begin()
		{
			return iterator(_head);
		}

		const_iterator
		end() const
		{
			return const_iterator();
		}

		iterator
		end()
		{
			return iterator();
		}

		owner_mem_block
		decay()
		{
			owner_mem_block result = resurrect(_head);
			_head.value_ptr = nullptr;
			_count = 0;
			return result;
		}

		owner_mem_block
		decay_continuous()
		{
			//allocate the memory of the supplied allocator
			owner_mem_block result = _allocator.alloc(_count*sizeof(T));

			//move the elements over
			usize i = 0;
			for(auto&& value: *this)
				new (result.template at<T>(i++)) T(tmp::move(value));

			//reset this linked list
			reset();

			//return the piece of memory
			return result;
		}
	};
}