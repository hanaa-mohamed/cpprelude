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

		sequential_iterator<T>&
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

		sequential_iterator<T>&
		operator--(int)
		{
			auto result = *this;
			--_count;
			return result;
		}

		bool
		operator==(const sequential_iterator<T>& other) const
		{
			return _data_block == other._data_block && _count == other._count;
		}

		bool
		operator!=(const sequential_iterator<T>& other) const
		{
			return !operator==(other);
		}

		const T&
		operator*() const
		{
			if(_count < _data_block.size / sizeof(T))
				return *_data_block.template at<const T>(_count);
		}

		T&
		operator*()
		{
			return *_data_block.template at<T>(_count);
		}
	};
}