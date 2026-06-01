# C++ Concurrency - Short Notes

Concurrency means running multiple tasks during the same program execution. In C++, the common tools are `std::thread`, `std::mutex`, `std::future`, `std::async`, `std::promise`, and `std::packaged_task`.

---

## Table of Contents

1. [Compile with Threads](#compile-with-threads)
2. [Creating Threads](#creating-threads)
3. [Join vs Detach](#join-vs-detach)
4. [Passing Arguments to Threads](#passing-arguments-to-threads)
5. [Getting Return Values from Threads](#getting-return-values-from-threads)
6. [Mutex and Lock Guard](#mutex-and-lock-guard)
7. [Thread ID, Sleep, and Hardware Threads](#thread-id-sleep-and-hardware-threads)
8. [Thread Naming](#thread-naming)
9. [std::async](#stdasync)
10. [Promise and Future](#promise-and-future)
11. [Best Practices](#best-practices)

---

## Compile with Threads

To use C++ threads, include `<thread>` and compile with pthread support.

```bash
g++ main.cpp -o app -std=c++17 -pthread
```

Important headers:

```cpp
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <iostream>
```

---

## Creating Threads

A thread can run a normal function.

```cpp
#include <iostream>
#include <thread>

void Work()
{
    std::cout << "Work running in another thread\n";
}

int main()
{
    std::thread th(Work);

    th.join();

    return 0;
}
```

`std::thread th(Work);` starts a new thread and runs the `Work` function.

---

## Join vs Detach

### `join()`

`join()` makes the main thread wait until the worker thread finishes.

```cpp
std::thread th(Work);

if (th.joinable()) {
    th.join();
}
```

Use `joinable()` before joining because a thread can only be joined if it owns a running thread.

### `detach()`

`detach()` separates the new thread from the main thread.

```cpp
std::thread th(Work);
th.detach();
```

After detach, the main thread will not wait for that thread. The detached thread continues by itself.

Important: after a thread is detached, you cannot call `join()` on it.

---

## Passing Arguments to Threads

Arguments can be passed after the function name.

```cpp
void Print(int value, std::string name)
{
    std::cout << name << ": " << value << "\n";
}

int main()
{
    std::thread th(Print, 10, "Task");
    th.join();
}
```

### Passing by Reference

By default, arguments are copied into the thread. To pass by reference, use `std::ref()`.

```cpp
#include <functional>

void Increment(int& value)
{
    value++;
}

int main()
{
    int number = 5;

    std::thread th(Increment, std::ref(number));
    th.join();

    std::cout << number << "\n"; // 6
}
```

For const reference, use `std::cref()`.

```cpp
void PrintName(const std::string& name)
{
    std::cout << name << "\n";
}

std::string name = "Dasun";
std::thread th(PrintName, std::cref(name));
```

---

## Getting Return Values from Threads

`std::thread` itself does not directly return a value. To get a result from a thread, use `std::future` with `std::packaged_task`, `std::promise`, or `std::async`.

### Using `std::packaged_task`

```cpp
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int ComputeSum(const std::vector<int>& data)
{
    int sum = 0;

    for (int value : data) {
        sum += value;
    }

    return sum;
}

int main()
{
    std::vector<int> data {1, 2, 3, 4, 5};

    std::packaged_task<int(const std::vector<int>&)> task(ComputeSum);
    std::future<int> result = task.get_future(); // need to get future be9 move task, task can't copy

    std::thread th(std::move(task), std::cref(data));

    int sum = result.get(); // waits until the result is ready

    th.join();

    std::cout << "Sum: " << sum << "\n";
}
```

Key points:

- `std::packaged_task` wraps a function.
- `get_future()` gives a `std::future`.
- `future.get()` waits until the thread produces the result.
- `std::packaged_task` must be moved into the thread using `std::move()`.

---

## Mutex and Lock Guard

When multiple threads access the same data, a race condition can happen. A mutex protects shared data so only one thread can access it at a time.

### Manual Lock and Unlock

```cpp
#include <mutex>

std::mutex globalMutex;
int counter = 0;

void Increment()
{
    globalMutex.lock();
    counter++;
    globalMutex.unlock();
}
```

This works, but it is dangerous. If an error happens before `unlock()`, the mutex may stay locked forever. That can cause a deadlock.

### Better: `std::lock_guard`

```cpp
#include <mutex>

std::mutex globalMutex;
int counter = 0;

void Increment()
{
    std::lock_guard<std::mutex> lock(globalMutex);
    counter++;
}
```

`std::lock_guard` automatically locks the mutex when it is created and unlocks it when it goes out of scope.

This style is safer and cleaner.

---

## Thread ID, Sleep, and Hardware Threads

### Get Thread ID

```cpp
std::thread t1(Work);

auto id = t1.get_id();
std::cout << "Thread ID: " << id << "\n";

t1.join();
```

Inside the running thread, use:

```cpp
std::cout << std::this_thread::get_id() << "\n";
```

### Sleep a Thread

```cpp
#include <chrono>
#include <thread>

std::this_thread::sleep_for(std::chrono::seconds(1));
```

With C++14 chrono literals:

```cpp
using namespace std::chrono_literals;

std::this_thread::sleep_for(1s);
```

### Hardware Concurrency

```cpp
unsigned int cores = std::thread::hardware_concurrency();
std::cout << "Hardware threads: " << cores << "\n";
```

`hardware_concurrency()` gives a hint about how many hardware threads the system can run at the same time.

Usually, creating more CPU-heavy threads than hardware threads is not efficient because the CPU must keep switching between them.

---

## Thread Naming

C++ standard library does not provide a portable way to name threads. You can use native OS APIs through `native_handle()`.

### Linux

```cpp
#include <pthread.h>

std::thread t1(Work);
pthread_setname_np(t1.native_handle(), "worker_1");
t1.join();
```

### Windows

```cpp
SetThreadDescription(
    t1.native_handle(),
    L"worker_1"
);
```

`native_handle()` gives access to the operating system thread handle behind the C++ thread.

---

## `std::async`

`std::async` is a higher-level way to run a task asynchronously and get a result using `std::future`.

```cpp
#include <future>
#include <iostream>

int Sum(int count)
{
    int total = 0;

    for (int i = 1; i <= count; i++) {
        total += i;
    }

    return total;
}

int main()
{
    std::future<int> result = std::async(std::launch::async, Sum, 100);

    int value = result.get();

    std::cout << value << "\n";
}
```

### Launch Policies

```cpp
std::async(std::launch::async, Sum, 100);
```

Runs the function asynchronously, usually in a new thread.

```cpp
std::async(std::launch::deferred, Sum, 100);
```

Does not run immediately. It runs only when `.get()` or `.wait()` is called.

If no launch policy is specified, C++ can choose either async or deferred behavior.

```cpp
std::future<int> result = std::async(Sum, 100);
```

For predictable real asynchronous execution, use `std::launch::async`.

### Waiting for Result

```cpp
using namespace std::chrono_literals;

auto status = result.wait_for(1s);

if (status == std::future_status::ready) {
    std::cout << result.get() << "\n";
}
else if (status == std::future_status::timeout) {
    std::cout << "Result is not ready yet\n";
}
else if (status == std::future_status::deferred) {
    std::cout << "Task is deferred\n";
}
```

Common future functions:

| Function | Meaning |
|----------|---------|
| `get()` | Waits and returns the value |
| `wait()` | Waits but does not return the value |
| `wait_for()` | Waits for a time duration |
| `wait_until()` | Waits until a specific time point |
| `valid()` | Checks whether the future has a shared state |

Important: call `get()` only once on a normal `std::future`.

---

## Promise and Future

`std::promise` and `std::future` are useful when one thread wants to send a value or signal to another thread.

Example: worker thread calculates a value and sends it to the main thread.

```cpp
#include <future>
#include <iostream>
#include <thread>

void Worker(std::promise<int> promise)
{
    int result = 42;
    promise.set_value(result);
}

int main()
{
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread th(Worker, std::move(promise));

    int value = future.get(); // waits until set_value() is called

    th.join();

    std::cout << "Value: " << value << "\n";
}
```

Use `std::promise` when:

- The thread function does not naturally return a value.
- You want to send a result later.
- You want to notify another thread that something is ready.

---

std::shared_future (The Broadcast Receiver)
​Purpose: Allows multiple threads to wait for and read the exact same result simultaneously.
​Copyable: Unlike a standard std::future (which is move-only and single-use), a std::shared_future can be copied freely to as many threads as you need.
​How .get() works: Calling .get() returns a const reference to the data. It does not destroy the data, meaning multiple threads can read it safely without a mutex.


| Task | Tool to Use |
| :--- | :--- |
| **Send data/signal from one thread** | `std::promise` |
| **Receive data in exactly ONE thread** | `std::future` |
| **Broadcast data/signal to MULTIPLE threads** | `std::shared_future` |
| **Modify shared data *after* it's been sent** | Pass a pointer to a struct containing a `std::mutex` *through* the future. |
---

## Best Practices

- Prefer `std::lock_guard` instead of manual `lock()` and `unlock()`.
- Always call `join()` or `detach()` before a `std::thread` object is destroyed.
- Use `joinable()` before calling `join()`.
- Use `std::ref()` when a thread function needs a real reference.
- Use `std::launch::async` with `std::async` when you really need another thread.
- Keep shared data small and protected with a mutex.
- Avoid holding a lock longer than needed.
- Do not create too many CPU-heavy threads. Use `hardware_concurrency()` as a guide.
- Prefer high-level tools like `std::async` when you only need to run a task and get a result.

---

## Quick Summary

```cpp
std::thread th(func);                  // start thread
th.join();                             // wait for thread
th.detach();                           // separate thread
std::thread th(func, arg1, arg2);       // pass arguments
std::thread th(func, std::ref(value));  // pass reference
std::mutex mtx;                        // protect shared data
std::lock_guard<std::mutex> lock(mtx);  // safe locking
std::this_thread::sleep_for(1s);        // sleep current thread
std::async(std::launch::async, func);   // async task with result
```

