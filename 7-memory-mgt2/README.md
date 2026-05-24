# Memory Management (Part 2) - Smart Pointers

This section covers C++ smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`) that automatically manage memory and prevent leaks. Include: `#include <memory>`

---

## 📋 Table of Contents
1. [What are Smart Pointers?](#what-are-smart-pointers)
2. [unique_ptr - Exclusive Ownership](#unique_ptr---exclusive-ownership)
3. [shared_ptr - Shared Ownership](#shared_ptr---shared-ownership)
4. [weak_ptr - Non-Owning Reference](#weak_ptr---non-owning-reference)
5. [Circular References Problem](#circular-references-problem)
6. [Custom Deleters](#custom-deleters)
7. [Dynamic Arrays](#dynamic-arrays)
8. [Comparison Table](#comparison-table)

---

## What are Smart Pointers?

### **Why Smart Pointers?**
- ✓ **Automatic cleanup** - No manual `delete` needed
- ✓ **Exception safe** - Memory freed even if exceptions occur
- ✓ **Prevents leaks** - RAII (Resource Acquisition Is Initialization) principle
- ✓ **Clear ownership** - Semantics show who owns the resource
- ✓ **Move semantics** - Efficient resource transfer (C++11+)

### **RAII Principle**
Resources are tied to object lifetime:
- Acquired in constructor
- Released in destructor
- No explicit cleanup needed

```cpp
#include <memory>

void Function() {
    auto ptr = std::make_unique<int>(42);  // Acquire
    // ... use ptr ...
}  // Automatically deleted at scope end (Release)
```

---

## unique_ptr - Exclusive Ownership

### **Concept**
Only one owner at a time. When deleted, resource is freed. No copy allowed.

### **Declaration**
```cpp
#include <memory>

// Method 1: Direct initialization (older, less preferred)
std::unique_ptr<int> ptr1{new int(5)};

// Method 2: Using make_unique (RECOMMENDED - C++14+)
auto ptr2 = std::make_unique<int>(5);
std::unique_ptr<int> ptr3 = std::make_unique<int>(5);

// Method 3: From return value
std::unique_ptr<int> GetPointer(int value) {
    return std::make_unique<int>(value);
}

auto ptr4 = GetPointer(10);
```

### **Accessing Values**
```cpp
auto ptr = std::make_unique<int>(42);

int value = *ptr;           // Dereference
int* raw_ptr = ptr.get();   // Get raw pointer (don't delete!)
```

### **Resetting & Null Assignment**
```cpp
auto ptr = std::make_unique<int>(42);

// Method 1: Reset with new value
ptr.reset(new int(100));    // Old value deleted, new assigned

// Method 2: Reset to nullptr
ptr.reset();                // Delete and set to nullptr
ptr = nullptr;              // Same as above (overloaded =)

// Check if null
if (ptr == nullptr) {
    ptr = std::make_unique<int>(50);
}
```

### **Important: No Direct Assignment!**
```cpp
auto ptr = std::make_unique<int>(5);

// ✗ WRONG - This causes ownership confusion
// ptr = new int(10);  // Compilation error!

// ✓ CORRECT
ptr.reset(new int(10));  // Explicit reset
```

### **Transfer Ownership with Move**
```cpp
auto ptr1 = std::make_unique<int>(42);
auto ptr2 = std::move(ptr1);  // Transfer ownership

// After move:
// ptr1 = nullptr (ownership transferred)
// ptr2 owns the resource
```

### **Passing to Functions**
```cpp
void ProcessValue(std::unique_ptr<int> ptr) {
    std::cout << *ptr << std::endl;
}  // ptr deleted here

auto myPtr = std::make_unique<int>(42);
ProcessValue(std::move(myPtr));  // Transfer ownership

// After call: myPtr is nullptr
// Cannot use: std::cout << *myPtr;  // Crash!
```

### **Returning from Functions (Automatic Move)**
```cpp
std::unique_ptr<int> CreateValue() {
    auto ptr = std::make_unique<int>(42);
    return ptr;  // NRVO if elided, otherwise moved
}

auto result = CreateValue();  // Efficient, no copy
```

---

## shared_ptr - Shared Ownership

### **Concept**
Multiple owners can exist. Resource deleted when last owner destroyed. Reference counting.

### **Declaration**
```cpp
// Method 1: make_shared (RECOMMENDED)
auto ptr1 = std::make_shared<int>(5);

// Method 2: Direct
std::shared_ptr<int> ptr2{new int(5)};

// Method 3: Copy from another shared_ptr
std::shared_ptr<int> ptr3 = ptr1;  // Copy OK (unlike unique_ptr)
```

### **Reference Counting**
```cpp
auto ptr1 = std::make_shared<int>(42);
std::cout << ptr1.use_count();      // Output: 1

{
    std::shared_ptr<int> ptr2 = ptr1;  // Reference count increases
    std::cout << ptr1.use_count();      // Output: 2
}
// ptr2 destroyed, reference count decreases
std::cout << ptr1.use_count();      // Output: 1

// ptr1 destroyed, resource freed
```

### **Control Block**
Maintains reference count internally (transparent to you):

```cpp
// Control Block contains:
// - Pointer to actual object
// - Reference count (number of shared_ptr owners)
// - Weak count (number of weak_ptr observers)
```

### **Accessing Values**
```cpp
auto ptr = std::make_shared<int>(42);

int value = *ptr;           // Dereference
int* raw_ptr = ptr.get();   // Get raw pointer

// For objects with members:
struct Person { std::string name; };
auto p = std::make_shared<Person>("Alice");
p->name;                    // Member access
```

### **Null Check**
```cpp
auto ptr = std::make_shared<int>(42);

if (ptr) {
    std::cout << *ptr;      // Safe
}

if (ptr == nullptr) {
    ptr = std::make_shared<int>(100);
}
```

### **Copying is Safe**
```cpp
auto ptr1 = std::make_shared<int>(42);
auto ptr2 = ptr1;           // Safe copy, increments reference count
auto ptr3 = ptr1;           // Another copy

// All three own the resource
std::cout << ptr1.use_count();  // Output: 3
```

---

## weak_ptr - Non-Owning Reference

### **Concept**
Observer only - doesn't affect reference count. Converts to `shared_ptr` when needed.

### **Declaration & Use**
```cpp
// Create from shared_ptr
auto sp = std::make_shared<int>(10);
std::weak_ptr<int> wp = sp;        // Non-owning reference

// Accessing (using lock())
auto temp = wp.lock();              // Convert to shared_ptr
if (temp) {                         // Check if still alive
    std::cout << *temp << std::endl;
} else {
    std::cout << "Object already deleted\n";
}
```

### **Reference Count Behavior**
```cpp
auto sp = std::make_shared<int>(42);
std::weak_ptr<int> wp = sp;

std::cout << sp.use_count();        // Output: 1 (wp doesn't count)
std::cout << wp.use_count();        // Output: 1 (same as sp)

auto sp2 = sp;                      // Copy shared_ptr
std::cout << sp.use_count();        // Output: 2
// wp still doesn't count
```

### **Important: Cannot Access Directly**
```cpp
auto sp = std::make_shared<int>(42);
std::weak_ptr<int> wp = sp;

// ✗ WRONG - Compilation error
// std::cout << *wp;           // Error!
// std::cout << wp->value;     // Error!

// ✓ CORRECT - Use lock()
if (auto temp = wp.lock()) {        // Convert to shared_ptr
    std::cout << *temp;
}
```

### **Use Case: Avoid Circular References**
See [Circular References Problem](#circular-references-problem) section below.

---

## Circular References Problem

### **The Problem**
When two objects hold `shared_ptr` to each other, they keep each other alive forever (memory leak).

```cpp
class A;
class B;

class A {
public:
    std::shared_ptr<B> b;           // Holds reference to B
};

class B {
public:
    std::shared_ptr<A> a;           // Holds reference to A
};

// Create circular reference
auto objA = std::make_shared<A>();
auto objB = std::make_shared<B>();
objA->b = objB;                     // A references B (count: 2)
objB->a = objA;                     // B references A (count: 2)

// Now both have reference count 2
// When we lose objA and objB:
// - objA destroyed, but objB->a keeps it alive (count: 1)
// - objB destroyed, but objA->b keeps it alive (count: 1)
// MEMORY LEAK: Neither can be fully deleted!
```

### **The Solution: Use weak_ptr**
One side holds `weak_ptr` instead:

```cpp
class A {
public:
    std::shared_ptr<B> b;           // Strong reference
};

class B {
public:
    std::weak_ptr<A> a;             // Weak reference (observer)
};

auto objA = std::make_shared<A>();
auto objB = std::make_shared<B>();
objA->b = objB;                     // A references B (count: 2)
objB->a = objA;                     // B observes A (count: 1 - weak doesn't count!)

// Now:
// - objA destroyed, objB->a becomes invalid (weak reference breaks)
// - objB's reference count drops to 1 (only objA->b holds it)
// - objA and objB properly cleaned up!

// Accessing through weak_ptr:
if (auto strongRef = objB->a.lock()) {
    std::cout << "objA still exists\n";
} else {
    std::cout << "objA was deleted\n";
}
```

### **Guideline**
- Use `shared_ptr` for **ownership** (Parent → Child)
- Use `weak_ptr` for **backlinks** (Child → Parent)

This breaks the cycle and allows proper cleanup.

---

## Custom Deleters

### **When Needed**
- Legacy code with custom cleanup functions
- Objects allocated with non-standard allocators
- Resource management beyond simple `delete`

### **Function Pointer Deleter**
```cpp
#include <memory>

// Custom deleter function
void CustomDelete(int* ptr) {
    std::cout << "Custom deleter called\n";
    delete ptr;
}

// Use with unique_ptr
std::unique_ptr<int, decltype(&CustomDelete)> ptr(
    new int(42), 
    &CustomDelete
);
```

### **Lambda Deleter**
```cpp
auto deleter = [](int* ptr) {
    std::cout << "Cleaning up: " << *ptr << "\n";
    delete ptr;
};

std::unique_ptr<int, decltype(deleter)> ptr(
    new int(42), 
    deleter
);
```

### **Class Deleter**
```cpp
struct CustomDeleter {
    void operator()(int* ptr) const {
        std::cout << "CustomDeleter called\n";
        delete ptr;
    }
};

std::unique_ptr<int, CustomDeleter> ptr(
    new int(42),
    CustomDeleter()
);
```

### **shared_ptr with Custom Deleter (Simpler)**
```cpp
auto deleter = [](int* ptr) {
    std::cout << "Cleaning up\n";
    delete ptr;
};

std::shared_ptr<int> ptr(new int(42), deleter);
```

---

## Dynamic Arrays

### **unique_ptr with Arrays**
```cpp
// Create array
std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);

// Element access
arr[0] = 10;
arr[1] = 20;

// Automatic deletion with [] (not just delete)
// No manual cleanup needed
```

### **shared_ptr with Arrays (C++17+)**
```cpp
// C++17 and later
std::shared_ptr<int[]> arr(new int[5]{});  // value-initialize to zero

arr[0] = 10;
arr[1] = 20;

// Automatic cleanup
```

`std::make_shared<int[]>(5)` is available for arrays starting in C++20.

### **Making Arrays Safely**
```cpp
// With value initialization
auto arr1 = std::make_unique<int[]>(5);  // int values are zero-initialized

// C++20: skip initialization when you plan to overwrite every element
auto arr2 = std::make_unique_for_overwrite<int[]>(5);  // Values are undefined
```

### **Better Alternative: Use Containers**
```cpp
#include <vector>

// Safer and more flexible than smart pointer arrays
std::vector<int> vec(5);  // Size 5, zero-initialized
vec[0] = 10;

// Easier to use, similar safety, more features
```

---

## Comparison Table

| Feature | unique_ptr | shared_ptr | weak_ptr |
|---------|-----------|-----------|----------|
| Ownership | Exclusive | Shared | None (observer) |
| Reference Count | No | Yes | No (doesn't increment) |
| Copy Constructor | Deleted | Allowed | Allowed |
| Move Semantics | Yes | Yes | Yes |
| null check | `ptr == nullptr` | `ptr == nullptr` | `ptr.expired()` |
| Access | `*ptr`, `ptr->` | `*ptr`, `ptr->` | `if (auto sp = wp.lock())` |
| Overhead | Minimal | Control block | Control block |
| Use Case | Exclusive ownership | Shared ownership | Avoid cycles |

---

## Best Practices Summary

### ✓ DO:
```cpp
// Use make_unique/make_shared
auto ptr1 = std::make_unique<int>(42);
auto ptr2 = std::make_shared<int>(42);

// Use move for unique_ptr transfers
auto ptr3 = std::move(ptr1);

// Use weak_ptr to break cycles
std::weak_ptr<A> backlink = shared_a;

// Use lock() for weak_ptr
if (auto sp = wp.lock()) {
    // Safe to use sp
}

// Let scope handle cleanup
void Function() {
    auto ptr = std::make_unique<int>(42);
}  // Auto-deleted
```

### ✗ DON'T:
```cpp
// Don't use raw pointers with smart pointers
// std::unique_ptr<int> ptr = new int(42);  // Error: constructor is explicit
// std::unique_ptr<int> ptr(new int(42));   // Works, but make_unique is preferred

// Don't copy unique_ptr
// auto ptr2 = ptr1;  // Error!

// Don't use after move
// auto ptr2 = std::move(ptr1);
// *ptr1;  // Undefined behavior!

// Don't access weak_ptr directly
// std::weak_ptr<int> wp = ...;
// *wp;  // Error!

// Don't use dynamic arrays with shared_ptr (C++11-C++16)
// std::shared_ptr<int[]> arr(new int[5]);  // Risky before C++17
```

---

## 🔨 Quick Reference

### **Creation**
```cpp
auto up = std::make_unique<T>(args);           // unique_ptr
auto sp = std::make_shared<T>(args);           // shared_ptr
std::weak_ptr<T> wp = sp;                      // weak_ptr
```

### **Access**
```cpp
*ptr                // Dereference
ptr->member         // Member access
ptr.get()           // Get raw pointer
ptr.use_count()     // Reference count (shared_ptr/weak_ptr)
wp.lock()           // weak_ptr → shared_ptr
```

### **Modification**
```cpp
ptr.reset();                    // Delete and nullify
ptr.reset(new T(...));          // Delete and assign new
ptr = nullptr;                  // Nullify
ptr = std::move(other);         // Move ownership
```

---

## 📌 Key Takeaways

✓ **Smart pointers** automate memory management  
✓ **unique_ptr** for exclusive ownership  
✓ **shared_ptr** for shared ownership  
✓ **weak_ptr** for non-owning references (break cycles)  
✓ **Make functions** (`make_unique`, `make_shared`) are preferred  
✓ **Reference counting** in shared_ptr tracks ownership  
✓ **Circular references** leak memory - use weak_ptr to break them  
✓ **RAII principle** - Resource tied to object lifetime  
✓ **Exception safe** - No cleanup code needed  
✓ **No raw `delete`** - Let smart pointers handle it!  

---

## 📚 Memory Management Journey

1. **Part 1: Basic Allocation**
   - `malloc`/`free` (C-style)
   - `new`/`delete` (C++-style)

2. **Part 2: Smart Pointers** ← You are here
   - `unique_ptr` - Exclusive ownership
   - `shared_ptr` - Shared ownership
   - `weak_ptr` - Observer pattern

3. **Best Practice: Use Smart Pointers by Default**
   - Safer than manual `delete`
   - Exception-safe
   - Clear ownership semantics
   - No overhead for `unique_ptr`
