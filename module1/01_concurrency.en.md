<!-- .slide: data-background="#111111"-->
# Concurrency

___

## What is concurrency?

* <!-- .element: class="fragment fade-in" --> Two processes are concurrent if one of them starts before the end of the other

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Concurrency is the process of performing two or more activities simultaneously

___

## Concurrency in computer systems

* <!-- .element: class="fragment fade-in" --> Multiprocessor/multicore computers supporting multiple applications simultaneously
* <!-- .element: class="fragment fade-in" --> Simulation of concurrency in single-processor systems using the mechanism of task switching

<img class="plain fragment fade-in" data-src="img/02.png" src="img/02.png" alt="'Concurency in action', Anthony Williams 2019" title="'Concurency in action', Anthony Williams 2019">

Note: Show `top`

___

## Models of concurrency

| &nbsp;        | Threads                                                                              | Processes                                                                                                |
| :------------ | :----------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------- |
| Stack         | Separate <!-- .element: class="fragment fade-in" --> | Separate <!-- .element: class="fragment fade-in" --> |
| Heap          | Common <!-- .element: class="fragment fade-in" -->   | Separate <!-- .element: class="fragment fade-in" --> |
| Creation      | Fast <!-- .element: class="fragment fade-in" -->     | Slow <!-- .element: class="fragment fade-in" --> |
| Communication | Fast (heap) - pointers, data structures  <!-- .element: class="fragment fade-in" --> | Slower (IPC) - sockets, files, signals, pipes, shared memory <!-- .element: class="fragment fade-in" --> |

___

## Process

* <!-- .element: class="fragment fade-in" --> The process is used to organize the execution of the program. One program consists of one or more processes. Thus, the process is the entire context necessary to execute the program.
* <!-- .element: class="fragment fade-in" --> As a result of the process execution, the data segment, stack segment, and processor registers state can be changed.
* <!-- .element: class="fragment fade-in" --> When a process is executed, the operating system allocates the necessary resources to the process (memory, CPU time, etc.).
* <!-- .element: class="fragment fade-in" --> Synchronization, the way processes are handled, etc. is controlled by the operating system.
* <!-- .element: class="fragment fade-in" --> There are one or more threads within each process.

___

## Thread

* <!-- .element: class="fragment fade-in" --> Threads of the same process share most of the address space (code and data segment, open files, etc.).
* <!-- .element: class="fragment fade-in" --> Switching thread context is relatively fast and doesn't burden operating system.
* <!-- .element: class="fragment fade-in" --> Creating a thread requires less resources to run and is faster than creating a process.
* <!-- .element: class="fragment fade-in" --> Easy (but also dangerous) communication between threads within one process.
* <!-- .element: class="fragment fade-in" --> Each thread has a separate stack (function return address and local variables).
