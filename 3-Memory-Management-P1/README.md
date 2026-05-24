# Memory Management (Part 1) - Dynamic Memory Allocation

This section covers fundamental memory management concepts in C++, including stack vs heap memory, C-style allocation (`malloc`/`free`), and C++ style allocation (`new`/`delete`).

---

## 📋 Table of Contents
1. [Process Address Spaces](#process-address-spaces)
2. [C-Style Memory Allocation](#c-style-memory-allocation)
3. [C++ Dynamic Memory Allocation](#c-dynamic-memory-allocation)
4. [Key Differences](#key-differences)

---

## Process Address Spaces

Memory is divided into three main regions:

### **1. Stack**
- **Allocation**: Automatic
- **Scope**: Local variables, function parameters
- **Lifetime**: Until variable goes out of scope
- **Size**: Limited (typically smaller)
- **Speed**: Fast access
- **Example**: 
```cpp
int x = 10;      // Stack allocation
float arr[5];    // Fixed-size array on stack
```

### **2. Data Section**
- **Allocation**: Compile-time
- **Content**: Global and static variables
- **Lifetime**: Entire program execution
- **Example**:
```cpp
static int count = 0;    // Data section
int global = 100;        // Data section
```

### **3. Heap**
- **Allocation**: Runtime (dynamic)
- **Responsibility**: Programmer must free memory
- **Scope**: Until explicitly deallocated
- **Size**: Large (limited by system)
- **Speed**: Slower than stack
- **Example**:
```cpp
int* ptr = new int;      // Heap allocation
int* arr = new int[100]; // Dynamic array
```

---

## C-Style Memory Allocation

### Standard Library Functions
Include: `<cstdlib>`, `<cstdio>`

#### **`malloc()` - Memory Allocate**
Allocates memory but **does not initialize**.

```cpp
#include <cstdlib>
#include <cstdio>

int *ptr = (int *)malloc(sizeof(int) * 5);  // Allocate for 5 integers

if (ptr == NULL) {
    printf("Memory allocation failed\n");
    return 1;
}

*ptr = 10;  // Assign value

free(ptr);  // Must free memory
ptr = NULL; // Set to NULL after freeing
```

#### **`calloc()` - Contiguous Allocation**
Allocates memory **and initializes to zero**.

```cpp
int *ptr = (int *)calloc(5, sizeof(int));  // Allocate 5 ints, initialize to 0

// Use the memory...

free(ptr);
ptr = NULL;
```

#### **`realloc()` - Resize Memory**
Reallocates previously allocated memory.

```cpp
int *ptr = (int *)malloc(sizeof(int) * 5);
int *newPtr = (int *)realloc(ptr, sizeof(int) * 10);  // Resize to 10 integers
if (newPtr != NULL) {
    ptr = newPtr;
}
free(ptr);
```

#### **`free()` - Deallocate Memory**
Releases allocated memory back to the heap.

```cpp
free(ptr);  // Deallocate
ptr = NULL; // Good practice: prevent dangling pointer
```

---

## C++ Dynamic Memory Allocation

### **`new` and `delete` Operators**
Preferred C++ approach. More type-safe than C-style allocation.

### **Single Variable Allocation**
```cpp
int *ptr = new int;        // Allocate single int
int *ptr = new int(5);     // Allocate and initialize to 5
int *ptr = new int{10};    // Uniform initialization (C++11+)

*ptr = 20;                 // Access/modify value

delete ptr;                // Deallocate single object
ptr = nullptr;             // Set to nullptr (C++11+)
```

### **Array Allocation**
```cpp
int *arr = new int[5];              // Allocate array of 5 ints
int *arr = new int[5]{1,2,3,4,5};  // Allocate and initialize array

arr[0] = 10;               // Access array elements

delete[] arr;              // Must use delete[] for arrays (not delete)
arr = nullptr;
```

### **2D Arrays (Pointer to Pointer)**
```cpp
int *p1 = new int[3];      // First row
int *p2 = new int[3];      // Second row

int **arr = new int*[2];   // Array of 2 pointers
arr[0] = p1;               // Point to first row
arr[1] = p2;               // Point to second row

arr[0][0] = 1;             // Access elements like 2D array

// Cleanup (reverse order of allocation)
delete[] p1;
delete[] p2;
delete[] arr;
```

---

## Key Differences

| Feature | `malloc`/`free` (C-style) | `new`/`delete` (C++) |
|---------|---------------------------|---------------------|
| Header | `<cstdlib>` | Built-in operator |
| Type Safety | No (returns void*) | Yes (type-aware) |
| Error Handling | Returns NULL | Throws `bad_alloc` |
| Constructors | Not called | Called automatically |
| Destructors | Not called | Called automatically |
| Syntax | Requires casting | Direct type |
| Array deallocation | `free()` | `delete[]` |
| Initialize to zero | `calloc()` only | With initialization list |

### Example Comparison

**C-style:**
```cpp
int *ptr = (int *)malloc(sizeof(int));  // Type casting required
*ptr = 100;
free(ptr);
ptr = NULL;
```

**C++-style (Recommended):**
```cpp
int *ptr = new int(100);  // Type-safe, cleaner
delete ptr;
ptr = nullptr;
```

---

## ⚠️ Important Rules

### **For Arrays: Use `delete[]` not `delete`**
```cpp
int *arr = new int[5];
delete[] arr;              // ✓ Correct
// delete arr;             // ✗ Wrong - undefined behavior!
```

### **Set to `nullptr` After Deletion**
```cpp
delete ptr;
ptr = nullptr;             // Prevents dangling pointer access
```

### **Always Check Allocation Success (C-style)**
```cpp
int *ptr = (int *)malloc(sizeof(int) * 1000);
if (ptr == NULL) {
    printf("Allocation failed\n");
    return;
}
// Use ptr...
free(ptr);
```

### **Match Allocation/Deallocation Methods**
- If you allocate with `new` → use `delete`
- If you allocate with `new[]` → use `delete[]`
- If you allocate with `malloc` → use `free`

---

## 🔨 Compilation & Execution

### Build
```bash
cd /path/to/3-Memory-Management-P1
mkdir build && cd build
cmake ..
make
```

### Run Programs
```bash
./1_memaloc-c     # C-style memory allocation
./2_mem           # C++ new/delete allocation
```

---

## 📌 Key Takeaways

✓ **Stack** for small, fixed-size data with automatic cleanup  
✓ **Heap** for dynamic, variable-size data (program-controlled)  
✓ **C-style** (`malloc`/`free`) still used in legacy code  
✓ **C++-style** (`new`/`delete`) preferred for modern C++  
✓ Always **match allocation with deallocation** methods  
✓ Use `delete[]` for arrays, `delete` for single objects  
✓ Set pointers to `nullptr` after `delete` to prevent dangling pointers  
✓ **Next step**: Smart pointers (`unique_ptr`, `shared_ptr`) handle this automatically!

---

## 📚 Next: Smart Pointers (Part 2)

Once you master basic `new`/`delete`, learn **smart pointers** which automatically manage memory:
- `std::unique_ptr` - Exclusive ownership
- `std::shared_ptr` - Shared ownership
- `std::weak_ptr` - Non-owning reference

These prevent **memory leaks** and **dangling pointers** automatically!
