#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/iterator.h"
#include "cpprelude/dlinked_list.h"
#include "cpprelude/dynamic_array.h"

#include <ostream>
#include <cstdlib>
#include <limits>
#include <cstring>

//because of the idiots at microsoft
#undef min
#undef max

namespace cpprelude
{
	template<typename T>
	struct string_slice
	{
		using iterator = sequential_iterator<T>;
		using const_iterator = sequential_iterator<const T>;
		using data_type = T;

		slice<T> _data;
		usize _count;

		string_slice():_count(0){}

		string_slice(const slice<T>& data)
			:_data(data)
		{
			_calc_count();
		}

		string_slice(slice<T>&& data)
			:_data(std::move(data))
		{
			_calc_count();
		}

		string_slice(T* data, usize str_count)
			:_data(data, str_count), _count(str_count)
		{}

		string_slice
		view(usize offset, usize char_count = 0)
		{
			if(char_count == 0)
				char_count = _count - offset;

			return string_slice(_data.view(offset, char_count));
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		capacity() const
		{
			return _data.count();
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		void
		reset()
		{
			_data.ptr = nullptr;
			_data.size = 0;
			_count = 0;
		}

		T*
		data()
		{
			return _data.ptr;
		}

		const T*
		data() const
		{
			return _data.ptr;
		}

		T&
		operator[](usize index)
		{
			return _data[index];
		}

		const T&
		operator[](usize index) const
		{
			return _data[index];
		}

		template<typename R>
		bool
		operator==(const string_slice<R>& other) const
		{
			if(_count != other._count)
				return false;

			auto it = _data.ptr;
			auto other_it = other._data.ptr;

			if(it == other_it)
				return true;

			auto it_count = _count;
			while(it_count--)
			{
				if(*it != *other_it)
					return false;

				++it;
				++other_it;
			}

			return true;
		}

		template<typename R>
		bool
		operator!=(const string_slice<R>& other) const
		{
			return !operator==(other);
		}

		bool
		operator<(const string_slice<T>& other) const
		{
			if(_count == 0 && other._count == 0)
				return false;
			else if(_count == 0)
				return true;
			else if(other._count == 0)
				return false;

			return std::strcmp(_data.ptr, other._data.ptr) < 0;
		}

		bool
		operator>(const string_slice<T>& other) const
		{
			if(_count == 0 && other._count == 0)
				return false;
			else if(_count == 0)
				return false;
			else if(other._count == 0)
				return true;

			return std::strcmp(_data.ptr, other._data.ptr) > 0;
		}

		bool
		operator<=(const string_slice<T>& other) const
		{
			return operator<(other) || operator==(other);
		}

		bool
		operator>=(const string_slice<T>& other) const
		{
			return operator>(other) || operator==(other);
		}

		iterator
		front()
		{
			return iterator(_data.ptr);
		}

		const_iterator
		front() const
		{
			return const_iterator(_data.ptr);
		}

		iterator
		back()
		{
			return iterator(_data.ptr + _count - 1);
		}

		const_iterator
		back() const
		{
			return const_iterator(_data.ptr + _count - 1);
		}

		iterator
		begin()
		{
			return iterator(_data.ptr);
		}

		const_iterator
		begin() const
		{
			return const_iterator(_data.ptr);
		}

		const_iterator
		cbegin() const
		{
			return const_iterator(_data.ptr);
		}

		iterator
		end()
		{
			return iterator(_data.ptr + _count);
		}

		const_iterator
		end() const
		{
			return const_iterator(_data.ptr + _count);
		}

		const_iterator
		cend() const
		{
			return const_iterator(_data.ptr + _count);
		}

		slice<T>
		decay()
		{
			return decay_continuous();
		}

		slice<T>
		decay_continuous()
		{
			_count = 0;
			return std::move(_data);
		}

		//associated functions
		template<typename AllocatorT = global_allocator>
		inline static string_slice
		from_cstring(const T* str, AllocatorT&& allocator = AllocatorT())
		{
			usize str_count = 0;

			auto it = str;
			while(*it++)
				++str_count;

			string_slice result(allocator.template alloc<T>(str_count + 1));
			result._count = str_count;

			it = str;
			auto res_it = result.data();
			while(str_count--)
				*res_it++ = *it++;

			*res_it = '\0';
			return result;
		}

		template<typename AllocatorT = global_allocator>
		static string_slice
		from_cstring(const T* str, usize count, AllocatorT&& allocator = AllocatorT())
		{
			string_slice result(allocator.template alloc<T>(count + 1));
			result._count = count;

			auto it = str;
			auto res_it = result.data();
			while(count--)
				*res_it++ = *it++;

			*res_it = '\0';
			return result;
		}

		static string_slice<const T>
		literal(const T* str, usize str_count)
		{
			return string_slice<const T>(make_slice(str, str_count+1));
		}

		template<typename AllocatorT = global_allocator>
		static string_slice
		copy(const string_slice& str, AllocatorT&& allocator = AllocatorT())
		{
			usize count = str.count();
			string_slice result(allocator.template alloc<T>(count + 1));

			auto it = str.data();
			auto res_it = result.data();

			while(count--)
				*res_it++ = *it++;

			*res_it = '\0';
			result._count = str._count;
			return result;
		}

		template<typename AllocatorT = global_allocator>
		static void
		dispose(string_slice& str, AllocatorT&& allocator = AllocatorT())
		{
			allocator.free(str._data);
			str._count = 0;
		}

		template<typename AllocatorT = global_allocator>
		static void
		dispose(string_slice&& str, AllocatorT&& allocator = AllocatorT())
		{
			allocator.free(std::move(str._data));
			str._count = 0;
		}

		// private procedures
		void
		_calc_count()
		{
			_count = 0;
			auto it = _data.ptr;
			auto it_count = _data.count();
			while(true)
			{
				if(*it == '\0')
					break;
				if(_count == it_count)
					break;
				++_count;
				++it;
			}
		}
	};

	template<typename T>
	using literal_slice = string_slice<const T>;

	using string = string_slice<char>;
	using literal = literal_slice<char>;
	using ustring = string_slice<wchar_t>;
	using uliteral = literal_slice<wchar_t>;

	template<typename T, usize slice_size>
	struct local_string_slice: string_slice<T>
	{
		T _buffer[slice_size];

		local_string_slice()
			:string_slice<T>(make_slice(_buffer, slice_size))
		{
			_buffer[0] = '\0';
			this->_count = 0;
		}

		local_string_slice(const T* str)
			:string_slice<T>(make_slice(_buffer, slice_size))
		{
			_buffer[slice_size-1] = '\0';
			auto it = str;
			auto count = slice_size - 1;
			auto buffer_it = this->data();
			while(*it && count--)
			{
				*buffer_it++ = *it++;
			}
			*buffer_it = '\0';
			this->_count = slice_size - 1 - count;
		}

		local_string_slice(const literal_slice<T>& str)
			:string_slice<T>(make_slice(_buffer, slice_size))
		{
			_buffer[slice_size-1] = '\0';
			auto it = str.data();
			auto count = slice_size - 1;
			auto buffer_it = this->data();
			while(*it && count--)
			{
				*buffer_it++ = *it++;
			}
			*buffer_it = '\0';
			this->_count = slice_size - 1 - count;
		}

		local_string_slice&
		operator=(const T* str)
		{
			auto it = str;
			auto count = slice_size - 1;
			auto buffer_it = this->data();
			while(*it && count--)
			{
				*buffer_it++ = *it++;
			}
			*buffer_it = '\0';
			this->_count = slice_size - 1 - count;
			return *this;
		}

		local_string_slice&
		operator=(const literal_slice<T>& str)
		{
			auto it = str.data();
			auto count = slice_size - 1;
			auto buffer_it = this->data();
			while(*it && count--)
			{
				*buffer_it++ = *it++;
			}
			*buffer_it = '\0';
			this->_count = slice_size - 1 - count;
			return *this;
		}
	};

	template<typename T>
	std::ostream&
	operator<<(std::ostream& out,const string_slice<T>& str)
	{
		out << str.data();
		return out;
	}

	template<usize slice_size>
	using local_string = local_string_slice<char, slice_size>;

	template<usize slice_size>
	using local_ustring = local_string_slice<wchar_t, slice_size>;

	//user defined stuff to help
	//auto koko_literal = "this is a literal string"_l;
	//auto koko_string = "this is a normal string"_s;
	API cpprelude::literal
	operator"" _l(const char* str, std::size_t str_count);

	API cpprelude::uliteral
	operator"" _l(const wchar_t* str, std::size_t str_count);

	API cpprelude::string
	operator"" _s(const char* str, std::size_t str_count);

	API cpprelude::ustring
	operator"" _s(const wchar_t* str, std::size_t str_count);

	//convert to string
	#define _define_string_show(type, pattern) \
	template<typename T>\
	inline static i32\
	_string_show(string_slice<T>& str, type val)\
	{\
		return std::snprintf(str.data(), str.capacity(), pattern, val);\
	}\
	template<typename T>\
	inline static i32\
	_string_show(string_slice<T>&& str, type val)\
	{\
		return _string_show(str, val);\
	}

	//convert primitive types
	_define_string_show(byte, "%c")
	_define_string_show(i8, "%hhd")
	_define_string_show(u8, "%hhu")
	_define_string_show(i16, "%hd")
	_define_string_show(u16, "%hu")
	_define_string_show(i32, "%d")
	_define_string_show(u32, "%u")
	_define_string_show(i64, "%ld")
	_define_string_show(u64, "%lu")
	_define_string_show(r32, "%f")
	_define_string_show(r64, "%f")
	_define_string_show(void*, "%p")

	#undef _define_string_show

	//convert strings into strings
	template<typename T>
	inline static i32
	_string_show(string_slice<T>& str, const string_slice<T>& val)
	{
		auto str_count = str.count();
		auto val_count = val.count();
		usize limit = str_count < val_count ? str_count : val_count;

		auto str_it = str.data();
		auto val_it = val.data();
		for(usize i = 0;
			i < limit;
			++i)
		{
			*str_it++ = *val_it++;
		}
		return limit;
	}

	template<typename T>
	inline static i32
	_string_show(string_slice<T>&& str, const string_slice<T>& val)
	{
		return _string_show(str, val);
	}

	template<typename T>
	inline static i32
	_string_show(string_slice<T>& str, const literal_slice<T>& val)
	{
		auto str_count = str.count();
		auto val_count = val.count();
		usize limit = str_count < val_count ? str_count : val_count;

		auto str_it = str.data();
		auto val_it = val.data();
		for(usize i = 0;
			i < limit;
			++i)
		{
			*str_it++ = *val_it++;
		}
		return limit;
	}

	template<typename T>
	inline static i32
	_string_show(string_slice<T>&& str, const literal_slice<T>& val)
	{
		return _string_show(str, val);
	}

	template<typename T>
	inline static i32
	_string_show(string_slice<T>& str, const T* val)
	{
		auto str_count = str.count();
		auto str_it = str.data();
		auto val_it = val;
		usize result = 0;

		while(*val_it && str_count--)
		{
			*str_it++ = *val_it++;
			++result;
		}
		return result;
	}

	template<typename T>
	inline static i32
	_string_show(string_slice<T>&& str, const T* val)
	{
		return _string_show(str, val);
	}

	//the write API
	template<typename T, typename FirstT>
	inline static i32
	write(string_slice<T>& str, FirstT&& first_arg)
	{
		return _string_show(str, std::forward<FirstT>(first_arg));
	}

	template<typename T, typename FirstT>
	inline static i32
	write(string_slice<T>&& str, FirstT&& first_arg)
	{
		return write(str, std::forward<FirstT>(first_arg));
	}

	//convert a serious of args into str
	template<typename T, typename FirstT, typename ... ArgsT>
	inline static i32
	write(string_slice<T>& str, FirstT&& first_arg, ArgsT&&... args)
	{
		auto next_start = _string_show(str, std::forward<FirstT>(first_arg));
		return next_start + write(str.view(next_start), args...);
	}

	template<typename T, typename FirstT, typename ... ArgsT>
	inline static i32
	write(string_slice<T>&& str, FirstT&& first_arg, ArgsT&&... args)
	{
		auto next_start = _string_show(str, std::forward<FirstT>(first_arg));
		return next_start + write(str.view(next_start), args...);
	}

	//the read API
	template<typename T>
	inline static bool
	read(const string_slice<T>& str, i16& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<i16>::max() ||
		   result_ < std::numeric_limits<i16>::min())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, i16& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<i16>::max() ||
		   result_ < std::numeric_limits<i16>::min())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, i32& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<i32>::max() ||
		   result_ < std::numeric_limits<i32>::min())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, i32& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<i32>::max() ||
		   result_ < std::numeric_limits<i32>::min())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, i64& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, i64& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoll(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, u16& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<u16>::max())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, u16& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<u16>::max())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, u32& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<u32>::max())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, u32& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		if(result_ > std::numeric_limits<u32>::max())
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, u64& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, u64& result, int base = 10)
	{
		T* end = nullptr;
		auto result_ = std::strtoull(str.begin(), &end, base);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, r32& result)
	{
		T* end = nullptr;
		auto result_ = std::strtof(str.begin(), &end);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, r32& result)
	{
		T* end = nullptr;
		auto result_ = std::strtof(str.begin(), &end);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const string_slice<T>& str, r64& result)
	{
		T* end = nullptr;
		auto result_ = std::strtod(str.begin(), &end);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	template<typename T>
	inline static bool
	read(const literal_slice<T>& str, r64& result)
	{
		T* end = nullptr;
		auto result_ = std::strtod(str.begin(), &end);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}
}
