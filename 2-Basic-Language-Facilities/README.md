# C++ Basic Language Facilities - Learning Guide

This section covers fundamental C++ concepts including data types, I/O, pointers, references, and function pointers.

---

## 📋 Table of Contents
1. [Compilation Process](#compilation-process)
2. [Primitive Data Types](#primitive-data-types)
3. [Console I/O](#console-io)
4. [Pointers & References](#pointers--references)
5. [Function Pointers](#function-pointers)

---

## Compilation Process

The C++ build process consists of **4 steps**:

1. **Preprocessing** - `#include` directives processed, macros expanded
2. **Compilation** - Syntax checking, converted to object code (`.o`)
3. **Linking** - Object code linked with standard libraries
4. **Output** - Executable file generated

---

## Primitive Data Types

### Type Categories
- **Integral Types**: `bool`, `char`, `int`, `short`, `long`
- **Floating Point**: `float`, `double`
- **Special**: `void` (use with pointers/functions)

### Modifiers
| Modifier | Usage |
|----------|-------|
| `signed/unsigned` | Affects range of values |
| `short/long` | Affects size in memory |
| `const` | Value cannot be modified |
| `static` | Internal linkage |

### Memory & Limits
```cpp
#include <climits>  // For integer limits
#include <cfloat>   // For float limits

float f = 1.2345f;   // 'f' suffix required for float
double d = 3.14;     // Default for decimal
```

### Initialization (Prefer Uniform)
```cpp
int a(0);        // Direct (before C++11)
int a{0};        // Uniform (C++11+) ✓ Recommended
int a = 0;       // Copy (avoid for user-defined types)
int arr[8]{};    // Initialize array with zeros
```

---

## Console I/O

### Input/Output Streams
```cpp
#include <iostream>

std::cout << "Output text" << std::endl;      // Print to console
std::cin >> variable;                         // Read single word
std::cin.getline(buffer, size, '\n');        // Read entire line
```

**Example:**
```cpp
char name[50];
std::cout << "Enter name: ";
std::cin.getline(name, 50, '\n');  // Reads full name with spaces
std::cout << "Hello, " << name << "!" << std::endl;
```

---

## Pointers & References

### Pointers
Store memory addresses of variables.

```cpp
int a = 10;
int* ptr = &a;           // Pointer to integer, initialized with address
int* ptr = nullptr;      // Null pointer (C++11+, safer than NULL)

std::cout << a << std::endl;      // Value: 10
std::cout << &a << std::endl;     // Address
std::cout << ptr << std::endl;    // Address
std::cout << *ptr << std::endl;   // Value: 10

*ptr = 20;               // Modify value through pointer
```

### Void Pointers
```cpp
void* voidPtr = ptr;     // Can point to any type
int value = *static_cast<int*>(voidPtr);  // Type cast needed
```

### References
Aliases to existing variables. Must be initialized, cannot be `nullptr`.

```cpp
int a = 20;
int &ref = a;            // Reference initialized
ref = 21;                // Changes a to 21
// &ref == &a (same address)
// ref is value, &ref is address
// No dereferencing needed with references
```

### Key Differences
| Feature | Pointer | Reference |
|---------|---------|-----------|
| Can be null | ✓ | ✗ |
| Can reassign | ✓ | ✗ |
| Dereferencing | Required (`*ptr`) | Not needed |
| Storage | Yes | No |

### Pointers with Arrays
```cpp
int arr[] = {1, 2, 3, 4, 5};
int *beg = std::begin(arr);
int *end = std::end(arr);

while (beg != end) {
    std::cout << *beg << " ";
    ++beg;
}
```

---

## Function Pointers

### Declaration & Usage
```cpp
void Print(int count, char c) {
    for (int i = 0; i < count; ++i)
        std::cout << c;
    std::cout << std::endl;
}

int main() {
    // Declare function pointer
    void(*funcPtr)(int, char) = Print;
    
    // Call through pointer
    funcPtr(5, '#');           // Method 1
    (*funcPtr)(5, '*');        // Method 2 (with dereference)
    
    return 0;
}
```

### With `atexit()`
```cpp
void EndMessage() {
    std::cout << "Program ending" << std::endl;
}

atexit(EndMessage);  // Called automatically at program exit
```

---

## 🔨 Quick Compilation

### Build All Files
```bash
cd /path/to/2-Basic-Language-Facilities
mkdir build && cd build
cmake ..
make
```

### Run Individual Programs
```bash
./1_fistcpp          # Hello World
./2_IO               # Console input/output
./3_pointers         # Basic pointer operations
./4_pointers         # Array pointers
./5_funcPointers     # Function pointers
```

---

## 📌 Key Takeaways

✓ Use **uniform initialization** `{}`  
✓ Prefer **pointers with addresses** for flexible memory management  
✓ Use **references** for aliases (simpler, safer)  
✓ **Function pointers** enable callbacks and dynamic behavior  
✓ Always initialize **pointers** to avoid undefined behavior  
✓ Use **const** to mark immutable values  

---

## 📚 Related Concepts
- Header guards (`#ifndef`, `#define`)
- Function overloading (name mangling)
- Default function arguments
- Inline functions (small, fast)
- `constexpr` for compile-time computation
- Namespaces for code organization

