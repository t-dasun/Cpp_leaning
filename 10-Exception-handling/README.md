# C++ Exception Handling - Short Notes

Exception handling is used to handle runtime errors without crashing the program immediately. In C++, the main keywords are `try`, `catch`, and `throw`.

---

## Table of Contents

1. [Basic Idea](#basic-idea)
2. [Throwing Exceptions](#throwing-exceptions)
3. [Catching Exceptions](#catching-exceptions)
4. [Multiple Catch Blocks](#multiple-catch-blocks)
5. [Catch by Reference](#catch-by-reference)
6. [Common Standard Exceptions](#common-standard-exceptions)
7. [Catch-All Handler](#catch-all-handler)
8. [Stack Unwinding](#stack-unwinding)
9. [Rethrowing Exceptions](#rethrowing-exceptions)
10. [Exceptions in Destructors](#exceptions-in-destructors)
11. [`noexcept`](#noexcept)
12. [Best Practices](#best-practices)

---

## Basic Idea

Normal program flow:

```cpp
int result = Divide(10, 2);
std::cout << result << "\n";
```

If something goes wrong, an exception can be thrown:

```cpp
throw std::runtime_error("Something failed");
```

Then another part of the program can catch and handle it:

```cpp
try {
    // code that may throw
}
catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
}
```

---

## Throwing Exceptions

Use `throw` to report an error.

```cpp
#include <stdexcept>

int Divide(int a, int b)
{
    if (b == 0) {
        throw std::runtime_error("Cannot divide by zero");
    }

    return a / b;
}
```

`std::runtime_error` is a standard exception class used for runtime errors.

---

## Catching Exceptions

Use `try` for code that may throw an exception. Use `catch` to handle the exception.

```cpp
#include <iostream>
#include <stdexcept>

int main()
{
    try {
        int result = Divide(10, 0);
        std::cout << result << "\n";
    }
    catch (const std::runtime_error& ex) {
        std::cout << "Error: " << ex.what() << "\n";
    }

    return 0;
}
```

`ex.what()` returns the error message stored inside the exception.

---

## Multiple Catch Blocks

You can have multiple `catch` blocks for different exception types.

```cpp
try {
    // code that may throw
}
catch (const std::out_of_range& ex) {
    std::cout << "Out of range: " << ex.what() << "\n";
}
catch (const std::runtime_error& ex) {
    std::cout << "Runtime error: " << ex.what() << "\n";
}
catch (const std::exception& ex) {
    std::cout << "Standard exception: " << ex.what() << "\n";
}
```

Order matters. Catch more specific exceptions first, then catch the base class `std::exception` later.

Most standard exception classes inherit from `std::exception`.

---

## Catch by Reference

Prefer catching exceptions by const reference.

```cpp
catch (const std::runtime_error& ex) {
    std::cout << ex.what() << "\n";
}
```

Why?

- Avoids copying the exception object.
- Preserves the real exception type.
- Prevents accidental modification.

Avoid this:

```cpp
catch (std::runtime_error ex) {
    // creates a copy
}
```

---

## Common Standard Exceptions

Include `<stdexcept>` for many common exception types.

| Exception | Meaning |
|-----------|---------|
| `std::runtime_error` | General runtime error |
| `std::logic_error` | Error caused by wrong program logic |
| `std::out_of_range` | Index or value is outside valid range |
| `std::invalid_argument` | Function received invalid argument |
| `std::length_error` | Object would become too large |
| `std::bad_alloc` | Dynamic memory allocation failed |

Example:

```cpp
throw std::out_of_range("Invalid index");
```

If `new` fails to allocate memory, it throws `std::bad_alloc`.

```cpp
try {
    int* data = new int[1000000000000];
    delete[] data;
}
catch (const std::bad_alloc& ex) {
    std::cout << "Memory allocation failed: " << ex.what() << "\n";
}
```

In modern C++, prefer smart pointers and containers instead of raw `new`.

---

## Catch-All Handler

`catch (...)` catches any exception type.

```cpp
try {
    // code that may throw anything
}
catch (...) {
    std::cout << "Unknown error happened\n";
}
```

This can catch exceptions that are not derived from `std::exception`, but it cannot directly show an error message.

Use it only when you really need a final safety net.

---

## Stack Unwinding

When an exception is thrown, C++ leaves the current function and searches for a matching `catch` block. During this process, local objects are destroyed automatically.

This is called stack unwinding.

```cpp
class File
{
public:
    File()
    {
        std::cout << "Open file\n";
    }

    ~File()
    {
        std::cout << "Close file\n";
    }
};

void Process()
{
    File file;
    throw std::runtime_error("Process failed");
}
```

When `Process()` throws, the local object `file` is destroyed and its destructor runs.

Important:

- Local stack objects are destroyed automatically.
- Raw heap memory from `new` is not automatically deleted.
- Smart pointers clean up automatically.

Better:

```cpp
#include <memory>

void Process()
{
    auto data = std::make_unique<int[]>(100);
    throw std::runtime_error("Process failed");
}
```

`std::unique_ptr` will release the memory during stack unwinding.

---

## Rethrowing Exceptions

Sometimes a catch block handles part of the error and then sends it upward again.

Use plain `throw;` to rethrow the same exception.

```cpp
try {
    DangerousFunction();
}
catch (const std::exception& ex) {
    std::cout << "Logging error: " << ex.what() << "\n";
    throw;
}
```

Use `throw;` only inside a `catch` block.

If you want to throw a new exception, create a new one:

```cpp
catch (const std::exception& ex) {
    throw std::runtime_error("DangerousFunction failed");
}
```

Note: assigning a new exception object to `ex` does not replace the original exception. Also, `ex` should usually be `const`.

---

## Exceptions in Destructors

Avoid throwing exceptions from destructors.

```cpp
class Resource
{
public:
    ~Resource()
    {
        // Do not throw from here
    }
};
```

Why?

If another exception is already active during stack unwinding and a destructor throws another exception, the program can call `std::terminate()` and stop immediately.

Destructors should clean up safely and not fail with exceptions.

---

## `noexcept`

`noexcept` tells the compiler that a function should not throw exceptions.

```cpp
int Func(int a) noexcept
{
    return a * 2;
}
```

These are also valid:

```cpp
int Func1(int a) noexcept(true);   // does not throw
int Func2(int a) noexcept(false);  // may throw
```

If a `noexcept` function throws an exception, the program calls `std::terminate()`.

Use `noexcept` only when you are sure the function will not throw.

### `noexcept` and Move Constructors

Move constructors should often be marked `noexcept`, especially when the class is used inside standard containers like `std::vector`.

```cpp
class Buffer
{
public:
    Buffer(Buffer&& other) noexcept
    {
        // move resource from other
    }
};
```

Why this matters:

- Containers may move elements during reallocation.
- If the move constructor is not `noexcept`, containers may use copying instead.
- Copying can be slower than moving.
- `noexcept` helps containers keep strong exception safety.

---

## Best Practices

- Catch exceptions by `const` reference.
- Catch specific exceptions before `std::exception`.
- Use `catch (...)` only as a final safety net.
- Do not throw exceptions from destructors.
- Use smart pointers and RAII to avoid memory leaks during exceptions.
- Use `noexcept` only for functions that really should not throw.
- Mark move constructors and move assignment operators `noexcept` when possible.
- Do not use exceptions for normal control flow.
- Use clear error messages in thrown exceptions.

---

## Quick Summary

```cpp
try {
    throw std::runtime_error("failed");
}
catch (const std::runtime_error& ex) {
    std::cout << ex.what() << "\n";
}
catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
}
catch (...) {
    std::cout << "Unknown error\n";
}
```

```cpp
void SafeFunction() noexcept
{
    // should not throw
}
```

Main idea:

- `throw` sends an error.
- `try` marks code that may fail.
- `catch` handles the error.
- stack unwinding destroys local objects.
- `noexcept` means the function promises not to throw.

