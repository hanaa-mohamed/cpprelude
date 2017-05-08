#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"

namespace cpprelude
{
	template<typename T>
	struct sequential_iterator
	{
		weak_mem_block _data_block;
		usize _count;

		sequential_iterator()
			:_count(0)
		{}

		sequential_iterator(weak_mem_block data_block, usize count)
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
		weak_mem_block _node_block;

		forward_iterator(){}

		forward_iterator(weak_mem_block node_block)
			:_node_block(node_block)
		{}

		forward_iterator<T>&
		operator++()
		{
			if(_node_block.ptr != nullptr && _node_block.size > 0)
				_node_block = *_node_block.template as<weak_mem_block>();
			return *this;
		}

		forward_iterator<T>
		operator++(int)
		{
			auto result = *this;
			if(_node_block.ptr != nullptr && _node_block.size > 0)
				_node_block = *_node_block.template as<weak_mem_block>();
			return result;
		}

		bool
		operator==(const forward_iterator<T>& other) const
		{
			return _node_block == other._node_block;
		}

		bool
		operator!=(const forward_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_node_block.template as<const T>(sizeof(weak_mem_block));
		}

		T&
		operator*()
		{
			return *_node_block.template as<T>(sizeof(weak_mem_block));
		}
	};

	template<typename T>
	struct bidirectional_iterator
	{
		weak_mem_block _node_block;

		bidirectional_iterator(){}

		bidirectional_iterator(weak_mem_block node_block)
			:_node_block(node_block)
		{}

		bidirectional_iterator<T>&
		operator++()
		{
			if(_node_block.ptr != nullptr && _node_block.size > 0)
			{
				auto next_block = *_node_block.template as<weak_mem_block>(sizeof(weak_mem_block));
				if(next_block.ptr != nullptr && next_block.size > 0)
					_node_block = next_block;
			}
			return *this;
		}

		bidirectional_iterator<T>
		operator++(int)
		{
			auto result = *this;

			if(_node_block.ptr != nullptr && _node_block.size > 0)
			{
				auto next_block = *_node_block.template as<weak_mem_block>(sizeof(weak_mem_block));
				if(next_block.ptr != nullptr && next_block.size > 0)
					_node_block = next_block;
			}

			return result;
		}

		bidirectional_iterator<T>&
		operator--()
		{
			if(_node_block.ptr != nullptr && _node_block.size > 0)
			{
				auto prev_block = *_node_block.template as<weak_mem_block>();
				if(prev_block.ptr != nullptr && prev_block.size > 0)
					_node_block = prev_block;
			}
			return *this;
		}

		bidirectional_iterator<T>
		operator--(int)
		{
			auto result = *this;
			if(_node_block.ptr != nullptr && _node_block.size > 0)
			{
				auto prev_block = *_node_block.template as<weak_mem_block>();
				if(prev_block.ptr != nullptr && prev_block.size > 0)
					_node_block = prev_block;
			}
			return result;
		}

		bool
		operator==(const bidirectional_iterator<T>& other) const
		{
			return _node_block == other._node_block;
		}

		bool
		operator!=(const bidirectional_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			return *_node_block.template as<const T>(2*sizeof(weak_mem_block));
		}

		T&
		operator*()
		{
			return *_node_block.template as<T>(2*sizeof(weak_mem_block));
		}
	};

	template<typename T>
	T next(T it)
	{
		return ++it;
	}

	template<typename T>
	T prev(T it)
	{
		return --it;
	}
}