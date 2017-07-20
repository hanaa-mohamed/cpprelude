#include "cpprelude/string.h"

cpprelude::literal
cpprelude::operator"" _l(const char* str, std::size_t str_count)
{
	return cpprelude::string::literal(str, str_count);
}

cpprelude::uliteral
cpprelude::operator"" _l(const wchar_t* str, std::size_t str_count)
{
	return cpprelude::ustring::literal(str, str_count);
}

cpprelude::string
cpprelude::operator"" _s(const char* str, std::size_t str_count)
{
	return cpprelude::string::from_cstring(str, str_count);
}

cpprelude::ustring
cpprelude::operator"" _s(const wchar_t* str, std::size_t str_count)
{
	return cpprelude::ustring::from_cstring(str, str_count);
}
