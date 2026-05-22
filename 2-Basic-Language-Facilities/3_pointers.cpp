#include <iostream>

int Add(int *a, int *b) {
    return *a + *b; // Dereference the pointers to get the values and return their sum
}


void Factorial(int *a, int *result) {
    *result = 1; // Initialize result to 1
    for (int i = 1; i <= *a; ++i) {
        *result *= i; // Multiply result by i for each iteration
    }
}

int main()
{
    int a = 10;
    int* ptr = &a; // ptr is a pointer to an integer, initialized to the address of a
    // int *ptr = nullptr; // but we can't read this pointer until we assign it to some address it will point to null and if we try to dereference it it will cause undefined behavior, first nullptr need an address to point to then we can dereference it

    // nullptr is introduced in C++11 to represent a null pointer, NULL is a macro that represents a null pointer constant,so using nullptr is safer and more clear in C++11 and later versions

    std::cout << "Value of a: " << a << std::endl; // Output: 10
    std::cout << "Address of a: " << &a << std::endl; // Output: (address of a)
    std::cout << "Value of ptr: " << ptr << std::endl; // Output: (address of a)
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl; // Output: 10

    *ptr = 20; // Change the value at the address pointed to by ptr
    
    //instade og pointer same type we can use void pointer but we need to type cast it to the type we want to use
     void* voidPtr = ptr; // void pointer can point to any type of data
     std::cout << "Value pointed to by voidPtr (after type casting): " << *static_cast<int*>(voidPtr) << std::endl; // Output: 20
    std::cout << "New value of a: " << a << std::endl; // Output: 20


    int b = 30;
    std::cout << "Sum of a and b: " << Add(&a, &b) << std::endl; // Output: 50

    int result;
    a =5;
    Factorial(&a, &result);
    std::cout << "Factorial of a: " << result << std::endl; 
    return 0;
}