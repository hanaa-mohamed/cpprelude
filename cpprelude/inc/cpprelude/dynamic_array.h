#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/tmp.h"
#include "cpprelude/iterator.h"
#include "cpprelude/allocator.h"
#include <initializer_list>
#include <new>
#include <iterator>

namespace cpprelude
{
	//configurations
	constexpr r32 grow_factor = 1.5f;
	constexpr usize starting_count = 64;

	template<typename T, typename AllocatorT = global_allocator>
	struct dynamic_array
	{
		using iterator = sequential_iterator<T>;
		using const_iterator = const sequential_iterator<T>;
		using data_type = T;

		owner_mem_block _data_block;
		usize _count;
		AllocatorT _allocator;

		dynamic_array(const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{}

		dynamic_array(std::initializer_list<T> list, const AllocatorT& allocator = AllocatorT())
			:_count(list.size()), _allocator(allocator)
		{
			_data_block = _allocator.alloc(_count*sizeof(T));
			auto it = list.begin();
			for(usize i = 0; i < _count; ++i)
			{
				new (_data_block.template at<T>(i)) T(*it);
				it = std::next(it);
			}
		}

		dynamic_array(usize count, const AllocatorT& allocator = AllocatorT())
			:_count(count), _allocator(allocator)
		{ _data_block = _allocator.alloc(count*sizeof(T)); }

		dynamic_array(usize count, const T& fill_value, const AllocatorT& allocator = AllocatorT())
			:_count(0), _allocator(allocator)
		{
			expand_back(count, fill_value);
		}

		dynamic_array(const dynamic_array<T, AllocatorT>& other)
			:_count(other._count),
			 _allocator(other._allocator)
		{
			_data_block = _allocator.alloc(other._data_block.size);

			for(usize i = 0; i < _count; ++i)
				new (_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));
		}

		dynamic_array(const dynamic_array<T, AllocatorT>& other, const AllocatorT& allocator)
			:_count(other._count),
			 _allocator(allocator)
		{
			_data_block = _allocator.alloc(other._data_block.size);

			for(usize i = 0; i < _count; ++i)
				new (_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));
		}

		dynamic_array(dynamic_array<T, AllocatorT>&& other)
			:_count(other._count),
			 _data_block(tmp::move(other._data_block)),
			 _allocator(tmp::move(other._allocator))
		{
			other._count = 0;
		}

		dynamic_array(dynamic_array<T, AllocatorT>&& other, const AllocatorT& allocator)
			:_count(other._count),
			 _data_block(tmp::move(other._data_block)),
			 _allocator(allocator)
		{
			other._count = 0;
		}

		~dynamic_array()
		{
			for(usize i = 0; i < _count; ++i)
				_data_block.template at<T>(i)->~T();
			_count = 0;

			_allocator.free(_data_block);
		}

		dynamic_array<T, AllocatorT>&
		operator=(const dynamic_array<T, AllocatorT>& other)
		{
			owner_mem_block tmp_data_block = _allocator.alloc(other._data_block.size);
			for(usize i = 0; i < other._count; ++i)
				new (tmp_data_block.template at<T>(i)) T(*other._data_block.template at<T>(i));

			_allocator.free(_data_block);
			_data_block = tmp::move(tmp_data_block);
			_count = other._count;
			_allocator = other._allocator;

			return *this;
		}

		dynamic_array<T, AllocatorT>&
		operator=(dynamic_array<T, AllocatorT>&& other)
		{
			for(usize i = 0; i < _count; ++i)
				_data_block.template at<T>(i)->~T();
			_allocator.free(_data_block);

			_data_block = tmp::move(other._data_block);
			_count = other._count;
			_allocator = tmp::move(other._allocator);
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
			_mem_expand(new_count*sizeof(T));
		}

		void
		expand_back(usize additional_count)
		{
			_mem_expand((_count+additional_count) * sizeof(T));

			for(usize i = 0; i < additional_count; ++i)
				new (_data_block.template at<T>(_count+i)) T();
			
			_count += additional_count;
		}

		void
		expand_back(usize additional_count, const T& fill_value)
		{
			_mem_expand((_count+additional_count) * sizeof(T));

			auto old_count = _count;
			_count += additional_count;

			for(usize i = old_count; i < _count; ++i)
				new (_data_block.template at<T>(i)) T(fill_value);
		}

		void
		shrink_back(usize shrinkage_count)
		{
			remove_back(shrinkage_count);
			shrink_to_fit();
		}

		void
		shrink_to_fit()
		{
			_mem_shrink(_count * sizeof(T));
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
					_mem_expand(lsize*sizeof(T));
				else
					_mem_expand((capacity_+lsize)*grow_factor*sizeof(T));
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
					_mem_expand(starting_count*sizeof(T));
				else
					_mem_expand(capacity_*grow_factor*sizeof(T));
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
					_mem_expand(starting_count*sizeof(T));
				else
					_mem_expand(capacity_*grow_factor*sizeof(T));
			}

			new (_data_block.template at<T>(_count++)) T(tmp::move(value));
		}

		void
		remove_back(usize removal_count = 1)
		{
			_count -= removal_count;
			for(usize i = _count; i < _count+removal_count; ++i)
				_data_block.template at<T>(i)->~T();
		}

		void
		clear()
		{
			for(usize i = 0; i < _count; ++i)
				_data_block.template at<T>(i)->~T();
			_count = 0;
		}

		void
		reset()
		{
			for(usize i = 0; i < _count; ++i)
				_data_block.template at<T>(i)->~T();
			_count = 0;
			_allocator.free(_data_block);
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		const_iterator
		front() const
		{
			return const_iterator(_data_block.template as<T>());
		}

		iterator
		front()
		{
			return iterator(_data_block.template as<T>());
		}

		const_iterator
		back() const
		{
			return const_iterator(_data_block.template at<T>(_count-1));
		}

		iterator
		back()
		{
			return iterator(_data_block.template at<T>(_count-1));
		}

		const_iterator
		begin() const
		{
			return const_iterator(_data_block.template as<T>());
		}

		iterator
		begin()
		{
			return iterator(_data_block.template as<T>());
		}

		const_iterator
		end() const
		{
			return const_iterator(_data_block.template at<T>(_count));
		}

		iterator
		end()
		{
			return iterator(_data_block.template at<T>(_count));
		}

		owner_mem_block
		decay()
		{
			owner_mem_block result = tmp::move(_data_block);
			_count = 0;
			return result;
		}

		owner_mem_block
		decay_continuous()
		{
			owner_mem_block result = tmp::move(_data_block);
			_count = 0;
			return result;	
		}

		void
		_mem_expand(usize new_size)
		{
			if(_data_block.size >= new_size)
				return;

			if(_data_block.ptr != nullptr && _data_block.size > 0)
				_allocator.realloc(_data_block, new_size);
			else
				_data_block = _allocator.alloc(new_size);
		}

		void
		_mem_shrink(usize new_size)
		{
			if(_data_block.size <= new_size)
				return;

			if(_data_block.ptr != nullptr && _data_block.size > 0)
				_allocator.realloc(_data_block, new_size);
			else
				_data_block = _allocator.alloc(new_size);
		}
	};
}
