# --------------------------------------------------------------------------------------------------
#
# CPU THREAD AFFINITY HELPER
# ==========================
#
# This file:
#    cpu_affinity_util test driver for Windows dynamic library
#
# Authors: 
#    Fabio Galuppo
#
# Releases: July 29 2021 - v0.1
#    v0.1 - July 2021 - initial release
#
# The MIT License (MIT)
# Copyright © 2021 Fabio Galuppo
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the “Software”), to deal in the Software without
# restriction, including without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# --------------------------------------------------------------------------------------------------

from ctypes import *
from threading import *
import time

dll = cdll.LoadLibrary("cpu_affinity_util.dll")

def test1():
    print("process id =", dll.cpuh_get_process_id())
    print("thread id =", dll.cpuh_get_thread_id(), "Python thread id =", current_thread().ident)
    print("# of processors =", dll.cpuh_get_number_of_processors())
    print("processor id =", dll.cpuh_get_process_affinity())
    print("process affinity =", dll.cpuh_get_processor())
    print("thread affinity =", dll.cpuh_get_thread_affinity())

def threadfunc(use_thread_affinity, timeslice_in_seconds, run):
    if use_thread_affinity == True: 
        affinity = 4
        dll.cpuh_set_thread_affinity(affinity)
    fini = time.time_ns() + (timeslice_in_seconds * 1000000000)
    print("thread", dll.cpuh_get_thread_id(), "spinning with affinity", dll.cpuh_get_thread_affinity())
    while time.time_ns() < fini:
        run()        
    print("thread", dll.cpuh_get_thread_id(), "stopped")

def test2():
    def test(use_thread_affinity):
        print("-------------------------------------")
        print("set thread affinity?", use_thread_affinity)
        timeslice_in_seconds = 25
        t = Thread(target = threadfunc, args = (use_thread_affinity, timeslice_in_seconds, lambda: time.sleep(0)))
        t.start()
        t.join()
        print("-------------------------------------")
    test(use_thread_affinity=True)
    time.sleep(5)
    test(use_thread_affinity=False)

test1()
test2()