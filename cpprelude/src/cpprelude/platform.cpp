#include "cpprelude/platform.h"

#if defined(OS_WINDOWS)
#include <Windows.h>
#elif defined(OS_LINUX)
#include <sys/mman.h>
#endif

namespace cpprelude
{
    namespace platform
    {
        void*
        virtual_alloc(void* address_hint, usize size)
        {
            if(size == 0)
                return nullptr;

            void* result = nullptr;

            #if defined(OS_WINDOWS)
                result = VirtualAlloc(address_hint, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            #elif defined(OS_LINUX)
                result = mmap(address_hint, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
            #endif

            return result;
        }

        bool
        virtual_free(void* ptr, usize size)
        {
            #if defined(OS_WINDOWS)
                return VirtualFree(ptr, size, MEM_RELEASE) != NULL;
            #elif defined(OS_LINUX)
                return munmap(ptr, size) == 0;
            #endif
        }
    }
}