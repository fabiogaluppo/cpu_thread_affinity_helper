/* -------------------------------------------------------------------------------------------------
*
* CPU THREAD AFFINITY HELPER
* ==========================
*
* This file:
*    cpu_affinity_util C++ header-only
*
* Authors: 
*   Fabio Galuppo
*
* Releases: July 29 2021 - v0.1
*   v0.1 - July 2021 - initial release
*
* The MIT License (MIT)
* Copyright © 2021 Fabio Galuppo
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the “Software”), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
------------------------------------------------------------------------------------------------- */

#ifndef CPU_AFFINITY_UTIL_HPP
#define CPU_AFFINITY_UTIL_HPP

#include <cstdint>

//INTERFACE

namespace cpuh
{
    struct cpu_affinity_util
    {
        //CLASS METHODS
        static uint64_t get_thread_affinity();
        static void set_thread_affinity(uint64_t affinity);
        static int32_t get_processor();
        static int32_t get_number_of_processors();
        static uint64_t get_process_affinity();
        static int32_t get_process_id() noexcept;
        static int32_t get_thread_id() noexcept;
    };
}

//CORE OS IMPLEMENTATION

#ifdef _WIN32

//WINDOWS

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601 //Windows 7
#include <windows.h>

namespace cpuh
{
    inline uint64_t cpu_affinity_util::get_thread_affinity()
    {
        HANDLE hThread = ::GetCurrentThread();
        DWORD_PTR retval = ::SetThreadAffinityMask(hThread, -1);
        if (!retval) 
        {
            //TODO: error
            //GetLastError ERROR_INVALID_PARAMETER
            return 0;
        }
        unsigned long long oldval = retval;
        retval = ::SetThreadAffinityMask(hThread, oldval);
        if (!retval) 
        {
            //TODO: error
            //GetLastError ERROR_INVALID_PARAMETER
            return 0;
        }
        return oldval;
    }

    inline void cpu_affinity_util::set_thread_affinity(uint64_t affinity)
    {
        //TODO: assert affinity (<= 64)
        DWORD_PTR dwThreadAffinityMask = affinity;
        HANDLE hThread = ::GetCurrentThread();
        DWORD_PTR retval = ::SetThreadAffinityMask(hThread, dwThreadAffinityMask);
        if (!retval) 
        {
            //TODO: error
            //GetLastError ERROR_INVALID_PARAMETER
        }
    }

    inline int32_t cpu_affinity_util::get_processor()
    {
        DWORD processor_id = ::GetCurrentProcessorNumber();
        //TODO: error
        return processor_id;    
    }

    inline int32_t cpu_affinity_util::get_number_of_processors()
    {
        DWORD count = ::GetActiveProcessorCount(0);
        //TODO: error
        return count;
    }

    inline uint64_t cpu_affinity_util::get_process_affinity()
    {
        const HANDLE CURRENT_PROCESS = reinterpret_cast<HANDLE>(-1);
        DWORD_PTR processAffinityMask, systemAffinityMask;
        BOOL retval = ::GetProcessAffinityMask(CURRENT_PROCESS, &processAffinityMask, &systemAffinityMask);
        if (!retval) 
        {
            //TODO: error
            return 0;
        }
        return processAffinityMask;    
    }

    inline int32_t cpu_affinity_util::get_process_id() noexcept
    {
        DWORD pid = ::GetCurrentProcessId();
        return pid;
    }

    inline int32_t cpu_affinity_util:: get_thread_id() noexcept
    {
        DWORD tid = ::GetCurrentThreadId();
        return tid;
    }
}

#elif  __linux__

//LINUX

#include <unistd.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/syscall.h>

namespace cpuh
{
    namespace internal 
    {
        static inline uint64_t get_thread_affinity(pid_t pid) 
        {
            ::cpu_set_t mask;
            int retval = ::sched_getaffinity(pid, sizeof(cpu_set_t), &mask);
            if (retval) 
            {
                //TODO: error
                //errno
                return 0;
            }
            return mask.__bits[0];
        }
    }

    inline uint64_t cpu_affinity_util::get_thread_affinity()
    {
        return internal::get_thread_affinity(0);
    }

    inline void cpu_affinity_util::set_thread_affinity(uint64_t affinity)
    {
        ::cpu_set_t mask;
        CPU_ZERO(&mask);
        mask.__bits[0] = affinity;
        int retval = ::sched_setaffinity(0, sizeof(cpu_set_t), &mask);
        if (retval) 
        {
            //TODO: error
            //errno
        }
        return;
    }

    inline int32_t cpu_affinity_util::get_processor()
    {
        int cpu = ::sched_getcpu();
        //TODO: error
        return cpu;
    }

    inline int32_t cpu_affinity_util::get_number_of_processors()
    {
        int count = ::get_nprocs_conf();
        //TODO: error
        return count;
    }

    inline uint64_t cpu_affinity_util::get_process_affinity()
    {
        return internal::get_thread_affinity(::getpid());    
    }

    inline int32_t cpu_affinity_util::get_process_id() noexcept
    {
        pid_t pid = ::getpid();
        return pid;
    }

    inline int32_t cpu_affinity_util::get_thread_id() noexcept
    {
        pid_t tid = ::syscall(SYS_gettid);
        return tid;
    }
}

#else
#error "OS not supported"
#endif

#endif //CPU_AFFINITY_UTIL_HPP
