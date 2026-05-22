#include <iostream>

void Print(int count, char c)
{
    for (int i = 0; i < count; ++i) {
        std::cout << c;
    }
    std::cout << std::endl;

}

void EndMessage() {
    std::cout << "End of program" << std::endl;
}

int main()
{
    atexit(EndMessage); // Register the EndMessage function to be called at program exit

    Print(5, '#');
    void(*funcPtr)(int, char) = Print; // Declare a function pointer and assign it to the Print function
    funcPtr(5, '*'); // Call the Print function through the function pointer
    //or
    (*funcPtr)(5, '@'); // Call the Print function through the function pointer using dereference operator
    return 0;
}

//output:
// #####
// *****
// @@@@@
// End of program
