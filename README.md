# Multithreaded Producer-Consumer in C

A multithreaded implementation of the classic Producer-Consumer problem in C using POSIX threads. This project compares three synchronization techniques:

- Mutex + Condition Variables
- Semaphores
- Spinlock (implemented with C11 `atomic_flag`)

The program measures execution time and throughput for each synchronization method.

---

## Features

- Bounded circular buffer
- Multiple producer threads
- Multiple consumer threads
- Three synchronization implementations
- Execution time benchmarking
- Throughput comparison
- Modular project structure

---

## Project Structure

```
multithreadedProducerConsumer/
│
├── include/
│   ├── common.h
│   └── boundedBuffer.h
│
├── src/
│   ├── main.c
│   ├── mutexBuffer.c
│   ├── semaphoreBuffer.c
│   └── spinlockBuffer.c
│
├── Makefile
└── README.md
```

---

## How It Works

A bounded buffer is shared between multiple producer and consumer threads.

- Producers generate integers and insert them into the shared buffer.
- Consumers remove integers from the buffer.
- Synchronization ensures multiple threads never corrupt the shared data.

Each synchronization method solves the same problem using different primitives.

---

## Synchronization Methods

### Mutex + Condition Variables

Uses:

- `pthread_mutex_t`
- `pthread_cond_t`

The mutex protects the shared buffer.

Condition variables allow:

- producers to sleep when the buffer is full
- consumers to sleep when the buffer is empty

This avoids wasting CPU time while waiting.

---

### Semaphores

Uses:

- POSIX semaphores
- mutex for protecting the buffer

Two semaphores are maintained:

- `emptySlots`
- `filledSlots`

The semaphores keep track of available buffer space and available items.

---

### Spinlock

Uses:

- C11 `atomic_flag`

Instead of sleeping, a thread repeatedly checks whether the lock becomes available.

This can perform well when critical sections are very short, but can waste CPU time if threads must wait for long periods.

---

## Building

```bash
make
```

Run:

```bash
./producerConsumer
```

Clean:

```bash
make clean
```

---

## Example Output

```
=========================================
Producer Consumer Benchmark
=========================================

Buffer size: 8
Producer threads: 4
Consumer threads: 4
Items per producer: 100000

Mutex + Condition Variables
Execution time: 1.160962 seconds
Total items processed: 400000
Throughput: 344541.85 items/second

Semaphores
Execution time: 1.417661 seconds
Total items processed: 400000
Throughput: 282154.90 items/second

Spinlock
Execution time: 0.564463 seconds
Total items processed: 400000
Throughput: 708638.12 items/second
```

Performance will vary depending on hardware, operating system, and thread scheduling.

---

## What I Learned

- Creating and managing threads with POSIX pthreads
- Implementing a bounded circular buffer
- Preventing race conditions using synchronization primitives
- Using mutexes, condition variables, semaphores, and spinlocks
- Measuring execution time and throughput
- Organizing a multithreaded C project into multiple source files