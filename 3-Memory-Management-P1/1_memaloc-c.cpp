#include <stdio.h>
#include <stdlib.h>

int main()
{
    // this is a C-style memory allocation example in C++. In C++, it's generally recommended to use new and delete for memory management, but this example demonstrates how to use malloc and free from the C standard library.
    int *ptr = (int *)malloc(sizeof(int) * 5); // Allocate memory for 5 integers
    // int *ptr = (int *)calloc(5, sizeof(int)); // Allocate memory for 5 integers and initialize to 0

    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit with an error code
    }

    *ptr = 10; // Assign a value to the first element

    free(ptr); // Free the allocated memory
    ptr = NULL; // Set pointer to NULL after freeing
 
    return 0; // Exit with success
}