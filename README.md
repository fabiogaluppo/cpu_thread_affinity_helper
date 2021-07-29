# CPU THREAD AFFINITY HELPER

C++ header-only utility for query or change **thread affinity** in a process

### Compile instructions

- Compile test driver for C++ with Visual C++
```str
cl /O2 /EHsc /std:c++14 cpu_affinity_util.t.cpp
```

- Compile test driver for C++ with g++ 10
```str
g++-10 -O3 -std=c++14 -pthread cpu_affinity_util.t.cpp
```

- Build a .dll for Windows with Visual C++
```str
cl /O2 /EHsc /std:c++14 /DCPU_AFFINITY_UTIL_EXPORTS cpu_affinity_util_exports.cpp /link /DLL /out:cpu_affinity_util.dll
```

- Compile test driver for Windows .dll with Visual C++
```str
cl /O2 /EHsc /std:c++14 cpu_affinity_util.windows.dll.t.cpp
```

- Build a .so for Linux with g++ 10
```str
g++-10 -Wall -fPIC -c -O3 -std=c++14 -pthread cpu_affinity_util_exports.cpp 
g++-10 -shared -Wl,-soname,libcpu_affinity_util.so -o libcpu_affinity_util.so cpu_affinity_util_exports.o
```

- Compile test driver for Linux .so with g++ 10
```str
g++-10 -O3 -std=c++14 -pthread cpu_affinity_util.linux.so.t.cpp -ldl
```

### Python example 

- Consuming Windows .dll in Python for query thread, process and processor info

```python
from ctypes import *
dll = cdll.LoadLibrary("cpu_affinity_util.dll");
print("process id =", dll.cpuh_get_process_id())
print("thread id =", dll.cpuh_get_thread_id())
print("# of processors =", dll.cpuh_get_number_of_processors())
print("processor id =", dll.cpuh_get_process_affinity())
print("process affinity =", dll.cpuh_get_processor())
print("thread affinity =", dll.cpuh_get_thread_affinity())
```

### Linux screenshot

![CPU THREAD AFFINITY HELPER in action](/images/cpuh.png "CPU THREAD AFFINITY HELPER in action")

## License

MIT

**Free Software, Hell Yeah!**