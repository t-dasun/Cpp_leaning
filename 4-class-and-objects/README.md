# Classes and Objects - Object-Oriented Programming Basics

This section covers fundamental Object-Oriented Programming (OOP) concepts including classes, constructors, destructors, copy semantics, and move semantics.

---

## 📋 Table of Contents
1. [OOP Principles](#oop-principles)
2. [Class Basics](#class-basics)
3. [Access Modifiers](#access-modifiers)
4. [Constructors & Destructors](#constructors--destructors)
5. [Copy Semantics (Deep vs Shallow Copy)](#copy-semantics-deep-vs-shallow-copy)
6. [Move Semantics](#move-semantics)
7. [Static Members & Functions](#static-members--functions)
8. [Const Functions](#const-functions)

---

## OOP Principles

### **The Four Pillars of OOP:**

1. **Abstraction** - Hide complex implementation details, expose only necessary interface
2. **Encapsulation** - Bundle data and methods, control access via modifiers
3. **Inheritance** - Create class hierarchies and model "is-a" relationships
4. **Polymorphism** - Same interface, multiple implementations (overloading, overriding)

### **Composition vs Inheritance:**
- **Composition (Has-A)**: Class contains objects of other classes
- **Inheritance (Is-A)**: Class inherits from another class

---

## Class Basics

### **Class Declaration**
```cpp
#pragma once  // Prevents multiple inclusions

class Integer {
    // Private section (default)
    int *m_pInt;

public:
    // Public section - accessible from outside
    Integer();                              // Constructor
    Integer(int value);                     // Parameterized constructor
    Integer(const Integer& other);          // Copy constructor
    Integer& operator=(const Integer& other);  // Copy assignment
    Integer(Integer&& other);               // Move constructor
    Integer& operator=(Integer&& other);    // Move assignment
    
    int GetValue() const;                   // Read-only function
    void SetValue(int value);               // Modifier function
    
    ~Integer();                             // Destructor
};
```

### **Class Definition**
```cpp
#include "1_copy.h"

// Default constructor
Integer::Integer() {
    m_pInt = new int(0);
}

// Parameterized constructor
Integer::Integer(int value) {
    m_pInt = new int(value);
}

// Destructor
Integer::~Integer() {
    delete m_pInt;
}
```

---

## Access Modifiers

Control how class members are accessed from outside the class:

| Modifier | Class | Derived Class | Outside |
|----------|-------|---------------|---------|
| `private` | ✓ | ✗ | ✗ |
| `protected` | ✓ | ✓ | ✗ |
| `public` | ✓ | ✓ | ✓ |

```cpp
class MyClass {
private:
    int privateVar;      // Only accessible within class

protected:
    int protectedVar;    // Accessible in derived classes

public:
    int publicVar;       // Accessible everywhere
};
```

---

## Constructors & Destructors

### **Constructors**
Initialize object state when created. Multiple constructors allowed (overloading).

```cpp
Integer a;           // Default constructor
Integer b(5);        // Parameterized constructor
Integer c = a;       // Copy constructor
```

### **Destructors**
Clean up resources (memory, files, etc.) when object is destroyed.

```cpp
~Integer();          // Called automatically
```

### **Constructor Delegation (C++11+)**
```cpp
class Integer {
public:
    Integer() : Integer(0) {}           // Delegate to parameterized
    Integer(int value) {
        m_pInt = new int(value);
    }
};
```

### **Member Initialization (In-Class)**
```cpp
class MyClass {
    int count = 0;              // Direct initialization
    std::string name = "default";  // Auto-added to constructors
};
```

---

## Copy Semantics: Deep vs Shallow Copy

### **Shallow Copy (Default - PROBLEMATIC!)**
Default copy constructor copies pointers **by value** (same address).

```cpp
Integer a(5);
Integer b = a;  // Shallow copy: both point to same memory!

b.SetValue(10);
std::cout << a.GetValue();  // Output: 10 (a also changed!)

// Destructor issue: double-delete when b is destroyed
// a becomes dangling pointer
```

**Problems:**
- ❌ Both objects share same memory
- ❌ Modifications affect both
- ❌ Double-deletion when destructor runs
- ❌ Dangling pointers

### **Deep Copy (CORRECT - Custom Copy Constructor)**
```cpp
class Integer {
public:
    // Copy constructor - deep copy
    Integer(const Integer& other) {
        m_pInt = new int(*other.m_pInt);  // Allocate NEW memory
    }

    // Copy assignment operator - deep copy
    Integer& operator=(const Integer& other) {
        if (this != &other) {              // Self-assignment check
            delete m_pInt;                 // Free old memory
            m_pInt = new int(*other.m_pInt);  // Allocate and copy
        }
        return *this;
    }
};
```

**Benefits:**
- ✓ Independent copies with separate memory
- ✓ Modifications don't affect original
- ✓ Safe cleanup (no double-deletion)
- ✓ No dangling pointers

### **Comparison**
```cpp
Integer a(5);
Integer b = a;  // Deep copy with custom constructor

b.SetValue(10);
std::cout << a.GetValue();  // Output: 5 (a unchanged) ✓
std::cout << b.GetValue();  // Output: 10 ✓
```

---

## Move Semantics

### **What is Move?**
Transfer ownership of resources without copying (C++11+).

```cpp
Integer&& rvalue_ref = Integer(10);  // Temporary object (rvalue)
```

### **Move Constructor**
```cpp
Integer(Integer&& other) {           // && is rvalue reference
    m_pInt = other.m_pInt;           // Steal the pointer
    other.m_pInt = nullptr;          // Leave source in valid state
}
```

### **Move Assignment Operator**
```cpp
Integer& operator=(Integer&& other) {
    if (this != &other) {
        delete m_pInt;               // Free old memory
        m_pInt = other.m_pInt;       // Steal resource
        other.m_pInt = nullptr;      // Leave source valid but empty
    }
    return *this;
}
```

### **When Move is Used**
```cpp
Integer GetValue() {
    return Integer(42);              // Usually elided; no extra object in C++17+
}

Integer a = GetValue();              // Efficient - no copy!
```

### **Move vs Copy**
| Operation | Copy | Move |
|-----------|------|------|
| Source | Unchanged | Valid but moved-from; state depends on the type |
| Performance | Slow (allocate + copy) | Fast (steal) |
| Use Case | Keeping original | Temporary objects |

---

## Static Members & Functions

### **Static Member Variables**
Shared among all instances of the class.

```cpp
class Counter {
public:
    static int count;          // Declaration in header
    
    Counter() { count++; }
};

int Counter::count = 0;        // Definition in .cpp
```

**Usage:**
```cpp
Counter c1, c2, c3;
std::cout << Counter::count;   // Output: 3 (shared by all)
```

### **Static Member Functions**
Belong to class, not to objects. Can only access static members.

```cpp
class Math {
    static int result;         // Can access in static function
    int temp;                  // Cannot access
    
public:
    static void Calculate() {
        result = 10;           // ✓ OK
        // temp = 5;           // ✗ Error - non-static member
    }
};
```

---

## Const Functions

### **Read-Only Functions**
Promise not to modify object state.

```cpp
class Integer {
    int *m_pInt;

public:
    int GetValue() const {     // const at end
        return *m_pInt;        // Read OK
        // Note: because m_pInt is an int*, a const member function
        // makes the pointer itself const, not the pointed-to int.
        // For true read-only pointed data, use const int* or return by value.
    }
    
    void SetValue(int value) {
        *m_pInt = value;       // No const - can modify
    }
};
```

### **Const Objects**
Can only call const functions.

```cpp
const Integer a(5);
a.GetValue();               // ✓ OK - const function
// a.SetValue(10);          // ✗ Error - non-const function
```

---

## Default Generated Functions

### **Explicitly Defaulting**
```cpp
class MyClass {
public:
    MyClass() = default;                    // Use compiler-generated
    MyClass(const MyClass&) = default;      // Default copy constructor
};
```

### **Explicitly Deleting**
```cpp
class NoCopy {
public:
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;         // No copying allowed
    NoCopy& operator=(const NoCopy&) = delete;
};

// Usage:
NoCopy a;
// NoCopy b = a;     // ✗ Compilation error
```

### **Deleting Specific Overloads**
```cpp
class Integer {
public:
    void SetValue(int value);
    void SetValue(float) = delete;          // Disable float overload
};

Integer i;
i.SetValue(5);              // ✓ OK - int version
// i.SetValue(5.5f);        // ✗ Error - deleted
```

---

## Complete Example: Integer Class

### **Header File (1_copy.h)**
```cpp
#pragma once

class Integer {
    int *m_pInt;

public:
    Integer();                               // Default
    Integer(int value);                      // Parameterized
    Integer(const Integer& other);           // Copy constructor
    Integer& operator=(const Integer& other);  // Copy assignment
    Integer(Integer&& other);                // Move constructor
    Integer& operator=(Integer&& other);     // Move assignment
    
    int GetValue() const;
    void SetValue(int value);
    
    ~Integer();
};
```

### **Implementation (1_copy.cpp)**
```cpp
#include "1_copy.h"

Integer::Integer() {
    m_pInt = new int(0);
}

Integer::Integer(int value) {
    m_pInt = new int(value);
}

// Deep copy constructor
Integer::Integer(const Integer& other) {
    m_pInt = new int(*other.m_pInt);
}

// Copy assignment
Integer& Integer::operator=(const Integer& other) {
    if (this != &other) {
        delete m_pInt;
        m_pInt = new int(*other.m_pInt);
    }
    return *this;
}

// Move constructor
Integer::Integer(Integer&& other) {
    m_pInt = other.m_pInt;
    other.m_pInt = nullptr;
}

// Move assignment
Integer& Integer::operator=(Integer&& other) {
    if (this != &other) {
        delete m_pInt;
        m_pInt = other.m_pInt;
        other.m_pInt = nullptr;
    }
    return *this;
}

int Integer::GetValue() const {
    return *m_pInt;
}

void Integer::SetValue(int value) {
    *m_pInt = value;
}

Integer::~Integer() {
    delete m_pInt;
}
```

### **Usage (1_copy-main.cpp)**
```cpp
#include <iostream>
#include "1_copy.h"

int main() {
    Integer a(5);
    std::cout << "Value of a: " << a.GetValue() << std::endl;

    Integer b = a;  // Deep copy
    std::cout << "Value of b (after copy): " << b.GetValue() << std::endl;

    b.SetValue(10);
    std::cout << "Value of a (after modifying b): " << a.GetValue() << std::endl;  // Still 5
    std::cout << "Value of b: " << b.GetValue() << std::endl;  // 10

    return 0;
}
```

---

## 🔨 Compilation & Execution

### Build
```bash
cd /path/to/4-class-and-objects
mkdir build && cd build
cmake ..
make
```

### Run
```bash
./1_copy
```

### Expected Output
```
Value of a: 5
Value of b (after copy): 5
Value of a (after modifying b): 5
Value of b (after modifying b): 10
```

---

## 📌 Key Takeaways

✓ **Use `#pragma once`** to prevent header redeclaration  
✓ **Encapsulation** via access modifiers protects data  
✓ **Constructor** initializes; **Destructor** cleans up  
✓ **Deep copy** for objects with pointers (safer)  
✓ **Move semantics** (C++11+) optimize temporary transfers  
✓ **const functions** prevent modifying non-mutable members directly  
✓ **Static members** are shared across all instances  
✓ **Self-assignment check** prevents corruption: `if (this != &other)`  
✓ Set owning pointers to `nullptr` after `delete` when the pointer will remain in use  

---

## 📚 Summary Table

| Feature | Purpose | Example |
|---------|---------|---------|
| Constructor | Initialize object | `Integer(5)` |
| Destructor | Cleanup resources | `~Integer()` |
| Copy constructor | Deep copy | `Integer b = a` |
| Copy assignment | Copy via `=` | `b = a` |
| Move constructor | Efficient transfer | `Integer b = std::move(a)` |
| Const function | Read-only | `int GetValue() const` |
| Static member | Shared data | `static int count` |
| Access modifiers | Visibility control | `private`, `public` |
