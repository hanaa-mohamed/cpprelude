#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/memory.h"
#include "cpprelude/iterator.h"
#include "cpprelude/tmp.h"
#include "cpprelude/dlinked_list.h"
#include "cpprelude/bucket_array.h"

#include <ostream>
#include <cstdlib>
#include <limits>

//because of the idiots at microsoft
#define max max
#define min min

namespace cpprelude
{
	template<typename T>
	struct string_slice
	{
		using iterator = sequential_iterator<T>;
		using const_iterator = const sequential_iterator<T>;
		using data_type = T;

		slice<T> _data;

		string_slice()
		{}

		string_slice(const slice<T>& data)
			:_data(data)
		{}

		string_slice(slice<T>&& data)
			:_data(data)
		{}

		string_slice
		view(usize offset, usize count = 0)
		{
			if(count == 0)
				count = this->count() - offset;
			return string_slice(_data.cut(offset, count+1));
		}

		usize
		count() const
		{
			if(_data.valid())
				return (_data.size / sizeof(T)) - 1;
			return 0;
		}

		bool
		empty() const
		{
			return count() == 0;
		}

		void
		reset()
		{
			_data.ptr = nullptr;
			_data.size = 0;
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
			if(count() != other.count())
				return false;

			auto it = _data.ptr;
			auto other_it = other._data.ptr;

			if(it == other_it)
				return true;

			auto count_ = count();
			while(count_--)
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
			return iterator(_data.ptr + count() - 1);
		}

		const_iterator
		back() const
		{
			return const_iterator(_data.ptr + count() - 1);
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

		iterator
		end()
		{
			return iterator(_data.ptr + count());
		}

		const_iterator
		end() const
		{
			return const_iterator(_data.ptr + count());
		}

		slice<T>
		decay()
		{
			return decay_continuous();
		}

		slice<T>
		decay_continuous()
		{
			return tmp::move(_data);
		}

		//associated functions
		template<typename AllocatorT = global_allocator>
		static string_slice
		from_cstring(const T* str, AllocatorT&& allocator = AllocatorT())
		{
			usize str_count = 0;

			auto it = str;
			while(*it++)
				++str_count;

			string_slice result(allocator.template alloc<T>(str_count + 1));

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
			auto literal_data_slice = make_slice(str, str_count+1);
			return string_slice<const T>(literal_data_slice);
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
			return result;
		}

		template<typename AllocatorT = global_allocator>
		static void
		dispose(string_slice& str, AllocatorT&& allocator = AllocatorT())
		{
			allocator.free(str._data);
		}

		template<typename AllocatorT = global_allocator>
		static void
		dispose(string_slice&& str, AllocatorT&& allocator = AllocatorT())
		{
			allocator.free(tmp::move(str._data));
		}
	};

	template<typename T>
	using literal_slice = string_slice<const T>;

	using string = string_slice<char>;
	using literal = literal_slice<char>;
	using ustring = string_slice<wchar_t>;
	using uliteral = literal_slice<wchar_t>;

	template<typename T, usize slice_size>
	struct local_string_slice: public string_slice<T>
	{
		T _buffer[slice_size];

