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
	 * owner_mem_block = [sizeof(owner_mem_block): "prev" | sizeof(owner_mem_block): "next" | T]
	 * this has three parts a previous owner_mem_block which has the previous element, a next owner_mem_block which has the next element and a T which houses the data
	 */

	template<typename T, typename AllocatorT = global_allocator>
	struct dlinked_list
	{
		using iterator = bidirectional_iterator<T>;
		using const_iterator = const bidirectional_iterator<T>;
		using data_type = T;
		using node_type = details::double_node<T>;

		handle<node_type> _head, _tail;
		usize _count;
		AllocatorT _allocator;

		dlinked_list(const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			_init_sentinels();
		}

		dlinked_list(std::initializer_list<T> list, const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			_init_sentinels();

			for(auto& value: list)
				insert_back(value);
		}

		dlinked_list(usize count, const T& fill_value, const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			_init_sentinels();

			for(usize i = 0; i < count; ++i)
				insert_back(fill_value);
		}

		dlinked_list(const dlinked_list<T>& other)
			:_count(0), _allocator(other._allocator)
		{
			_init_sentinels();

			for(const auto& other_value: other)
				insert_back(other_value);
		}

		dlinked_list(const dlinked_list<T>& other, const AllocatorT& allocator)
			:_count(0), _allocator(allocator)
		{
			_init_sentinels();

			for(auto& other_value: other)
				insert_back(other_value);
		}

		dlinked_list(dlinked_list<T>&& other)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _tail(tmp::move(other._tail)),
			 _allocator(other._allocator)
		{
			other._count = 0;
			other._head.value_ptr = nullptr;
			other._tail.value_ptr = nullptr;
		}

		dlinked_list(dlinked_list<T>&& other, const AllocatorT& allocator)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _tail(tmp::move(other._tail)),
			 _allocator(allocator)
		{
			other._count = 0;
			other._head.value_ptr = nullptr;
			other._tail.value_ptr = nullptr;
		}

		~dlinked_list()
		{
			reset();
			//remove the sentinals
			_allocator.free(_head);
			_allocator.free(_tail);
			_count = 0;
		}

		dlinked_list<T>&
		operator=(const dlinked_list<T>& other)
		{
			reset();
			_allocator = other._allocator;

			for(auto& other_value: other)
				insert_back(other_value);

			return *this;
		}

		dlinked_list<T>&
		operator=(dlinked_list<T>&& other)
		{
			reset();
			_allocator = other._allocator;

			_count = other._count;
			_head = tmp::move(other._head);
			_tail = tmp::move(other._tail);
			other._count = 0;
			other._head.value_ptr = nullptr;
			other._tail.value_ptr = nullptr;

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
		expand_back(usize additional_count, const T& fill_value)
		{
			auto new_count = _count + additional_count;
			for(usize i = _count; i < new_count; ++i)
				insert_back(fill_value);
		}

		void
		shrink_front(usize shrinkage_count)
		{
			remove_front(shrinkage_count);
		}

		void
		shrink_back(usize shrinkage_count)
		{
			remove_back(shrinkage_count);
		}

		T&
		operator[](usize index)
		{
			if(index >= _count * 0.5)
			{
				index = _count - index;
				auto it = _tail;
				while(index--)
					it = it->prev;

				return it->data;
			}
			else
			{
				auto it = _head->next;
				while(index--)
					it = it->next;

				return it->data;
			}
		}

		T
		operator[](usize index) const
		{
			if(index >= _count * 0.5)
			{
				index = _count - index;
				auto it = _tail;
				while(index--)
					it = it->prev;

				return it->data;
			}
			else
			{
				auto it = _head->next;
				while(index--)
					it = it->next;

				return it->data;
			}
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
			handle<node_type> new_node = _allocator.template alloc<node_type>();

			auto first_node = _head->next;

			first_node->prev = new_node;

			new_node->next = first_node;

			_head->next = new_node;

			new_node->prev = _head;

			new (&new_node->data) T(value);

			++_count;
		}

		void
		insert_front(T&& value)
		{
			handle<node_type> new_node = _allocator.template alloc<node_type>();

			auto first_node = _head->next;

			first_node->prev = new_node;
			new_node->next = first_node;

			_head->next = new_node;
			new_node->prev = _head;

			new (&new_node->data) T(tmp::move(value));

			++_count;
		}

		void
		insert_back(std::initializer_list<T> list)
		{
			for(const auto& value: list)
				insert_back(value);
		}

		void
		insert_back(const T& value)
		{
			handle<node_type> new_node = _allocator.template alloc<node_type>();

			auto last_node = _tail->prev;

			last_node->next = new_node;
			new_node->prev = last_node;

			_tail->prev = new_node;
			new_node->next = _tail;

			new (&new_node->data) T(value);

			++_count;
		}

		void
		insert_back(T&& value)
		{
			handle<node_type> new_node = _allocator.template alloc<node_type>();

			auto last_node = _tail->prev;

			last_node->next = new_node;
			new_node->prev = last_node;

			_tail->prev = new_node;
			new_node->next = _tail;

			new (&new_node->data) T(tmp::move(value));

			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			if(removal_count == 0)
				return;

			auto it = _head->next;

			while(removal_count--)
			{
				auto next_node = it->next;

				it.destroy();

				_allocator.free(it);

				it = next_node;
				--_count;
			}

			it->prev = _head;
			_head->next = it;
		}

		void
		remove_back(usize removal_count = 1)
		{
			auto it = _tail->prev;

			while(removal_count--)
			{
				auto prev_node = it->prev;

				it.destroy();
				_allocator.free(it);

				it = prev_node;
				--_count;
			}

			it->next = _tail;
			_tail->prev = it;
		}

		void
		reset()
		{
			if(_count == 0)
				return;

			auto it = _head->next;

			while(_count)
			{
				auto next_node = it->next;

				it.destroy();

				_allocator.free(it);

				it = next_node;

				--_count;
			}

			_head->next = _tail;
			_tail->prev = _head;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return const_iterator(_head->next);
		}

		iterator
		front()
		{
			return iterator(_head->next);
		}

		const_iterator
		back() const
		{
			return const_iterator(_tail->prev);
		}

		iterator
		back()
		{
			return iterator(_tail->prev);
		}

		const_iterator
		begin() const
		{
			return const_iterator(_head->next);
		}

		iterator
		begin()
		{
			return iterator(_head->next);
		}

		const_iterator
		end() const
		{
			return const_iterator(_tail);
		}

		iterator
		end()
		{
			return iterator(_tail);
		}

		owner_mem_block
		decay()
		{
			owner_mem_block result = resurrect(_head);
			_head.value_ptr = nullptr;
			_tail.value_ptr = nullptr;
			_count = 0;
			return result;
		}

		owner_mem_block
		decay_continuous()
		{
			//allocate the memory of supplied allocator
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

		void
		_init_sentinels()
		{
			_head.construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());
			_tail.construct(_allocator.alloc(sizeof(node_type)).template decay<node_type>());

			_tail->prev = _head;
			_head->next = _tail;
		}
	};
}
