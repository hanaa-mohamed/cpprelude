#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	namespace details
	{
		template<typename T>
		struct single_node
		{
			handle<single_node<T>> next;
			T data;
		};

		template<typename T>
		struct double_node
		{
			handle<double_node<T>> prev;
			handle<double_node<T>> next;
			T data;
		};
	}

	template<typename T>
	struct sequential_iterator
	{
		mem_block _data_block;
		usize _count;

		sequential_iterator()
			:_count(0)
		{}

		sequential_iterator(mem_block data_block, usize count)
			:_count(count),
			 _data_block(data_block)
		{}

		sequential_iterator<T>&
		operator++()
		{
			_count++;
			return *this;
		}

		sequential_iterator<T>
		operator++(int)
		{
			auto result = *this;
			_count++;
			return result;
		}

		sequential_iterator<T>&
		operator--()
		{
			--_count;
			return *this;
		}

		sequential_iterator<T>
		operator--(int)
		{
			auto result = *this;
			--_count;
			return result;
		}

		bool
		operator==(const sequential_iterator<T>& other) const
		{
			if(_data_block == other._data_block)
			{
				//if the two iterator is out of memory region then they are equal 
				if(_count >= _data_block.size / sizeof(T) && other._count >= other._data_block.size / sizeof(T))
					return true;
				else if(_count == other._count)
					return true;
			}
			return false;
		}

		bool
		operator!=(const sequential_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_data_block.template at<const T>(_count);
		}

		T&
		operator*()
		{
			return *_data_block.template at<T>(_count);
		}
	};

	template<typename T>
	struct forward_iterator
	{
		handle<details::single_node<T>> _node;

		forward_iterator(){}

		forward_iterator(handle<details::single_node<T>> node)
			:_node(node)
		{}

		forward_iterator&
		operator++()
		{
			if(_node)
				_node = _node->next;
			return *this;
		}

		forward_iterator
		operator++(int)
		{
			auto result = *this;
			if(_node)
				_node = _node->next;
			return result;
		}

		bool
		operator==(const forward_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const forward_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		T&
		operator*()
		{
			return _node->data;
		}
	};

	template<typename T>
	struct bidirectional_iterator
	{
		handle<details::double_node<T>> _node;

		bidirectional_iterator(){}

		bidirectional_iterator(handle<details::double_node<T>> node)
			:_node(node)
		{}

		bidirectional_iterator&
		operator++()
		{
			if(_node->next)
				_node = _node->next;

			return *this;
		}

		bidirectional_iterator
		operator++(int)
		{
			auto result = *this;

			if(_node->next)
				_node = _node->next;

			return result;
		}

		bidirectional_iterator&
		operator--()
		{
			if(_node->prev)
				_node = _node->prev;
			return *this;
		}

		bidirectional_iterator
		operator--(int)
		{
			auto result = *this;
			
			if(_node->prev)
				_node = _node->prev;

			return result;
		}

		bool
		operator==(const bidirectional_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const bidirectional_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		T&
		operator*()
		{
			return _node->data;
		}
	};

	template<typename T>
	T
	next(T it, usize n = 1)
	{
		while(n--)
			++it;
		return it;
	}

	template<typename T>
	T
	prev(T it, usize n = 1)
	{
		while(n--)
			--it;
		return it;
	}

	template<typename T>
	sequential_iterator<T>
	next(sequential_iterator<T> it, usize n = 1)
	{
		it._count += n;
		return it;
	}

	template<typename T>
	sequential_iterator<T>
	prev(sequential_iterator<T> it, usize n = 1)
	{
		it._count -= n;
		return it;
	}
}
