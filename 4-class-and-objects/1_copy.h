#pragma once

class Integer {
    int *m_pInt;

public:
    // Integer() = default; // Default constructor that compiler generates automatically. 
    // Integer(const Integer& other) = default;

    // Integer(const Integer& other) = delete; // this will say to compiler to not generate copy constructor 

    //void SetValue(float) = delete; // this will say to compiler to not generate SetValue function that takes float as parameter.

    Integer(); // Default constructor
    Integer(int value); // Parameterized constructor
    Integer(const Integer& other); // Copy constructor (deep copy)
    Integer& operator=(const Integer& other); // Copy assignment operator (deep copy)
    Integer(Integer&& other); // Move constructor
    Integer& operator=(Integer&& other); // Move assignment operator
    int GetValue() const; // Getter for the value read-only
    void SetValue(int value);
    ~Integer(); // Destructor
};