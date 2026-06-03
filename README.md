# C++ General Notes

Short, organized notes for quickly revising important C++ concepts.

## 1. Build And Program Basics

Compile and run:

```bash
g++ hello.cpp -o hello
./hello

g++ hello.cpp
./a.out

g++ -c main.cpp   # creates object file
```

Command-line arguments:

```cpp
int main(int argc, char* argv[]) {}
```

- `argv[0]` is the program name.
- `argv[1]` is the first argument.
- `argv[argc]` is a null pointer.

C++ is often described as a bottom-up language because small objects/classes can be built first and then combined into larger systems.

## 2. Binary, Linking, And Calling Conventions

A calling convention defines:

- how function arguments are passed
- how return values are returned
- who cleans the stack

Common conventions:

- `cdecl`: common C/C++ style; caller cleans the stack
- `stdcall`: often used by Windows API
- `fastcall`: passes some arguments in CPU registers
- `thiscall`: used for C++ member functions, especially older MSVC
- `pascal`: older convention
- `syscall`: system call convention

Example:

```cpp
int __fastcall add(int a, int b) {
    return a + b;
}
```

On Linux, executable and object files usually use the ELF format.

ELF = Executable and Linkable Format.

Dynamic linkage usually means linking external libraries, such as `.dll` or `.so`, at runtime.

## 3. C++ Version Features

C++11 introduced:

- `long long`
- `override`
- in-class member initialization

C++14 added:

- auto return type deduction
- generic lambdas
- variable templates
- binary literals
- digit separators
- `std::make_unique`

Examples:

```cpp
int value = 1'000'000;

template <typename T>
constexpr T pi = T(3.1415926535897932385);
```

## 4. Basic Language Details

Convert number to string:

```cpp
std::string s = std::to_string(42);
```

Array indexing:

```cpp
2[arr] == arr[2] == *(2 + arr)
```

Literals are constant values written directly in code.

Examples:

```cpp
42
'A'
"hello"
3.14
```

Undefined behavior:

```cpp
x + (++x);
x + ++x;
```

Avoid expressions that modify and read the same variable without clear sequencing.

## 5. Const, Volatile, Mutable, And Storage

Const objects can call only `const` member functions.

```cpp
void print() const;
```

`mutable` allows a data member to change even inside a `const` function:

```cpp
mutable int accessCount = 0;
```

`mutable` in lambda capture allows changing the copied value inside the lambda:

```cpp
auto f = [x]() mutable {
    x++;
};
```

This changes only the lambda's copy. To affect the outside variable, capture by reference.

`const volatile`:

```cpp
const volatile int x;
```

Meaning: the program should not write to `x`, but the value may change at any time due to hardware or external effects.

Storage-related keywords:

- `mutable`
- `thread_local`
- `extern`
- `auto`
- `static`
- `register`

Example:

```cpp
thread_local int counter = 0;
```

## 6. Structs, Padding, And Enums

Struct padding depends on member order and alignment.

```cpp
struct Student {
    char a;  // 1 byte
    char b;  // 1 byte
    int c;   // usually 4 bytes
};
```

Possible layout:

- `char a`: 1 byte
- `char b`: 1 byte
- padding: 2 bytes
- `int c`: 4 bytes

Another order:

```cpp
struct Student {
    char a;
    int c;
    char b;
};
```

Possible layout:

- `char a`: 1 byte
- padding: 3 bytes
- `int c`: 4 bytes
- `char b`: 1 byte
- padding: 3 bytes

Reason: the whole struct is usually aligned to the largest member alignment. If the largest member is `int`, the size is usually a multiple of 4.

Enum:

```cpp
enum Color {
    black, // 0
    blue,  // 1
    red    // 2
};
```

Traditional enum size is often 4 bytes, but modern C++ can specify the underlying type:

```cpp
enum Color : char {
    black,
    blue,
    red
};
```

## 7. Classes And Objects

Class members are initialized in the order they are declared in the class, not the order in the constructor initializer list.

Modern C++ allows member initialization inside the class:

```cpp
class Player {
    int score = 0;
};
```

C++ can have an unnamed class, called an anonymous class.

Singleton pattern:

