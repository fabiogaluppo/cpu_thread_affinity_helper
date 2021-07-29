/* -------------------------------------------------------------------------------------------------
*
* CPU THREAD AFFINITY HELPER
* ==========================
*
* This file:
*    cpu_affinity_util test driver for Windows DLL
*
* Authors: 
*    Fabio Galuppo
*
* Releases: July 29 2021 - v0.1
*    v0.1 - July 2021 - initial release
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

#include <windows.h>
#include <cstdint>
#include <iostream>
#include <bitset>
#include <thread>

int main(int argc, char* argv[]) 
{
    HINSTANCE hDLL = LoadLibrary(TEXT("cpu_affinity_util.dll"));
    if (hDLL == NULL)
    {
        std::cerr << "error: " << GetLastError() << '\n';
        return -1;
    }

    typedef uint64_t (*cpuh_get_thread_affinity_fn)();
    typedef void (*cpuh_set_thread_affinity_fn)(uint64_t);
    typedef int32_t (*cpuh_get_processor_fn)();
    typedef int32_t (*cpuh_get_number_of_processors_fn)();
    typedef uint64_t (*cpuh_get_process_affinity_fn)();
    typedef int32_t (*cpuh_get_process_id_fn)();
    typedef int32_t (*cpuh_get_thread_id_fn)();

    cpuh_get_thread_affinity_fn cpuh_get_thread_affinity = (cpuh_get_thread_affinity_fn) GetProcAddress(hDLL, "cpuh_get_thread_affinity");
    cpuh_set_thread_affinity_fn cpuh_set_thread_affinity = (cpuh_set_thread_affinity_fn) GetProcAddress(hDLL, "cpuh_set_thread_affinity");
    cpuh_get_processor_fn cpuh_get_processor =  (cpuh_get_processor_fn) GetProcAddress(hDLL, "cpuh_get_processor");
    cpuh_get_number_of_processors_fn cpuh_get_number_of_processors = (cpuh_get_number_of_processors_fn) GetProcAddress(hDLL, "cpuh_get_number_of_processors");
    cpuh_get_process_affinity_fn cpuh_get_process_affinity = (cpuh_get_process_affinity_fn) GetProcAddress(hDLL, "cpuh_get_process_affinity");
    cpuh_get_process_id_fn cpuh_get_process_id = (cpuh_get_process_id_fn) GetProcAddress(hDLL, "cpuh_get_process_id");
    cpuh_get_thread_id_fn cpuh_get_thread_id = (cpuh_get_thread_id_fn) GetProcAddress(hDLL, "cpuh_get_thread_id");

    std::cout << "main thread:\n";
    std::cout << "process id = " << cpuh_get_process_id() << "\n";
    std::cout << "thread id = " << cpuh_get_thread_id() << "\n";
    std::cout << "# of processors = " << cpuh_get_number_of_processors() << "\n";
    std::cout << "processor id = " << cpuh_get_processor() << "\n";
    std::cout << "process affinity = " << cpuh_get_process_affinity() << "\n";
    std::cout << "thread affinity = " << cpuh_get_thread_affinity() << "\n";
    std::thread t([&] {
        std::cout << "\nbefore change thread affinity:\n";
        std::cout << "process id = " << cpuh_get_process_id() << "\n";
        std::cout << "thread id = " << cpuh_get_thread_id() << "\n";
        std::cout << "# of processors = " << cpuh_get_number_of_processors() << "\n";
        std::cout << "processor id = " << cpuh_get_processor() << "\n";
        std::cout << "process affinity = " << cpuh_get_process_affinity() << "\n";
        std::cout << "thread affinity = " << cpuh_get_thread_affinity() << "\n";

        std::bitset<64> mask(cpuh_get_thread_affinity());
        std::cout << "thread affinity mask = " << mask << "\n";

        std::bitset<8> new_mask(std::string("00000010"));
        int affinity = static_cast<int>(new_mask.to_ulong());
        cpuh_set_thread_affinity(affinity);

        std::cout << "\nafter change thread affinity:\n";

        std::cout << "process id = " << cpuh_get_process_id() << "\n";
        std::cout << "thread id = " << cpuh_get_thread_id() << "\n";
        std::cout << "# of processors = " << cpuh_get_number_of_processors() << "\n";
        std::cout << "processor id = " << cpuh_get_processor() << "\n";
        std::cout << "process affinity = " << cpuh_get_process_affinity() << "\n";
        std::cout << "thread affinity = " << cpuh_get_thread_affinity() << "\n";

        mask = cpuh_get_thread_affinity();
        std::cout << "thread affinity mask = " << mask << "\n";
    });
    t.join();

    FreeLibrary(hDLL);

    return 0;
}