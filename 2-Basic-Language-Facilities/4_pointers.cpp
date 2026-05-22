#include <iostream>

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    //int* ptr = arr; // ptr points to the first element of the array

    int *beg = std::begin(arr); // or int *beg = arr[0]
    int *end = std::end(arr); // or int *end = arr[5] or int *end = arr + 5

    while (beg != end) {
        std::cout << *beg << " "; 
        ++beg;
    }
    
    // for (; beg != end; ++beg) {
    //     std::cout << *beg << " ";
    // }
    

    return 0;
}