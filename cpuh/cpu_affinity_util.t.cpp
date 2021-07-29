/* -------------------------------------------------------------------------------------------------
*
* CPU THREAD AFFINITY HELPER
* ==========================
*
* This file:
*    cpu_affinity_util test driver for C++ as header-only
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

#include "cpu_affinity_util.hpp"

#include <iostream>
#include <iomanip>
#include <bitset>
#include <thread>
#include <chrono>

void test_2(bool use_thread_affinity)
{
    using std::chrono::system_clock;
    using std::chrono::minutes;
    using std::chrono::seconds;

    std::cout << "-------------------------------------\n";

    std::cout << "set thread affinity? " << std::boolalpha << use_thread_affinity << '\n';

    std::thread t1([use_thread_affinity] {        
        if (use_thread_affinity)
        {
            std::bitset<8> new_mask(std::string("00000100"));
            int affinity = static_cast<int>(new_mask.to_ulong());
            cpuh::cpu_affinity_util::set_thread_affinity(affinity);
        }
        system_clock::time_point fini = system_clock::now() + minutes(1);
        std::cout << "thread " << cpuh::cpu_affinity_util::get_thread_id() 
            << " spinning with affinity " << cpuh::cpu_affinity_util::get_thread_affinity() << '\n';
        while (system_clock::now() < fini) std::this_thread::yield();
        std::cout << "thread " << cpuh::cpu_affinity_util::get_thread_id() << " stopped\n";
    });

    std::thread t2([use_thread_affinity] {
        std::this_thread::sleep_for(seconds(5));
        if (use_thread_affinity) 
        {
            std::bitset<8> new_mask(std::string("00001000"));
            int affinity = static_cast<int>(new_mask.to_ulong());
            cpuh::cpu_affinity_util::set_thread_affinity(affinity);
        }
        system_clock::time_point fini = system_clock::now() + minutes(1);
        std::cout << "thread " << cpuh::cpu_affinity_util::get_thread_id() 
            << " spinning with affinity " << cpuh::cpu_affinity_util::get_thread_affinity() << '\n';
        while (system_clock::now() < fini) std::this_thread::yield();
        std::cout << "thread " << cpuh::cpu_affinity_util::get_thread_id() << " stopped\n";
    });

    t1.join();
    t2.join();

    std::cout << "-------------------------------------\n";    
}

void test_2()
{
    test_2(true);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    test_2(false);
}

void test_1()
{
    std::cout << "main thread:\n";
    std::cout << "process id = " << cpuh::cpu_affinity_util::get_process_id() << "\n";
    std::cout << "thread id = " << cpuh::cpu_affinity_util::get_thread_id() << "\n";
    std::cout << "# of processors = " << cpuh::cpu_affinity_util::get_number_of_processors() << "\n";
    std::cout << "processor id = " << cpuh::cpu_affinity_util::get_processor() << "\n";
    std::cout << "process affinity = " << cpuh::cpu_affinity_util::get_process_affinity() << "\n";
    std::cout << "thread affinity = " << cpuh::cpu_affinity_util::get_thread_affinity() << "\n";

    std::thread t([] {
        std::cout << "\nbefore change thread affinity:\n";
        std::cout << "process id = " << cpuh::cpu_affinity_util::get_process_id() << "\n";
        std::cout << "thread id = " << cpuh::cpu_affinity_util::get_thread_id() << "\n";
        std::cout << "# of processors = " << cpuh::cpu_affinity_util::get_number_of_processors() << "\n";
        std::cout << "processor id = " << cpuh::cpu_affinity_util::get_processor() << "\n";
        std::cout << "process affinity = " << cpuh::cpu_affinity_util::get_process_affinity() << "\n";
        std::cout << "thread affinity = " << cpuh::cpu_affinity_util::get_thread_affinity() << "\n";

        std::bitset<64> mask(cpuh::cpu_affinity_util::get_thread_affinity());
        std::cout << "thread affinity mask = " << mask << "\n";

        std::bitset<8> new_mask(std::string("00000010"));
        int affinity = static_cast<int>(new_mask.to_ulong());
        cpuh::cpu_affinity_util::set_thread_affinity(affinity);

        std::cout << "\nafter change thread affinity:\n";

        std::cout << "process id = " << cpuh::cpu_affinity_util::get_process_id() << "\n";
        std::cout << "thread id = " << cpuh::cpu_affinity_util::get_thread_id() << "\n";
        std::cout << "# of processors = " << cpuh::cpu_affinity_util::get_number_of_processors() << "\n";
        std::cout << "processor id = " << cpuh::cpu_affinity_util::get_processor() << "\n";
        std::cout << "process affinity = " << cpuh::cpu_affinity_util::get_process_affinity() << "\n";
        std::cout << "thread affinity = " << cpuh::cpu_affinity_util::get_thread_affinity() << "\n";

        mask = cpuh::cpu_affinity_util::get_thread_affinity();
        std::cout << "thread affinity mask = " << mask << "\n";
    });
    t.join();
}

int main(int argc, char* argv[]) 
{
    test_1();
    test_2();
    return 0;
}