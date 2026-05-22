#include <iostream>

int main()
{
    char name[50];
    std::cout << "Enter your full name: ";
    std::cin.getline(name, 50, '\n'); // cin >> name; // This will only read the first word if space is there only 1st taken getline will read the whole line 50 characters or until newline is encountered
    std::cout << "Hello, " << name << "!" << std::endl;
    return 0;
}