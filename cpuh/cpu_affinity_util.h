/* -------------------------------------------------------------------------------------------------
*
* CPU THREAD AFFINITY HELPER
* ==========================
*
* This file:
*    cpu_affinity_util C interface, especially to be exposed as dynamic library (.dll or .so)
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

#ifndef CPU_AFFINITY_UTIL_H
#define CPU_AFFINITY_UTIL_H

#include <cstdint>

#ifdef _WIN32

#ifdef CPU_AFFINITY_UTIL_EXPORTS
#define CPU_AFFINITY_UTIL_CALL __declspec(dllexport)
#else
#define CPU_AFFINITY_UTIL_CALL __declspec(dllimport)
#endif

#elif  __linux__

#define CPU_AFFINITY_UTIL_CALL

#else
#error "OS not supported"
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

CPU_AFFINITY_UTIL_CALL uint64_t cpuh_get_thread_affinity();
CPU_AFFINITY_UTIL_CALL void cpuh_set_thread_affinity(uint64_t affinity);
CPU_AFFINITY_UTIL_CALL int32_t cpuh_get_processor();
CPU_AFFINITY_UTIL_CALL int32_t cpuh_get_number_of_processors();
CPU_AFFINITY_UTIL_CALL uint64_t cpuh_get_process_affinity();
CPU_AFFINITY_UTIL_CALL int32_t cpuh_get_process_id();
CPU_AFFINITY_UTIL_CALL int32_t cpuh_get_thread_id();

#ifdef __cplusplus
}
#endif

#endif //CPU_AFFINITY_UTIL_H
