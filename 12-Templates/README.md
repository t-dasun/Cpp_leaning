# C++ Templates - Short Notes

Templates allow C++ code to work with many types while still being checked and generated at compile time. They are used heavily in the standard library, Boost, and many high-performance C++ libraries.

---

## Table of Contents

1. [Why Templates?](#why-templates)
2. [Function Templates](#function-templates)
3. [Template Type Deduction](#template-type-deduction)
4. [Explicit Instantiation](#explicit-instantiation)
5. [Template Specialization](#template-specialization)
6. [Non-Type Template Arguments](#non-type-template-arguments)
7. [Perfect Forwarding](#perfect-forwarding)
8. [Variadic Templates](#variadic-templates)
9. [Class Templates](#class-templates)
10. [Type Aliases](#type-aliases)
11. [Type Traits](#type-traits)
12. [Best Practices](#best-practices)

---

## Why Templates?

Templates provide compile-time polymorphism.

Main benefits:

- Write one function or class for many types.
- Compiler generates the needed versions.
- No runtime virtual function overhead.
- Useful for generic containers, algorithms, and high-performance code.

Example:

```cpp
template <typename T>
T Add(T x, T y)
{
    return x + y;
}
```

When you call this with `int`, the compiler creates an `int` version. When you call it with `float`, the compiler creates a `float` version.

---

## Function Templates

A function template is a blueprint for creating functions.

```cpp
template <typename T>
T Add(T x, T y)
{
    return x + y;
}

int main()
{
    auto a = Add(1, 2);       // T is int
    auto b = Add(1.5f, 2.5f); // T is float
}
```

If the function is never called, the compiler does not generate a real function from the template.

`typename` and `class` mean the same thing in template parameters:

```cpp
template <class T>
T Add(T x, T y)
{
    return x + y;
}
```

---

## Template Type Deduction

The compiler tries to find the template type from the function arguments.

```cpp
Add(1, 2);       // OK, T is int
Add(1.5, 2.5);   // OK, T is double
```

This does not work:

```cpp
Add(3, 3.5);     // Error: first argument is int, second is double
```

Fix by explicitly giving the template type:

```cpp
Add<double>(3, 3.5); // T is double
```

Or create a template with two types:

```cpp
template <typename T1, typename T2>
auto Add(T1 x, T2 y)
{
    return x + y;
}

auto result = Add(3, 3.5); // returns double
```

---

## Explicit Instantiation

Usually templates are defined in header files because the compiler must see the full definition when it creates a specific version.

Example header:

```cpp
template <typename T>
T Add(T x, T y)
{
    return x + y;
}
```

If you want to put template code in a `.cpp` file, you must explicitly instantiate the types you want to support.

```cpp
template int Add<int>(int, int);
template double Add<double>(double, double);
template char Add<char>(char, char);
```

This tells the compiler:

- Generate `Add<int>`.
- Generate `Add<double>`.
- Generate `Add<char>`.

You can also store a generated function in a function pointer:

```cpp
int (*pfn)(int, int) = Add;
int result = pfn(2, 3);
```

---

## Template Specialization

Template specialization means creating a special version for a specific type.

Normal template:

```cpp
template <typename T>
T Max(T x, T y)
{
    return x > y ? x : y;
}
```

This works for numbers, but for C-style strings it compares addresses, not text.

```cpp
const char* a = "Apple";
const char* b = "Banana";

Max(a, b); // compares pointer addresses, not string values
```

Specialized version for `const char*`:

```cpp
#include <cstring>

template <>
const char* Max<const char*>(const char* x, const char* y)
{
    return std::strcmp(x, y) > 0 ? x : y;
}
```

Now `Max()` compares the actual string content.

Specialization declaration can be placed in a header:

```cpp
template <>
const char* Max<const char*>(const char* x, const char* y);
```

Specialization definition can be placed in a `.cpp` file.

---

## Non-Type Template Arguments

Templates can also accept values, not only types. These values must be known at compile time.

Example: array size as a template argument.

```cpp
template <typename T, int size>
T Sum(T (&arr)[size])
{
    T sum{};

    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum;
}

int main()
{
    int arr[] {2, 3, 4, 5};
    int result = Sum(arr); // size is detected at compile time
}
```

Here `size` is not passed as a normal function argument. The compiler knows it from the array type.

Important:

- Non-type template arguments are compile-time constants.
- They can be integers, enums, pointers, references, and some other compile-time values.
- They cannot be changed at runtime.

---

## Perfect Forwarding

Perfect forwarding preserves whether an argument is an lvalue or rvalue.

This is useful when passing constructor arguments to member objects without unnecessary copying.

```cpp
#include <string>
#include <utility>

class Employee
{
private:
    std::string m_Name;
    int m_Id;

public:
    template <typename T1, typename T2>
    Employee(T1&& name, T2&& id)
        : m_Name(std::forward<T1>(name)),
          m_Id(std::forward<T2>(id))
    {
    }
};
```

Usage:

```cpp
Employee emp1 {"Umar", 100}; // rvalues

std::string name = "Dasun";
int id = 200;
Employee emp2 {name, id};    // lvalues
```

Why use `std::forward`?

- If the input is an rvalue, it stays an rvalue.
- If the input is an lvalue, it stays an lvalue.
- Move constructors can be used when possible.
- Extra copies can be avoided.

---

## Variadic Templates

Variadic templates allow a function or class to accept any number of template arguments.

They use a parameter pack.

```cpp
void Print()
{
    std::cout << "\n";
}

template <typename T, typename... Params>
void Print(T&& first, Params&&... args)
{
    std::cout << first;

    if constexpr (sizeof...(args) > 0) {
        std::cout << ", ";
    }

    Print(std::forward<Params>(args)...);
}
```

Usage:

```cpp
Print(1, 2.5, "Hello", 'A');
```

Important syntax:

```cpp
typename... Params    // template parameter pack
Params&&... args      // function parameter pack
sizeof...(args)       // number of function arguments left
sizeof...(Params)     // number of template types left
```

The base function `Print()` stops the recursion.

---

## Class Templates

Class templates create generic classes.

```cpp
template <typename T, int size>
class Stack
{
private:
    T m_Buffer[size];
    int m_Top {-1};

public:
    void Push(const T& value)
    {
        if (m_Top < size - 1) {
            m_Buffer[++m_Top] = value;
        }
    }

    T Pop()
    {
        return m_Buffer[m_Top--];
    }
};
```

Usage:

```cpp
Stack<int, 10> numbers;
numbers.Push(5);
```

Here:

- `T` is the stored type.
- `size` is a compile-time value.

### Class Template Specialization

You can specialize a class template for a specific type.

```cpp
template <>
class Stack<char*>
{
    // special implementation for char*
};
```

You can also specialize only some template arguments. This is called partial specialization.

```cpp
template <typename T>
class Stack<T*, 10>
{
    // special implementation for pointer types with size 10
};
```

---

## Type Aliases

Type aliases give a new readable name to an existing type.

Old style:

```cpp
typedef long long LLONG;

LLONG value {};
```

Modern C++ style:

```cpp
using UINT = unsigned int;

UINT value {};
```

`using` is preferred in modern C++ because it also works nicely with templates.

```cpp
#include <list>
#include <vector>

template <typename T>
using ListVector = std::vector<std::list<T>>;

ListVector<int> data;
```

---

## Type Traits

Type traits allow checking and modifying types at compile time.

Include:

```cpp
#include <type_traits>
```

Examples:

```cpp
std::is_integral<int>::value;        // true
std::is_floating_point<double>::value; // true
std::remove_reference<int&>::type;   // int
```

C++17 provides shorter `_v` and `_t` forms:

```cpp
std::is_integral_v<int>;          // true
std::is_floating_point_v<float>;  // true
std::remove_reference_t<int&>;    // int
```

Type traits are useful when writing templates that should behave differently for different types.

Example with `static_assert`:

```cpp
#include <type_traits>

template <typename T>
T Square(T value)
{
    static_assert(std::is_arithmetic_v<T>, "T must be a number");
    return value * value;
}
```

If `T` is not a number type, compilation stops with the message.

---

## Best Practices

- Put normal template definitions in header files.
- Use explicit instantiation only when you want to limit supported types.
- Use template specialization for special type behavior.
- Prefer `using` over `typedef` in modern C++.
- Use `std::forward` only with forwarding references like `T&&`.
- Use `static_assert` to give clear compile-time errors.
- Keep template error messages readable by using simple names and small functions.
- Avoid overusing templates when a normal function or class is enough.

---

## Quick Summary

```cpp
template <typename T>
T Add(T x, T y)
{
    return x + y;
}

auto a = Add(1, 2);           // int
auto b = Add<double>(3, 3.5); // double
```

```cpp
template <typename T, int size>
T Sum(T (&arr)[size])
{
    T sum {};

    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum;
}
```

Main idea:

- Templates generate code at compile time.
- Function templates create generic functions.
- Class templates create generic classes.
- Specialization handles special types.
- Variadic templates handle any number of arguments.
- Type traits inspect or transform types during compilation.

