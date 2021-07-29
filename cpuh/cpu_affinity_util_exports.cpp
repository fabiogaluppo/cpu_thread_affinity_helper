/* -------------------------------------------------------------------------------------------------
*
* CPU THREAD AFFINITY HELPER
* ==========================
*
* This file:
*    cpu_affinity_util C exports, especially to be exposed as dynamic library (.dll or .so)
*
* Authors:
*   Fabio Galuppo
*
* Releases: July 29 2021 - v0.1
    v0.1 - July 2021 - initial release
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

#include "cpu_affinity_util.h"
#include "cpu_affinity_util.hpp"

uint64_t cpuh_get_thread_affinity()
{
    return cpuh::cpu_affinity_util::get_thread_affinity();
}

void cpuh_set_thread_affinity(uint64_t affinity)
{
    cpuh::cpu_affinity_util::set_thread_affinity(affinity);
}

int32_t cpuh_get_processor()
{
    return cpuh::cpu_affinity_util::get_processor();
}

int32_t cpuh_get_number_of_processors()
{
    return cpuh::cpu_affinity_util::get_number_of_processors();
}

uint64_t cpuh_get_process_affinity()
{
    return cpuh::cpu_affinity_util::get_process_affinity();
}

int32_t cpuh_get_process_id()
{
    return cpuh::cpu_affinity_util::get_process_id();
}

int32_t cpuh_get_thread_id()
{
    return cpuh::cpu_affinity_util::get_thread_id();
}