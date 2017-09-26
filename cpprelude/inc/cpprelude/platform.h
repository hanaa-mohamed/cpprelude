#pragma once

#include "cpprelude/defines.h"

namespace cpprelude
{
    namespace platform
    {
        void*
        virtual_alloc(void* address_hint, usize size);

        bool
        virtual_free(void* ptr, usize size);
    }
}