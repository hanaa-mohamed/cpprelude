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
		using const_iterator = bidirectional_iterator<const T>;
		using data_type = T;

		owner_mem_block _head;
		owner_mem_block _tail;
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

			for(const auto& value: list)
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

			auto other_it = other._head.template as<owner_mem_block>(sizeof(owner_mem_block));
			for(usize i = 0; i < other.count(); ++i)
			{
				insert_back(*other_it->template as<T>(2*sizeof(owner_mem_block)));
				other_it = other_it->template as<owner_mem_block>(sizeof(owner_mem_block));
			}
		}

		dlinked_list(const dlinked_list<T>& other, const AllocatorT& allocator)
			:_count(0), _allocator(allocator)
		{
			_init_sentinels();

			auto other_it = other._head.template as<owner_mem_block>(sizeof(owner_mem_block));
			for(usize i = 0; i < other.count(); ++i)
			{
				insert_back(*other_it->template as<T>(2*sizeof(owner_mem_block)));
				other_it = other_it->template as<owner_mem_block>(sizeof(owner_mem_block));
			}
		}

		dlinked_list(dlinked_list<T>&& other)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _tail(tmp::move(other._tail)),
			 _allocator(other._allocator)
		{
			other._count = 0;
		}

		dlinked_list(dlinked_list<T>&& other, const AllocatorT& allocator)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _tail(tmp::move(other._tail)),
			 _allocator(allocator)
		{
			other._count = 0;
		}

		~dlinked_list()
		{
			reset();
			_allocator.free(_head);
			_allocator.free(_tail);
			_count = 0;
		}

		dlinked_list<T>&
		operator=(const dlinked_list<T>& other)
		{
			reset();
			_allocator = other._allocator;

			auto other_it = other._head.template as<owner_mem_block>(sizeof(owner_mem_block));
			for(usize i = 0; i < other.count(); ++i)
			{
				insert_back(*other_it->template as<T>(2*sizeof(owner_mem_block)));
				other_it = other_it->template as<owner_mem_block>(sizeof(owner_mem_block));
			}

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

			return*this;
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
				//traverse from tail
				auto it = &_tail;
				while(index--)
				{
					it = it->template as<owner_mem_block>();
				}

				return *(it->template as<T>(2 * sizeof(owner_mem_block)));
			}
			else
			{
				//traverse from head
				auto it = _head.template as<owner_mem_block>(sizeof(owner_mem_block));
				while(index--)
				{
					it = it->template as<owner_mem_block>(sizeof(owner_mem_block));
				}

				return *(it->template as<T>(2 * sizeof(owner_mem_block)));
			}
		}

		T
		operator[](usize index) const
		{
			if(index >= _count * 0.5)
			{
				index = _count - index;
				//traverse from tail
				auto it = &_tail;
				while(index--)
				{
					it = it->template as<owner_mem_block>();
				}

				return *(it->template as<T>(2 * sizeof(owner_mem_block)));
			}
			else
			{
				//traverse from head
				auto it = _head.template as<owner_mem_block>(sizeof(owner_mem_block));
				while(index--)
				{
					it = it->template as<owner_mem_block>(sizeof(owner_mem_block));
				}

				return *(it->template as<T>(2 * sizeof(owner_mem_block)));
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
			auto new_memory_block = _allocator.alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//get the first node in the list
			auto first_node = _head.template as<owner_mem_block>(sizeof(owner_mem_block));
			//make new_memory_block as previous to the first node
			*first_node->template as<weak_mem_block>() = new_memory_block.sub_block();

			//make _head as previous to new_memory_block
			new (new_memory_block.template as<weak_mem_block>()) weak_mem_block(_head.sub_block());

			//make first_node as next to new_memoey_block
			new (new_memory_block.template as<weak_mem_block>(sizeof(weak_mem_block))) weak_mem_block(first_node->sub_block());

			//put the value inside the new_memory_block
			new (new_memory_block.template as<T>(2*sizeof(weak_mem_block))) T(value);

			//make the head next point to this node
			new (_head.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block(tmp::move(new_memory_block));

			++_count;
		}

		void
		insert_front(T&& value)
		{
			auto new_memory_block = _allocator.alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//get the first node in the list
			auto first_node = _head.template as<owner_mem_block>(sizeof(owner_mem_block));
			//make new_memory_block as previous to the first node
			*first_node->template as<weak_mem_block>() = new_memory_block.sub_block();

			//make _head as previous to new_memory_block
			new (new_memory_block.template as<weak_mem_block>()) weak_mem_block(_head.sub_block());

			//make first_node as next to new_memoey_block
			new (new_memory_block.template as<weak_mem_block>(sizeof(weak_mem_block))) weak_mem_block(first_node->sub_block());

			//put the value inside the new_memory_block
			new (new_memory_block.template as<T>(2*sizeof(weak_mem_block))) T(tmp::move(value));

			//make the head next point to this node
			new (_head.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block(tmp::move(new_memory_block));

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
			auto new_memory_block = _allocator.alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//get the last node in the list
			auto last_node = _tail.template as<owner_mem_block>();
			//make the next of last_node point to new_memory_block
			*last_node->template as<weak_mem_block>(sizeof(weak_mem_block)) = new_memory_block.sub_block();

			//make last_node as previous to new_memory_block
			new (new_memory_block.template as<weak_mem_block>()) weak_mem_block(last_node->sub_block());

			//make _tail as next to new_memory_blcok
			new (new_memory_block.template as<weak_mem_block>(sizeof(weak_mem_block))) weak_mem_block(_tail.sub_block());

			//assign the value
			new (new_memory_block.template as<T>(2*sizeof(weak_mem_block))) T(value);

			//make new_memory_block as previous to _tail
			new (_tail.template as<owner_mem_block>()) owner_mem_block(tmp::move(new_memory_block));

			++_count;
		}

		void
		insert_back(T&& value)
		{
			auto new_memory_block = _allocator.alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//get the last node in the list
			auto last_node = _tail.template as<owner_mem_block>();
			//make the next of last_node point to new_memory_block
			*last_node->template as<weak_mem_block>(sizeof(weak_mem_block)) = new_memory_block.sub_block();

			//make last_node as previous to new_memory_block
			new (new_memory_block.template as<weak_mem_block>()) weak_mem_block(last_node->sub_block());

			//make _tail as next to new_memory_blcok
			new (new_memory_block.template as<weak_mem_block>(sizeof(weak_mem_block))) weak_mem_block(_tail.sub_block());

			//assign the value
			new (new_memory_block.template as<T>(2*sizeof(weak_mem_block))) T(tmp::move(value));

			//make new_memory_block as previous to _tail
			new (_tail.template as<owner_mem_block>()) owner_mem_block(tmp::move(new_memory_block));

			++_count;
		}

		void
		remove_front(usize removal_count = 1)
		{
			auto it = tmp::move(*_head.template as<owner_mem_block>(sizeof(owner_mem_block)));

			for(usize i = 0; i < removal_count; ++i)
			{
				auto next_block = tmp::move(*it.template as<owner_mem_block>(sizeof(owner_mem_block)));

				it.template as<T>(2*sizeof(owner_mem_block))->~T();

				_allocator.free(it);
				it = tmp::move(next_block);
				--_count;
			}

			*it.template as<weak_mem_block>() = _head.sub_block();
			new (_head.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block(tmp::move(it));
		}

		void
		remove_back(usize removal_count = 1)
		{
			auto it = tmp::move(*_tail.template as<owner_mem_block>());

			for(usize i = 0; i < removal_count; ++i)
			{
				auto next_block = tmp::move(*it.template as<owner_mem_block>());

				it.template as<T>(2*sizeof(owner_mem_block))->~T();

				_allocator.free(it);
				it = tmp::move(next_block);
				--_count;
			}

			*it.template as<weak_mem_block>(sizeof(weak_mem_block)) = _tail.sub_block();

			new (_tail.template as<owner_mem_block>()) owner_mem_block(tmp::move(it));
		}

		void
		reset()
		{
			if(_count == 0)
				return;

			auto it = tmp::move(*_head.template as<owner_mem_block>(sizeof(owner_mem_block)));
			while(_count)
			{
				auto next_block = tmp::move(*it.template as<owner_mem_block>(sizeof(owner_mem_block)));

				it.template as<T>(2*sizeof(owner_mem_block))->~T();
				_allocator.free(it);
				it = tmp::move(next_block);
				--_count;
			}

			it.release();

			//make _tail prev -> _head
			*_tail.template as<weak_mem_block>() = _head.sub_block();
			//make _tail next -> empty block
			new (_tail.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block();

			//make _head next -> _tail
			*_head.template as<weak_mem_block>(sizeof(weak_mem_block)) = _tail.sub_block();
			//make _head prev -> empty block
			new (_head.template as<owner_mem_block>()) owner_mem_block();
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		bidirectional_iterator<const T>
		front() const
		{
			return bidirectional_iterator<const T>(*_head.template as<weak_mem_block>(sizeof(weak_mem_block)));
		}

		bidirectional_iterator<T>
		front()
		{
			return bidirectional_iterator<T>(*_head.template as<weak_mem_block>(sizeof(weak_mem_block)));
		}

		bidirectional_iterator<const T>
		back() const
		{
			return bidirectional_iterator<const T>(*_tail.template as<weak_mem_block>());
		}

		bidirectional_iterator<T>
		back()
		{
			return bidirectional_iterator<T>(*_tail.template as<weak_mem_block>());
		}

		bidirectional_iterator<const T>
		begin() const
		{
			return bidirectional_iterator<const T>(*_head.template as<weak_mem_block>(sizeof(weak_mem_block)));
		}

		bidirectional_iterator<T>
		begin()
		{
			return bidirectional_iterator<T>(*_head.template as<weak_mem_block>(sizeof(weak_mem_block)));
		}

		bidirectional_iterator<const T>
		end() const
		{
			return bidirectional_iterator<const T>(_tail.sub_block());
		}

		bidirectional_iterator<T>
		end()
		{
			return bidirectional_iterator<T>(_tail.sub_block());
		}

		void
		_init_sentinels()
		{
			_head = _allocator.alloc(2*sizeof(owner_mem_block));
			_tail = _allocator.alloc(2*sizeof(owner_mem_block));

			//make _tail prev -> _head
			*_tail.template as<weak_mem_block>() = _head.sub_block();
			//make _tail next -> empty block
			new (_tail.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block();

			//make _head next -> _tail
			*_head.template as<weak_mem_block>(sizeof(weak_mem_block)) = _tail.sub_block();
			//make _head prev -> empty block
			new (_head.template as<owner_mem_block>()) owner_mem_block();
		}
	};
}