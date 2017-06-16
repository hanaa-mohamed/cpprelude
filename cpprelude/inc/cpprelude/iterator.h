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
		T* _element;
		using data_type = T;

		sequential_iterator()
			:_element(nullptr)
		{}

		sequential_iterator(T* ptr)
			:_element(ptr)
		{}

		sequential_iterator<T>&
		operator++()
		{
			_element++;
			return *this;
		}

		sequential_iterator<T>
		operator++(int)
		{
			auto result = *this;
			_element++;
			return result;
		}

		sequential_iterator<T>&
		operator--()
		{
			--_element;
			return *this;
		}

		sequential_iterator<T>
		operator--(int)
		{
			auto result = *this;
			--_element;
			return result;
		}

		bool
		operator==(const sequential_iterator<T>& other) const
		{
			return _element == other._element;
		}

		bool
		operator!=(const sequential_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_element;
		}

		T&
		operator*()
		{
			return *_element;
		}

		T*
		operator->()
		{
			return _element;
		}

		const T*
		operator->() const
		{
			return _element;
		}
	};

	template<typename T>
	struct forward_iterator
	{
		using data_type = T;
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

		T*
		operator->()
		{
			return &_node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T>
	struct bidirectional_iterator
	{
		using data_type = T;
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

		T*
		operator->()
		{
			return &_node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T, usize bucket_size>
	struct bucket_array_iterator
	{
		using data_type = T;
		handle<handle<T>> _bucket_it;
		handle<T> _element_it;
		usize _index;

		bucket_array_iterator()
			{}

		bucket_array_iterator(handle<handle<T>> bucket_it,
							  handle<T> element_it, usize index)
			:_bucket_it(bucket_it), _element_it(element_it), _index(index)
		{}

		bucket_array_iterator&
		operator++()
		{
			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return *this;
		}

		bucket_array_iterator
		operator++(int)
		{
			auto result = *this;

			++_index;

			if (_index >= bucket_size)
			{
				_index = 0;
				++_bucket_it;
				_element_it = *_bucket_it;
			}
			else
			{
				++_element_it;
			}

			return result;
		}

		bucket_array_iterator&
		operator--()
		{
			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return *this;
		}

		bucket_array_iterator
		operator--(int)
		{
			auto result = *this;

			if(_index != 0)
			{
				--_element_it;
				--_index;
			}
			else
			{
				--_bucket_it;
				_index = bucket_size - 1;
				_element_it = *_bucket_it + _index;
			}

			return result;
		}

		bool
		operator==(const bucket_array_iterator& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const bucket_array_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_element_it;
		}

		T&
		operator*()
		{
			return *_element_it;
		}

		T*
		operator->()
		{
			return &(*_element_it);
		}

		const T*
		operator->() const
		{
			return &(*_element_it);
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
		it._element += n;
		return it;
	}

	template<typename T>
	sequential_iterator<T>
	prev(sequential_iterator<T> it, usize n = 1)
	{
		it._element -= n;
		return it;
	}
}