		local_string_slice()
			:string_slice<T>(make_slice(_buffer, slice_size))
		{
			_buffer[slice_size-1] = '\0';
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

	template<typename T, typename AllocatorT = global_allocator>
	struct writer
	{
		using container_type = bucket_array<T,
			   						 details::default_size(sizeof(string_slice<T>)),
									 AllocatorT>;
		container_type _buffer;
		AllocatorT _allocator;

		writer(const AllocatorT& allocator = AllocatorT())
			:_buffer(allocator), _allocator(allocator)
		{}

		void
		write_back(const string_slice<T>& str)
		{
			for(const auto& ch: str)
				_buffer.insert_back(ch);
		}

		void
		write_front(const string_slice<T>& str)
		{
			auto it = str.back();
			usize count = str.count();

			while(count--)
				_buffer.insert_front(*it--);
		}

		void
		write_back(string_slice<T>&& str)
		{
			for(const auto& ch: str)
				_buffer.insert_back(ch);
		}

		void
		write_front(string_slice<T>&& str)
		{
			auto it = str.back();
			usize count = str.count();

			while(count--)
				_buffer.insert_front(*it--);
		}

		void
		write_back(const literal_slice<T>& str)
		{
			for(const auto& ch: str)
				_buffer.insert_back(ch);
		}

		void
		write_front(const literal_slice<T>& str)
		{
			auto it = str.back();
			usize count = str.count();

			while(count--)
				_buffer.insert_front(*it--);
		}

		void
		write_back(literal_slice<T>&& str)
		{
			for(const auto& ch: str)
				_buffer.insert_back(ch);
		}

		void
		write_front(literal_slice<T>&& str)
		{
			auto it = str.back();
			usize count = str.count();

			while(count--)
				_buffer.insert_front(*it--);
		}

		void
		clear()
		{
			_buffer.clear();
		}

		bool
		empty() const
		{
			return _buffer.count() == 0;
		}

		string_slice<T>
		output()
		{
			usize count = _buffer.count();
			if(count == 0)
				return string_slice<T>();

			string_slice<T> result(_allocator.template alloc<T>(count + 1));

			auto it = result.data();
			for(const auto& ch: _buffer)
				*it++ = ch;
			*it = '\0';

			clear();
			return result;
		}

		template<typename NewAllocT = AllocatorT>
		string_slice<T>
		output(const NewAllocT& allocator)
		{
			usize count = _buffer.count();
			if(count == 0)
				return string_slice<T>();

			string_slice<T> result(allocator.template alloc<T>(count + 1));

			auto it = result.data();
			for(const auto& ch: _buffer)
				*it++ = ch;
			*it = '\0';

			clear();
			return result;
		}
	};

	template<typename AllocatorT = global_allocator>
	using string_writer = writer<char, AllocatorT>;

	template<typename AllocatorT = global_allocator>
	using ustring_writer = writer<wchar_t, AllocatorT>;

	//convert to string
	//convert ubyte = u8

	//convert byte
	template<typename T>
	i32
	show(string_slice<T>& str, byte val)
	{
		return std::snprintf(str.data(), str.count(), "%c", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, byte val)
	{
		return show(str, val);
	}

	//convert i8
	template<typename T>
	i32
	show(string_slice<T>& str, i8 val)
	{
		return std::snprintf(str.data(), str.count(), "%hhd", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, i8 val)
	{
		return show(str, val);
	}

	//convert u8
	template<typename T>
	i32
	show(string_slice<T>& str, u8 val)
	{
		return std::snprintf(str.data(), str.count(), "%hhu", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, u8 val)
	{
		return show(str, val);
	}

	//convert i16
	template<typename T>
	i32
	show(string_slice<T>& str, i16 val)
	{
		return std::snprintf(str.data(), str.count(), "%hd", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, i16 val)
	{
		return show(str, val);
	}

	//convert u16
	template<typename T>
	i32
	show(string_slice<T>& str, u16 val)
	{
		return std::snprintf(str.data(), str.count(), "%hu", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, u16 val)
	{
		return show(str, val);
	}

	//convert i32
	template<typename T>
	i32
	show(string_slice<T>& str, i32 val)
	{
		return std::snprintf(str.data(), str.count(), "%d", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, i32 val)
	{
		return show(str, val);
	}

	//convert u32
	template<typename T>
	i32
	show(string_slice<T>& str, u32 val)
	{
		return std::snprintf(str.data(), str.count(), "%u", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, u32 val)
	{
		return show(str, val);
	}

	//convert i64
	template<typename T>
	i32
	show(string_slice<T>& str, i64 val)
	{
		return std::snprintf(str.data(), str.count(), "%ld", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, i64 val)
	{
		return show(str, val);
	}

	//convert u64
	template<typename T>
	i32
	show(string_slice<T>& str, u64 val)
	{
		return std::snprintf(str.data(), str.count(), "%lu", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, u64 val)
	{
		return show(str, val);
	}

	//convert r32
	template<typename T>
	i32
	show(string_slice<T>& str, r32 val)
	{
		return std::snprintf(str.data(), str.count(), "%f", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, r32 val)
	{
		return show(str, val);
	}

	//convert r64
	template<typename T>
	i32
	show(string_slice<T>& str, r64 val)
	{
		return std::snprintf(str.data(), str.count(), "%f", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, r64 val)
	{
		return show(str, val);
	}

	//convert pointer
	template<typename T>
	i32
	show(string_slice<T>& str, void* val)
	{
		return std::snprintf(str.data(), str.count(), "%p", val);
	}

	template<typename T>
	i32
	show(string_slice<T>&& str, void* val)
	{
		return show(str, val);
	}

	template<typename T>
	i32
	show(string_slice<T>& str, const string_slice<T>& val)
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
	i32
	show(string_slice<T>&& str, const string_slice<T>& val)
	{
		return show(str, val);
	}

	template<typename T>
	i32
	show(string_slice<T>& str, const literal_slice<T>& val)
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
	i32
	show(string_slice<T>&& str, const literal_slice<T>& val)
	{
		return show(str, val);
	}

	template<typename T>
	i32
	show(string_slice<T>& str, const T* val)
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
	i32
	show(string_slice<T>&& str, const T* val)
	{
		return show(str, val);
	}

	template<typename T, typename FirstT>
	i32
	write(string_slice<T>& str, FirstT&& first_arg)
	{
		return show(str, tmp::forward<FirstT>(first_arg));
	}

	template<typename T, typename FirstT>
	i32
	write(string_slice<T>&& str, FirstT&& first_arg)
	{
		return write(str, tmp::forward<FirstT>(first_arg));
	}

	//convert a serious of args into str
	template<typename T, typename FirstT, typename ... ArgsT>
	i32
	write(string_slice<T>& str, FirstT&& first_arg, ArgsT&&... args)
	{
		auto next_start = show(str, tmp::forward<FirstT>(first_arg));
		return next_start + write(str.view(next_start), args...);
	}

	template<typename T, typename FirstT, typename ... ArgsT>
	i32
	write(string_slice<T>&& str, FirstT&& first_arg, ArgsT&&... args)
	{
		auto next_start = show(str, tmp::forward<FirstT>(first_arg));
		return next_start + write(str.view(next_start), args...);
	}

	template<typename T>
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
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
	bool
	read(const literal_slice<T>& str, r64& result)
	{
		T* end = nullptr;
		auto result_ = std::strtod(str.begin(), &end);

		if(end == nullptr)
			return false;

		result = result_;
		return true;
	}

	API cpprelude::literal
	operator"" _l(const char* str, std::size_t str_count);

	API cpprelude::uliteral
	operator"" _l(const wchar_t* str, std::size_t str_count);

	API cpprelude::string
	operator"" _s(const char* str, std::size_t str_count);

	API cpprelude::ustring
	operator"" _s(const wchar_t* str, std::size_t str_count);
}
