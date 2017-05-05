#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/iterator.h"
#include <initializer_list>
#include <new>
#include <iterator>

namespace cpprelude
{
	//configurations
	constexpr r32 grow_factor = 1.5f;
	constexpr usize starting_count = 64;

	template<typename T>
	struct dynamic_array
	{
		owner_mem_block _data_block;
		usize _count;

		dynamic_array()
			:_count(0)
		{}

		dynamic_array(std::initializer_list<T> list)
			:_count(list.size())
		{
			_data_block = alloc(_count*sizeof(T));
			auto it = list.begin();
			for(usize i = 0; i < _count; ++i)
			{
				new (_data_block.template at<T>(i)) T(*it);
				it = std::next(it);
			}
		}

		dynamic_array(usize count)
			:_data_block(alloc(count*sizeof(T))),
			 _count(count)
		{}

		dynamic_array(usize count, const T& fill_value)
			:_count(0)
		{
			resize(count, fill_value);
		}

		dynamic_array(const dynamic_array<T>& other)
			:_count(other._count),
			 _data_block(alloc(other._data_block.size))
		{
			for(usize i = 0; i < _count; ++i)
				new (_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));
		}

		dynamic_array(dynamic_array<T>&& other)
			:_count(other._count),
			_data_block(tmp::move(other._data_block))
		{
			other._count = 0;
		}

		~dynamic_array()
		{
			_count = 0;
		}

		dynamic_array<T>&
		operator=(const dynamic_array<T>& other)
		{
			auto tmp_data_block = alloc(other._data_block.size);
			for(usize i = 0; i < other._count; ++i)
				new (tmp_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));

			_data_block = tmp::move(tmp_data_block);
			_count = other._count;

			return *this;
		}

		dynamic_array<T>&
		operator=(dynamic_array<T>&& other)
		{
			_data_block = tmp::move(other._data_block);
			_count = other._count;
			other._count = 0;

			return *this;
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		capacity() const
		{
			return _data_block.size / sizeof(T);
		}

		void
		reserve(usize new_count)
		{
			_mem_resize(new_count*sizeof(T));
		}

		void
		resize(usize new_count)
		{
			_mem_resize(new_count*sizeof(T));
			_count = new_count;
		}

		void
		resize(usize new_count, const T& fill_value)
		{
			_mem_resize(new_count*sizeof(T));

			for(usize i = _count; i < new_count; ++i)
				new (_data_block.template at<T>(i)) T(fill_value);

			_count = new_count;
		}

		T&
		operator[](usize index)
		{
			return *_data_block.template at<T>(index);
		}

		T
		operator[](usize index) const
		{
			return *_data_block.template at<T>(index);
		}

		const T*
		data() const
		{
			return _data_block.template at<T>(0);
		}

		T*
		data()
		{
			return _data_block.template at<T>(0);
		}

		void
		insert_back(std::initializer_list<T> list)
		{
			auto lsize = list.size();
			auto capacity_ = capacity();
			if(_count+lsize >= capacity_)
			{
				if(capacity_ == 0)
					_mem_resize(lsize*sizeof(T));
				else
					_mem_resize((capacity_+lsize)*grow_factor*sizeof(T));
			}

			auto it = list.begin();
			for(usize i = _count; i < _count+lsize; ++i)
			{
				new (_data_block.template at<T>(i)) T(*it);
				it = std::next(it);
			}

			_count += lsize;
		}

		void
		insert_back(const T& value)
		{
			auto capacity_ = capacity();
			if(_count >= capacity_)
			{
				if(capacity_ == 0)
					_mem_resize(starting_count*sizeof(T));
				else
					_mem_resize(capacity_*grow_factor*sizeof(T));
			}

			new (_data_block.template at<T>(_count++)) T(value);
		}

		void
		insert_back(T&& value)
		{
			auto capacity_ = capacity();
			if(_count >= capacity_)
			{
				if(capacity_ == 0)
					_mem_resize(starting_count*sizeof(T));
				else
					_mem_resize(capacity_*grow_factor*sizeof(T));
			}

			new (_data_block.template at<T>(_count++)) T(tmp::move(value));
		}

		void
		clear()
		{
			_count = 0;
		}

		void
		reset()
		{
			_count = 0;
			_data_block.~owner_mem_block();
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		sequential_iterator<const T>
		begin() const
		{
			return sequential_iterator<const T>(_data_block.sub_block(), 0);
		}

		sequential_iterator<T>
		begin()
		{
			return sequential_iterator<T>(_data_block.sub_block(), 0);
		}

		sequential_iterator<const T>
		end() const
		{
			return sequential_iterator<const T>(_data_block.sub_block(), _count);
		}

		sequential_iterator<T>
		end()
		{
			return sequential_iterator<T>(_data_block.sub_block(), _count);
		}

		void
		_mem_resize(usize new_size)
		{
			if(_data_block.size >= new_size)
				return;

			auto tmp_data_block = alloc(new_size);
			for(usize i = 0; i < _count; ++i)
				new (tmp_data_block.template at<T>(i)) T(tmp::move(*_data_block.template at<T>(i)));

			_data_block = tmp::move(tmp_data_block);
		}
	};
}