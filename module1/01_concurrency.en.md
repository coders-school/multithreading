<!-- .slide: data-background="#111111"-->
# Concurrency

___

## What is concurrency?

* <!-- .element: class="fragment fade-in" --> Two processes are concurrent if one of them starts before the end of the other.

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Concurrency is the process of performing two or more activities simultaneously.

___

## Concurrency in computer systems

* <!-- .element: class="fragment fade-in" --> "Simulation" of concurrency in single-processor systems using the mechanism of task switching,
* <!-- .element: class="fragment fade-in" --> Multiprocessor/multicore computers/servers supporting multiple applications simultaneously.

<img class="plain fragment fade-in" data-src="img/02.png" src="img/02.png" alt="'Concurency in action', Anthony Williams 2019" title="'Concurency in action', Anthony Williams 2019">

___

## Models of concurrency

* <!-- .element: class="fragment fade-in" --> Concurrency with multiple processes:
  * <!-- .element: class="fragment fade-in" --> Separate processes (communication via signals, files, pipes, sockets, etc.),
  * <!-- .element: class="fragment fade-in" --> Separate memory space, security of multithreaded processing,
  * <!-- .element: class="fragment fade-in" --> A long and complicated process of starting a new process.
* <!-- .element: class="fragment fade-in" --> Concurrency with multiple threads:
  * <!-- .element: class="fragment fade-in" --> Running multiple threads within one process sharing memory (shared address space),
  * <!-- .element: class="fragment fade-in" --> You must ensure the security of access to the shared memory yourself,
  * <!-- .element: class="fragment fade-in" --> Starting a thread is faster than starting a new process,
  * <!-- .element: class="fragment fade-in" --> Simpler and faster methods of communication between threads within a process

___

## When to use concurrency?

* <!-- .element: class="fragment fade-in" --> Distribution of tasks (e.g. Blu-ray player),
* <!-- .element: class="fragment fade-in" --> Performance:
  * <!-- .element: class="fragment fade-in" --> Parallel tasks (division of a task into parts),
  * <!-- .element: class="fragment fade-in" --> Data parallelization (performing the same tasks on different pieces of data)

___

## When not to use concurrency?

* <!-- .element: class="fragment fade-in" --> When too many threads can reduce performance instead of increasing it (cost of starting a thread, cost of resources - each thread typically uses 8MB of memory),
* <!-- .element: class="fragment fade-in" --> When the performance gain is not proportional to the effort involved and the complexity of the code (code maintenance cost is equally important).

___

## Process and thread

* <!-- .element: class="fragment fade-in" --> The process is used to organize the execution of the program. One program consists of one or more processes. Thus, the process is the entire context necessary to execute the program.
* <!-- .element: class="fragment fade-in" --> As a result of the process execution, the data segment, stack segment, and processor registers state can be changed.
* <!-- .element: class="fragment fade-in" --> When a process is executed, the operating system allocates the necessary resources to the process (memory, CPU time, etc.).
* <!-- .element: class="fragment fade-in" --> Synchronization, the way processes are handled, etc. is controlled by the operating system.
* <!-- .element: class="fragment fade-in" --> There are one or more threads within each process.

___

## Process and thread

* <!-- .element: class="fragment fade-in" --> Threads of the same process share most of the address space (code and data segment, open files, etc.).
* <!-- .element: class="fragment fade-in" --> Switching thread context is relatively fast and doesn't burden operating system.
* <!-- .element: class="fragment fade-in" --> Creating a thread requires less resources to run and is faster than creating a process.
* <!-- .element: class="fragment fade-in" --> Easy (but also dangerous) communication between threads within one process.
* <!-- .element: class="fragment fade-in" --> Each thread has a separate stack (function return address and local variables).
