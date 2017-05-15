#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"

namespace cpprelude
{
	template<typename T>
	struct queue_array
	{
		dynamic_array<T> _array;
		usize _first;
		usize _last;
		usize _count;

		queue_array()
			:_first(0),_last(0),_count(0)
		{}

		queue_array(usize count)
			:_array(count), _first(0), _last(0),_count(0)
		{}

		void
		enqueue(const T& item)
		{
			if (_count == _array.count())
			{
				_array.insert_back(item);
				++_last;
			}
			else if (_last == _array.count())
			{
				//Shifting array content to start from 0 index
				for (usize i = _first, j=0; i < _last; i++,j++)
				{
					_array[j] = _array[i];
				}
				
				_first = 0;
				_last = _count +1;
				_array[_count] = item;
				
			}
			else {
				_array[_last] = item;
				++_last;
			}

			++_count;

		}

		void
		enqueue(T&& item)
		{
			if (_count == _array.count())
			{
				_array.insert_back(tmp::move(item));
				++_last;
			}
			else if (_last == _array.count())
			{
				//Shifting array content to start from 0 index
				for (usize i = _first, j = 0; i < _last; i++, j++)
				{
					_array[j] = tmp::move(_array[i]);
				}

				_first = 0;
				_last = _count + 1;
				_array[_count] = tmp::move(item);

			}
			else {
				_array[_last] = tmp::move(item);
				++_last;
			}

			++_count;
		}

		bool
		dequeue()
		{
			if (_count > 0)
			{
				--_count;
				++_first;
				if (_count <= _array.count() * 0.25)
				{
					//Shifting array content to start from 0 index
					for (usize i = _first, j = 0; i < _last; i++, j++)
					{
						_array[j] = _array[i];
					}

					_first = 0;
					_last = _count;
					_array.shrink_back(_array.count() - _count);
				}
				return true;
			}

			return false;
		}

		const T&
		front() const
		{
			return _array[_first];
		}

		T&
		front()
		{
			return _array[_first];
		}

		bool
		empty()
		{
			return _count == 0;
		}

		usize
		count()
		{
			return _count;
		}

	};
}