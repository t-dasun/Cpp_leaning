#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void New() {
    // This function demonstrates the use of new and delete in C++. It allocates memory for an array of integers using new and then deallocates it using delete[].
    int *ptr = new int[5]; // Allocate memory for 5 integers
    //int *ptr = new int[5] {1,2`,3,4,5}; // Allocate memory for 5 integers and initialize with values

    // if (ptr == nullptr) { // not needed in c++ new will throw error if bad_alloc
    //     printf("Memory allocation failed\n");
    //     return; // Exit the function if allocation fails
    // }

    ptr[0] = 10; // Assign a value to the first element
    std::cout<<"First element: " << ptr[0] << std::endl; 

    delete[] ptr; // Free the allocated memory
    ptr = nullptr; // Set pointer to nullptr after freeing

    //ptr = nullptr; this is a good practice to avoid dangling pointers, which can lead to undefined behavior if dereferenced after the memory has been freed. but in this simple example, it's not strictly necessary since the pointer goes out of scope at the end of the function.
}

void New2() {
    // This function demonstrates the use of new and delete for a single integer. It allocates memory for a single integer using new and then deallocates it using delete.
    int *ptr = new int; // Allocate memory for a single integer
    //int *ptr = new int(5); // Allocate memory for a single integer and initialize to 5

    *ptr = 20; // Assign a value to the allocated integer
    std::cout<<"Value: " << *ptr << std::endl; 

    delete ptr; // Free the allocated memory
    ptr = nullptr; // Set pointer to nullptr after freeing
}

void TwoD() {
    int *p1 = new int[3]; // Allocate memory for an array of 3 integers
    int *p2 = new int[3]; // Allocate memory for another array of 3 integers

    int **arr = new int*[2]; // Allocate memory for an array of 2 pointers to integers
    arr[0] = p1; // Point the first element to the first array
    arr[1] = p2; // Point the second element to the second array 

    // Now you can use arr[0][i] and arr[1][i] to access the elements of the two arrays
    arr[0][0] = 1; // Assign a value to the first element of the first array
    // Don't forget to free the allocated memory
    delete[] p1; // Free the first array
    delete[] p2; // Free the second array
    delete[] arr; // Free the array of pointers


}

int main()
{
    New(); // Call the function that demonstrates new and delete for an array
    New2(); // Call the function that demonstrates new and delete for a single integer
 
    return 0; // Exit with success
}