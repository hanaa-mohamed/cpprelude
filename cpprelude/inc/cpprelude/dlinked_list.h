#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/iterator.h"


namespace cpprelude
{
	/*
	 * data is layouted this way
	 * owner_mem_block = [sizeof(owner_mem_block): "prev" | sizeof(owner_mem_block): "next" | T]
	 * this has three parts a previous owner_mem_block which has the previous element, a next owner_mem_block which has the next element and a T which houses the data
	 */

	template<typename T>
	struct dlinked_list
	{
		owner_mem_block _head;
		owner_mem_block _tail;
		usize _count;

		dlinked_list()
			:_count(0)
		{}

		dlinked_list(std::initializer_list<T> list)
			:_count(0)
		{
			for(const auto& value: list)
				insert_back(value);
		}

		dlinked_list(usize count; const T& fill_value)
			:_count(0)
		{
			for(usize i = 0; i < count; ++i)
				insert_back(fill_value);
		}

		dlinked_list(const dlinked_list<T>& other)
			:_count(0)
		{
			for(const auto& value: other)
				insert_back(value);
		}

		dlinked_list(dlinked_list<T>&& other)
			:_count(other._count),
			 _head(tmp::move(other._head)),
			 _tail(tmp::move(other._tail))
		{
			other._count = 0;
		}

		~dlinked_list()
		{
			reset();
		}

		dlinked_list<T>&
		operator=(const dlinked_list<T>& other)
		{
			reset();

			for(const auto& value: other)
				insert_back(value);

			return *this;
		}

		dlinked_list<T>&
		operator=(dlinked_list<T>&& other)
		{
			reset();

			_count = other._count
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
				auto it = &_head;
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
				auto it = &_head;
				while(index--)
				{
					it = it->template as<owner_mem_block>(sizeof(owner_mem_block));
				}

				return *(it->template as<T>(2 * sizeof(owner_mem_block)));
			}
		}

		void
		insert_front(const T& value)
		{
			auto new_memory_block = alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//copy the new_memory_block to the prev of the _head
			new (_head.template as<weak_mem_block>()) T(new_memory_block.sub_block());

			//move the _head to the next block
			new (new_memory_block.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block(tmp::move(_head));

			new (new_memory_block.template as<T>(sizeof(owner_mem_block) * 2)) T(value);

			//now make the _head hold the new_memory_block
			_head = tmp::move(new_memory_block);
			++_count;
		}

		void
		insert_front(T&& value)
		{
			auto new_memory_block = alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//copy the new_memory_block to the prev of the _head
			new (_head.template as<weak_mem_block>()) T(new_memory_block.sub_block());

			//move the _head to the next block
			new (new_memory_block.template as<owner_mem_block>(sizeof(owner_mem_block))) owner_mem_block(tmp::move(_head));

			new (new_memory_block.template as<T>(sizeof(owner_mem_block) * 2)) T(tmp::move(value));

			//now make the _head hold the new_memory_block
			_head = tmp::move(new_memory_block);
			++_count;
		}

		void
		insert_back(const T& value)
		{
			auto new_memory_block = alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//copy the new_memory_block to the next of the _tail
			new (_tail.template as<weak_mem_block>(sizeof(owner_mem_block))) T(new_memory_block.sub_block());

			//move the _tail to the prev of the new_memory_block
			new (new_memory_block.template as<owner_mem_block>()) owner_mem_block(tmp::move(_tail));

			new (new_memory_block.template as<T>(sizeof(owner_mem_block) * 2)) T(value);

			_tail = tmp::move(new_memory_block);
		}

		void
		insert_back(T&& value)
		{
			auto new_memory_block = alloc(2*sizeof(owner_mem_block) + sizeof(T));

			//copy the new_memory_block to the next of the _tail
			new (_tail.template as<weak_mem_block>(sizeof(owner_mem_block))) T(new_memory_block.sub_block());

			//move the _tail to the prev of the new_memory_block
			new (new_memory_block.template as<owner_mem_block>()) owner_mem_block(tmp::move(_tail));

			new (new_memory_block.template as<T>(sizeof(owner_mem_block) * 2)) T(tmp::move(value));

			_tail = tmp::move(new_memory_block);
		}
	};
}