```cpp
class Singleton {
private:
    Singleton() = default;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& GetInstance() {
        static Singleton instance;
        return instance;
    }
};
```

Important parts:

- private constructor prevents direct object creation
- deleted copy constructor prevents copying
- deleted assignment prevents assignment
- static `GetInstance()` gives access to the single object

## 8. Inheritance, Polymorphism, And Binding

Late binding, also called dynamic binding, means the function implementation is chosen at runtime based on the actual object type.

This is how virtual functions work:

```cpp
class Base {
public:
    virtual void show();
};
```

Virtual functions do not automatically make a function templated. Templates and virtual functions solve different problems.

Private inheritance:

- derived class can access public/protected base members inside its own class
- outside code cannot access the base part through the derived object

Protected inheritance:

- similar restriction from outside code
- grandchildren may access inherited protected base members if inheritance continues in an accessible way

Object slicing:

```cpp
Base b = Derived{};
```

This copies only the base part. Copying a base object into a derived object is not allowed unless you define a valid conversion.

Mixin classes are small classes used to add responsibilities or reusable behavior to another class.

## 9. Templates

When calling a member template through a dependent object, use the `template` keyword:

```cpp
struct A {
    template <typename X>
    void foo(X value) {
        std::cout << value << std::endl;
    }
};

template <typename T, typename U>
void call_foo(T t, U u) {
    t.template foo<U>(u);
}
```

`t.template foo<U>(u)` tells the compiler that `foo` is a template member function.

## 10. Files And Streams

File stream types:

- `std::ofstream`: write only
- `std::ifstream`: read only
- `std::fstream`: read and write

Common file modes:

- `ios::trunc`: truncate file when opening
- `ios::ate`: open and move to end immediately
- `ios::app`: append mode
- `ios::in`: input/read
- `ios::out`: output/write
- `ios::binary`: binary mode

Examples:

```cpp
std::ofstream file("data.txt", std::ios::app);
file << "New line added\n";
```

Combine modes using `|`:

```cpp
std::fstream file("data.txt", std::ios::in | std::ios::out);
```

File positions:

```cpp
std::ifstream file("test.txt");
auto pos = file.tellg(); // current read position

file.seekg(0);           // move read pointer to beginning
```

- `tellg()`: get current read pointer
- `tellp()`: get current write pointer
- `seekg()`: move read pointer
- `seekp()`: move write pointer

Binary read:

```cpp
std::ifstream file("data.bin", std::ios::in | std::ios::binary);
```

## 11. STL And Utilities

Use `std::for_each` from `<algorithm>`:

```cpp
#include <algorithm>
#include <vector>

void func(int i) {
    // ...
}

std::vector<int> v = {1, 2, 3, 4};
std::for_each(v.begin(), v.end(), func);
```

Accessing elements:

```cpp
vec.at(0);       // vector access with bounds checking
std::get<0>(arr); // tuple or std::array style access
```

Wrong examples:

```cpp
get<0>(vec); // wrong for vector
vec.get(0);  // wrong for vector
```

## 12. Exceptions

C++ handles synchronous exceptions.

Examples:

```cpp
try {
    throw std::runtime_error("error");
} catch (const std::exception& e) {
    std::cout << e.what();
}
```

Asynchronous/system-level events are not normal C++ exceptions:

- hardware interrupt
- segmentation fault
- access violation
- divide-by-zero signal
- Ctrl+C signal
- OS kill signal

Exception safety levels:

- no guarantee: object/state may be left broken after an exception
- basic guarantee: object/state remains valid, but value may change
- strong guarantee: operation either completes fully or has no effect
- no-throw guarantee: operation will not throw exceptions

## 13. Variadic Functions

C-style variadic functions use `<stdarg.h>`:

```cpp
#include <iostream>
#include <stdarg.h>

void dumplist(int n, ...) {
    va_list p;
    va_start(p, n);

    while (n-- > 0) {
        int i = va_arg(p, int);
        std::cout << i;
    }

    va_end(p);
}
```

In modern C++, prefer variadic templates when possible.

## 14. Other Notes

Container design often considers:

- structural and load requirements
- transportation and handling requirements
- safety and certification requirements

