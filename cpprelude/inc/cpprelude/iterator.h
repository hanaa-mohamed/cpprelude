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
			single_node<T>* next;
			T data;

			single_node()
				:next(nullptr)
			{}
		};

		template<typename T>
		struct double_node
		{
			double_node<T>* prev;
			double_node<T>* next;
			T data;

			double_node()
				:next(nullptr), prev(nullptr)
			{}
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

		operator T*()
		{
			return _element;
		}

		operator const T*() const
		{
			return _element;
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
	struct const_forward_iterator;

	template<typename T>
	struct forward_iterator
	{
		using data_type = T;
		details::single_node<T>* _node;

		forward_iterator()
			:_node(nullptr)
		{}

		forward_iterator(details::single_node<T>* node)
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

		bool
		operator==(const const_forward_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_forward_iterator<T>& other) const
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
	struct const_forward_iterator
	{
		using data_type = const T;
		const details::single_node<T>* _node;

		const_forward_iterator()
			:_node(nullptr)
		{}

		const_forward_iterator(const details::single_node<T>* node)
			:_node(node)
		{}

		const_forward_iterator(const forward_iterator<T>& other)
			:_node(other._node)
		{}

		const_forward_iterator&
		operator++()
		{
			if(_node)
				_node = _node->next;
			return *this;
		}

		const_forward_iterator
		operator++(int)
		{
			auto result = *this;
			if(_node)
				_node = _node->next;
			return result;
		}

		bool
		operator==(const forward_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const forward_iterator<T>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_forward_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_forward_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T>
	struct const_bidirectional_iterator;

	template<typename T>
	struct bidirectional_iterator
	{
		using data_type = T;
		details::double_node<T>* _node;

		bidirectional_iterator()
			:_node(nullptr)
		{}

		bidirectional_iterator(details::double_node<T>* node)
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

		bool
		operator==(const const_bidirectional_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_bidirectional_iterator<T>& other) const
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
	struct const_bidirectional_iterator
	{
		using data_type = T;
		const details::double_node<T>* _node;

		const_bidirectional_iterator()
			:_node(nullptr)
		{}

		const_bidirectional_iterator(const details::double_node<T>* node)
			:_node(node)
		{}

		const_bidirectional_iterator(const bidirectional_iterator<T>& other)
			:_node(other._node)
		{}

		const_bidirectional_iterator&
		operator++()
		{
			if(_node->next)
				_node = _node->next;

			return *this;
		}

		const_bidirectional_iterator
		operator++(int)
		{
			auto result = *this;

			if(_node->next)
				_node = _node->next;

			return result;
		}

		const_bidirectional_iterator&
		operator--()
		{
			if(_node->prev)
				_node = _node->prev;
			return *this;
		}

		const_bidirectional_iterator
		operator--(int)
		{
			auto result = *this;

			if(_node->prev)
				_node = _node->prev;

			return result;
		}

		bool
		operator==(const bidirectional_iterator<T>& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const bidirectional_iterator<T>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_bidirectional_iterator& other) const
		{
			return _node == other._node;
		}

		bool
		operator!=(const const_bidirectional_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return _node->data;
		}

		const T*
		operator->() const
		{
			return &_node->data;
		}
	};

	template<typename T, usize bucket_size>
	struct const_bucket_array_iterator;

	template<typename T, usize bucket_size>
	struct bucket_array_iterator
	{
		using data_type = T;
		T** _bucket_it;
		T* _element_it;
		usize _index;

		bucket_array_iterator()
			:_bucket_it(nullptr), _element_it(nullptr), _index(0)
		{}

		bucket_array_iterator(T** bucket_it,
							  T* element_it, usize index)
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

		bool
		operator==(const const_bucket_array_iterator<T, bucket_size>& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const const_bucket_array_iterator<T, bucket_size>& other) const
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

	template<typename T, usize bucket_size>
	struct const_bucket_array_iterator
	{
		using data_type = T;
		const T** _bucket_it;
		const T* _element_it;
		usize _index;

		const_bucket_array_iterator()
			:_bucket_it(nullptr), _element_it(nullptr), _index(0)
		{}

		const_bucket_array_iterator(const T** bucket_it,
							  const T* element_it, usize index)
			:_bucket_it(bucket_it), _element_it(element_it), _index(index)
		{}

		const_bucket_array_iterator(const bucket_array_iterator<T, bucket_size>& other)
			:_bucket_it(const_cast<const T**>(other._bucket_it)),
			 _element_it(const_cast<const T*>(other._element_it)),
			 _index(other._index)
		{}

		const_bucket_array_iterator&
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

		const_bucket_array_iterator
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

		const_bucket_array_iterator&
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

		const_bucket_array_iterator
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
		operator==(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const bucket_array_iterator<T, bucket_size>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_bucket_array_iterator& other) const
		{
			return _element_it == other._element_it;
		}

		bool
		operator!=(const const_bucket_array_iterator& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_element_it;
		}

		const T*
		operator->() const
		{
			return &(*_element_it);
		}
	};

	template<typename key_type, typename value_type>
	struct const_hash_array_iterator;

	template<typename key_type, typename value_type>
	struct hash_array_iterator
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<value_type> value_it;
		sequential_iterator<u8> _flag_it;
		usize _capacity;

		hash_array_iterator()
		{_capacity = 0;}

		hash_array_iterator(const key_type* key, value_type* value, u8* flag, usize capacity)
			:key_it(key), value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_iterator& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_hash_array_iterator<key_type, value_type>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_iterator<key_type, value_type>& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		value_type&
		value()
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		value_type*
		operator->()
		{
			return value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename key_type, typename value_type>
	struct const_hash_array_iterator
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<const value_type> value_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		const_hash_array_iterator()
		{_capacity = 0;}

		const_hash_array_iterator(const key_type* key, const value_type* value, const u8* flag, usize capacity)
			:key_it(key), value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		const_hash_array_iterator(const hash_array_iterator<key_type, value_type>& other)
			:key_it(other.key_it),
			 value_it(other.value_it),
			 _flag_it(other._flag_it),
			 _capacity(other._capacity)
		{}

		const_hash_array_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		const_hash_array_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_iterator<key_type, value_type>& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_iterator<key_type, value_type>& other) const
		{
			return !operator==(other);
		}

		bool
		operator==(const const_hash_array_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_iterator& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename key_type>
	struct hash_array_key_iterator
	{
		sequential_iterator<const key_type> key_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		hash_array_key_iterator()
		{_capacity = 0;}

		hash_array_key_iterator(const key_type* key,const u8* flag, usize capacity)
			:key_it(key), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_key_iterator&
		operator++()
		{
			++_flag_it;
			++key_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_key_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++key_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++key_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_key_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   key_it 		== other.key_it   &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_key_iterator& other) const
		{
			return !operator==(other);
		}

		const key_type&
		operator*() const
		{
			return *key_it;
		}

		const key_type&
		key() const
		{
			return *key_it;
		}

		const key_type*
		operator->() const
		{
			return key_it;
		}
	};

	template<typename value_type>
	struct hash_array_value_iterator
	{
		sequential_iterator<value_type> value_it;
		sequential_iterator<u8> _flag_it;
		usize _capacity;

		hash_array_value_iterator()
		{_capacity = 0;}

		hash_array_value_iterator(value_type* value, u8* flag, usize capacity)
			:value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		hash_array_value_iterator&
		operator++()
		{
			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		hash_array_value_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const hash_array_value_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const hash_array_value_iterator& other) const
		{
			return !operator==(other);
		}

		value_type&
		operator*()
		{
			return *value_it;
		}

		const value_type&
		operator*() const
		{
			return *value_it;
		}

		value_type&
		value()
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		value_type*
		operator->()
		{
			return value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

	template<typename value_type>
	struct const_hash_array_value_iterator
	{
		sequential_iterator<const value_type> value_it;
		sequential_iterator<const u8> _flag_it;
		usize _capacity;

		const_hash_array_value_iterator()
		{_capacity = 0;}

		const_hash_array_value_iterator(const value_type* value, const u8* flag, usize capacity)
			:value_it(value), _flag_it(flag), _capacity(capacity)
		{}

		const_hash_array_value_iterator&
		operator++()
		{
			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return *this;
		}

		const_hash_array_value_iterator&
		operator++(int)
		{
			auto result = *this;

			++_flag_it;
			++value_it;
			--_capacity;

			while(*_flag_it == 0 && _capacity > 0)
			{
				++_flag_it;
				++value_it;
				--_capacity;
			}

			return result;
		}

		bool
		operator==(const const_hash_array_value_iterator& other) const
		{
			return _flag_it 	== other._flag_it &&
				   value_it 	== other.value_it &&
				   _capacity	== other._capacity;
		}

		bool
		operator!=(const const_hash_array_value_iterator& other) const
		{
			return !operator==(other);
		}

		const value_type&
		operator*() const
		{
			return *value_it;
		}

		const value_type&
		value() const
		{
			return *value_it;
		}

		const value_type*
		operator->() const
		{
			return value_it;
		}
	};

   	template<typename iterator, typename const_iterator>
	struct view
	{
		iterator _begin_it;
		iterator _end_it;
		const_iterator _cbegin_it;
		const_iterator _cend_it;

		view(const iterator& begin_it, const iterator& end_it,
			 const const_iterator& cbegin_it, const const_iterator& cend_it)
			:_begin_it(begin_it), _end_it(end_it),
			 _cbegin_it(cbegin_it), _cend_it(cend_it)
		{}

		view()
		{}

		iterator
		begin()
		{
			auto result = _begin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		begin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		cbegin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		iterator
		end()
		{
			return _end_it;
		}

		const_iterator
		end() const
		{
			return _cend_it;
		}

		const_iterator
		cend() const
		{
			return _cend_it;
		}
	};

	template<typename const_iterator>
	struct const_view
	{
		const_iterator _cbegin_it;
		const_iterator _cend_it;

		const_view(const const_iterator& cbegin_it, const const_iterator&  cend_it)
			:_cbegin_it(cbegin_it), _cend_it(cend_it)
		{}

		const_view()
		{}

		const_iterator
		begin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		cbegin() const
		{
			auto result = _cbegin_it;
			if(*result._flag_it == 0)
				++result;
			return result;
		}

		const_iterator
		end() const
		{
			return _cend_it;
		}

		const_iterator
		cend() const
		{
			return _cend_it;
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
