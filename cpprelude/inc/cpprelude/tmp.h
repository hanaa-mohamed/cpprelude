#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
	namespace tmp
	{
		template<typename T>
		struct remove_reference
		{ typedef T type; };

		template<typename T>
		struct remove_reference<T&>
		{ typedef T type; };

		template<typename T>
		struct remove_reference<T&&>
		{ typedef T type; };

		template<typename T>
		constexpr typename remove_reference<T>::type&&
		move(T&& value)
		{
			return static_cast<typename remove_reference<T>::type&&>(value);
		}
	}
}