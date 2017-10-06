#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
    namespace platform
    {
        API void*
        virtual_alloc(void* address_hint, usize size);

        API bool
        virtual_free(void* ptr, usize size);
    }
}