#include <iostream>
#include "1_copy.h"

int main() {
    Integer a(5);
    std::cout << "Value of a: " << a.GetValue() << std::endl;

    Integer b = a; // This will cause a shallow copy
    std::cout << "Value of b (after copy): " << b.GetValue() << std::endl;

    b.SetValue(10); // Modifying b will affect a due to shallow copy
    std::cout << "Value of a (after modifying b): " << a.GetValue() << std::endl;
    std::cout << "Value of b (after modifying b): " << b.GetValue() << std::endl;

    return 0;
}