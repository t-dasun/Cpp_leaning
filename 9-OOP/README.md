# Object-Oriented Programming (OOP) - Inheritance & Polymorphism

This section covers advanced OOP concepts in C++: inheritance, virtual functions, polymorphism, abstract classes, and the diamond problem.

---

## 📋 Table of Contents
1. [Inheritance Types](#inheritance-types)
2. [Constructors in Inheritance](#constructors-in-inheritance)
3. [Virtual Functions & Polymorphism](#virtual-functions--polymorphism)
4. [Function Overriding](#function-overriding)
5. [Final Keyword](#final-keyword)
6. [Downcasting](#downcasting)
7. [Abstract Classes](#abstract-classes)
8. [Diamond Inheritance](#diamond-inheritance)
9. [Best Practices](#best-practices)

---

## Inheritance Types

### **Class Inheritance Syntax**
```cpp
class Base {
    // ...
};

class Derived : public Base {  // Public inheritance
    // ...
};
```

### **Three Types of Inheritance**

| Inheritance | Base public | Base protected | Base private |
|-------------|------------|----------------|-------------|
| **public** | public | protected | private |
| **protected** | protected | protected | private |
| **private** | private | private | private |

### **Public Inheritance (Most Common)**
Child class inherits public/protected members as public/protected.

```cpp
class Animal {
public:
    void Eat() { std::cout << "Eating\n"; }
protected:
    int age;
private:
    int id;
};

class Dog : public Animal {
public:
    void Bark() { 
        Eat();              // ✓ Can call public
        age = 5;            // ✓ Can access protected
        // id = 1;           // ✗ Cannot access private
    }
};

Dog d;
d.Eat();                    // ✓ Public access
// d.age;                   // ✗ Protected (not accessible outside)
```

### **Protected Inheritance**
Public members become protected in derived class (rarely used).

```cpp
class Animal { };
class Dog : protected Animal { };  // Uncommon
```

### **Private Inheritance**
All inherited members become private (rare, composition preferred).

```cpp
class Animal { };
class Dog : private Animal { };    // Uncommon - prefer composition
```

---

## Constructors in Inheritance

### **Base Constructor Must be Called**
If base class has no default constructor, derived class **must invoke it**.

```cpp
class Animal {
protected:
    std::string name;
public:
    Animal(const std::string& name) : name(name) {}
};

class Dog : public Animal {
public:
    // WRONG - doesn't call base constructor
    // Dog(const std::string& name) { }
    
    // CORRECT - delegates to base constructor
    Dog(const std::string& name) : Animal(name) {}
};
```

### **Constructor Delegation (C++11+)**
Inherit base constructor automatically:

```cpp
class Animal {
public:
    Animal(const std::string& name) { }
    Animal(int id) { }
};

class Dog : public Animal {
public:
    using Animal::Animal;  // Inherit all base constructors
};

Dog d1("Buddy");           // Uses Animal(const std::string&)
Dog d2(123);               // Uses Animal(int)
```

### **Member Initialization in Derived Class**
```cpp
class Dog : public Animal {
private:
    std::string breed;

public:
    Dog(const std::string& name, const std::string& b)
        : Animal(name),    // Initialize base
          breed(b)         // Initialize derived member
    {
    }
};
```

---

## Virtual Functions & Polymorphism

### **Why Virtual Functions?**
Enable **runtime polymorphism** - correct function called based on **actual type**, not pointer type.

```cpp
class Animal {
public:
    void Speak() { std::cout << "Some sound\n"; }  // Non-virtual
};

class Dog : public Animal {
public:
    void Speak() { std::cout << "Woof!\n"; }
};

// WITHOUT virtual (static binding):
Dog dog;
Animal* ptr = &dog;
ptr->Speak();              // Output: "Some sound" (WRONG!)
```

### **Virtual Functions - Correct Approach**
```cpp
class Animal {
public:
    virtual void Speak() { std::cout << "Some sound\n"; }
    virtual ~Animal() { }  // Always virtual destructor!
};

class Dog : public Animal {
public:
    void Speak() override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void Speak() override { std::cout << "Meow!\n"; }
};

// WITH virtual (runtime polymorphism):
Animal* ptr1 = new Dog();
Animal* ptr2 = new Cat();

ptr1->Speak();             // Output: "Woof!" ✓
ptr2->Speak();             // Output: "Meow!" ✓

delete ptr1;
delete ptr2;
```

### **Virtual Table (V-table)**
Compiler creates a table of function pointers for virtual functions:

```
Animal class
  [v-table ptr] → v-table
                    | Speak() → Animal::Speak
                    | ~Animal()
                    
Dog class
  [v-table ptr] → v-table
                    | Speak() → Dog::Speak
                    | ~Dog()
```

When you call `ptr->Speak()`, it looks up the correct function in the v-table.

### **Important: Base Pointer/Reference, Not Object**
```cpp
Dog dog;
Animal a = dog;            // ✗ OBJECT SLICING - copies only Animal part
Animal& ref = dog;         // ✓ Reference works
Animal* ptr = &dog;        // ✓ Pointer works

ref.Speak();               // Calls Dog::Speak ✓
```

**Object Slicing**: Creating a base object from a derived object copies only the base subobject and loses derived behavior/data.

---

## Function Overriding

### **override Keyword (C++11+)**
Explicitly mark function as overriding base class function. Compiler checks signature matches.

```cpp
class Animal {
public:
    virtual void Speak() { }
    virtual void Move(int speed) { }
};

class Dog : public Animal {
public:
    // ✓ Correct override
    void Speak() override { std::cout << "Woof!\n"; }
    
    // ✗ WRONG - Different parameter type
    // void Move(double speed) override { }  // Compilation ERROR!
    
    // ✓ Correct
    void Move(int speed) override { std::cout << "Running\n"; }
};
```

**Benefits of `override`:**
- Compiler catches typos in function names
- Catches parameter type mismatches
- Makes intent clear to readers

### **Calling Base Class Implementation**
```cpp
class Animal {
public:
    virtual void Speak() {
        std::cout << "Base speak\n";
    }
};

class Dog : public Animal {
public:
    void Speak() override {
        Animal::Speak();              // Call base implementation
        std::cout << "Woof!\n";       // Add derived behavior
    }
};

Dog d;
d.Speak();
// Output:
// Base speak
// Woof!
```

---

## Final Keyword

### **Prevent Class Inheritance**
```cpp
class FinalClass final {
    // Cannot be inherited
};

// ✗ Compilation ERROR
// class Derived : public FinalClass { };
```

### **Prevent Function Override**
```cpp
class Base {
public:
    virtual void Func() { }
};

class Derived : public Base {
public:
    void Func() final override { }  // No further overrides allowed
};

class GrandChild : public Derived {
public:
    // ✗ WRONG - Cannot override final function
    // void Func() override { }   // Compilation ERROR
};
```

---

## Downcasting

Converting base class pointer/reference to derived class pointer/reference.

### **Scenario: Access Derived-Only Functions**
```cpp
class Animal {
public:
    virtual void Eat() { }
    virtual ~Animal() { }
};

class Dog : public Animal {
public:
    void Fetch() { std::cout << "Fetching ball!\n"; }
};

Animal* ptr = new Dog();
ptr->Eat();                 // ✓ Base function
// ptr->Fetch();            // ✗ Error - not in Animal

// Need to downcast to access Dog-specific function
```

### **Method 1: Using typeid() and static_cast**
```cpp
#include <typeinfo>

if (typeid(*ptr) == typeid(Dog)) {
    Dog* dogPtr = static_cast<Dog*>(ptr);
    dogPtr->Fetch();        // ✓ Now can call
}
```

### **Method 2: Using dynamic_cast (Recommended)**
Safer - returns `nullptr` if downcast fails.

```cpp
Dog* dogPtr = dynamic_cast<Dog*>(ptr);
if (dogPtr != nullptr) {
    dogPtr->Fetch();        // ✓ Safe
} else {
    std::cout << "Not a Dog\n";
}
```

### **dynamic_cast with Reference (Exception Handling)**
```cpp
try {
    Dog& dogRef = dynamic_cast<Dog&>(*ptr);
    dogRef.Fetch();         // ✓ Works if ptr is Dog
} catch (std::bad_cast& e) {
    std::cout << "Downcast failed: " << e.what() << "\n";
}
```

### **Comparison**

| Method | Safety | Performance | Use Case |
|--------|--------|-------------|----------|
| `static_cast` | Unsafe | Fast | Only if you're certain |
| `dynamic_cast` | Safe | Slower | General use |
| `typeid()` | Safe | Slower | Debugging |

**Recommendation**: Use `dynamic_cast` by default. Avoid `typeid()` - use polymorphism instead.

---

## Abstract Classes

### **Purpose**
Base class defining interface that derived classes **must** implement.

### **Pure Virtual Function**
A function declared with `= 0` is pure virtual and makes the class abstract. It may still have a definition outside the class, but derived concrete classes must provide an override unless an intermediate base already does.

```cpp
class Shape {
public:
    virtual void Draw() = 0;           // Pure virtual
    virtual double Area() = 0;         // Pure virtual
    virtual ~Shape() { }
};

// ✗ Cannot instantiate abstract class
// Shape s;                  // Compilation ERROR

// ✓ Can use as pointer/reference
Shape* ptr = new Circle(5.0);  // Derived must implement Draw() and Area()
delete ptr;
```

### **Concrete Derived Class**
Must implement **all** pure virtual functions.

```cpp
class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) { }
    
    void Draw() override {
        std::cout << "Drawing circle\n";
    }
    
    double Area() override {
        return 3.14 * radius * radius;
    }
};

Circle c(5.0);
c.Draw();              // ✓ Works
std::cout << c.Area(); // ✓ Works
```

### **Partial Implementation Allowed**
If derived class doesn't implement all pure virtual functions, it's also abstract:

```cpp
class Shape {
public:
    virtual void Draw() = 0;
    virtual void Rotate() = 0;
};

class PartialShape : public Shape {
public:
    void Draw() override { }
    // Rotate not implemented
};

// ✗ Cannot instantiate - still abstract
// PartialShape ps;          // Compilation ERROR

class CompleteShape : public PartialShape {
public:
    void Rotate() override { }  // Now complete
};

CompleteShape cs;              // ✓ OK
```

---

## Diamond Inheritance

### **The Problem**
Multiple inheritance from same base class causes duplicate base object.

```cpp
class Stream {
public:
    virtual void Open() { }
    virtual ~Stream() { }
};

class InputStream : public Stream { };    // Inherits Stream
class OutputStream : public Stream { };   // Also inherits Stream

class IOStream : public InputStream, public OutputStream { };
                     // Stream appears TWICE!

IOStream io;
// Problem: io contains TWO Stream subobjects (duplicate)
// io.Open() is ambiguous - which Stream?
```

### **The Solution: Virtual Inheritance**
Base class inherited as **virtual** - appears only once in derived class.

```cpp
class Stream {
public:
    virtual void Open() { }
    virtual ~Stream() { }
};

class InputStream : virtual public Stream { };
                    // ↑ virtual inheritance

class OutputStream : virtual public Stream { };
                     // ↑ virtual inheritance

class IOStream : public InputStream, public OutputStream {
public:
    IOStream() : Stream(), InputStream(), OutputStream() {
        // Must explicitly initialize virtual base
    }
};

IOStream io;
io.Open();            // ✓ Unambiguous - only one Stream
```

### **Memory Layout**
Without virtual inheritance:
```
IOStream
├── InputStream
│   └── Stream (duplicate #1)
└── OutputStream
    └── Stream (duplicate #2)
```

With virtual inheritance:
```
IOStream
├── InputStream (pointer to shared Stream)
├── OutputStream (pointer to shared Stream)
└── Stream (single shared instance)
```

---

## Best Practices

### ✓ DO:

```cpp
// Use virtual functions for polymorphism
class Base {
public:
    virtual void Func() { }
    virtual ~Base() { }  // Always virtual!
};

// Use override keyword
class Derived : public Base {
public:
    void Func() override { }
};

// Use dynamic_cast for safe downcasting
Derived* d = dynamic_cast<Derived*>(base_ptr);
if (d) { d->DerivedFunc(); }

// Use pure virtual for abstract classes
class Interface {
public:
    virtual void Required() = 0;
    virtual ~Interface() { }
};

// Use virtual inheritance for diamond inheritance
class Derived1 : virtual public Base { };
class Derived2 : virtual public Base { };

// Prefer composition over inheritance
class Car {
    Engine engine;  // Composition (prefer)
};

// Avoid deep inheritance hierarchies
// 2-3 levels usually enough
```

### ✗ DON'T:

```cpp
// Don't forget virtual destructor when deleting through a base pointer
// class Base {
//     ~Base() { }  // WRONG - non-virtual
// };

// Don't use object slicing
// Base b = derived;  // WRONG - loses derived info

// Don't use typeid() unnecessarily
// if (typeid(*ptr) == typeid(Derived)) { }  // Avoid

// Don't use static_cast for uncertain downcasts
// Derived* d = static_cast<Derived*>(base_ptr);  // Unsafe

// Don't accidentally hide a virtual function with the same name but different signature
// class Derived : public Base {
//     void Func(int x) { }  // Wrong signature!
// };

// Don't overuse virtual functions
// Mark only functions that will be overridden

// Don't create unnecessary deep inheritance
// Use composition for "has-a" relationships
```

---

## 📌 Key Takeaways

✓ **Virtual functions** enable runtime polymorphism  
✓ Declare a **virtual destructor** in polymorphic base classes  
✓ **Use `override` keyword** to catch overriding errors  
✓ **Use `final` keyword** to prevent further inheritance/overriding  
✓ **Use `dynamic_cast`** for safe downcasting (returns nullptr on failure)  
✓ **Avoid `typeid()`** - use polymorphism instead  
✓ **Abstract classes** define interfaces with pure virtual functions  
✓ **Virtual inheritance** solves diamond problem - use cautiously  
✓ **Prefer composition over inheritance** when appropriate  
✓ **Base pointer/reference**, not object (avoid object slicing)  

---

## 📚 Quick Reference

### **Inheritance Declaration**
```cpp
class Derived : public Base { };          // Public inheritance (most common)
class Derived : protected Base { };       // Protected (rare)
class Derived : private Base { };         // Private (rare)
class Derived : virtual public Base { };  // Virtual (diamond problem)
```

### **Virtual Functions**
```cpp
virtual void Func() { }                   // Base class
void Func() override { }                  // Derived class
virtual void Func() = 0;                  // Pure virtual (abstract)
void Func() final { }                     // Cannot be overridden
```

### **Downcasting**
```cpp
auto* derived = dynamic_cast<Derived*>(base_ptr);
if (derived) { /* safe to use */ }
```

### **Abstract Classes**
```cpp
class AbstractBase {
public:
    virtual void Required() = 0;          // Must be implemented
    virtual ~AbstractBase() { }
};
```

---

## 🎯 Polymorphism Examples

### **Strategy Pattern (Runtime Polymorphism)**
```cpp
#include <memory>

class PaymentStrategy {
public:
    virtual void Pay(double amount) = 0;
    virtual ~PaymentStrategy() { }
};

class CreditCard : public PaymentStrategy {
public:
    void Pay(double amount) override {
        std::cout << "Paying $" << amount << " via credit card\n";
    }
};

class PayPal : public PaymentStrategy {
public:
    void Pay(double amount) override {
        std::cout << "Paying $" << amount << " via PayPal\n";
    }
};

// Usage
std::unique_ptr<PaymentStrategy> strategy = GetPaymentMethod();  // Returns actual type
strategy->Pay(99.99);  // Correct method called!
```

This demonstrates the power of polymorphism - same interface, multiple implementations!
