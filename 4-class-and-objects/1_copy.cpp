#include "1_copy.h"

Integer ::Integer() {
    m_pInt = new int(0); // Initialize with 0
}

Integer ::Integer(int value) {
    m_pInt = new int(value);
}

Integer::Integer(Integer&& other) {
    // Move constructor
    m_pInt = other.m_pInt; // Steal the resource
    other.m_pInt = nullptr; // Leave the source in a valid state
}

Integer& Integer ::operator=(Integer&& other) {
    // Move assignment operator
    if (this != &other) { // Self-assignment check
        delete m_pInt; // Free old memory
        m_pInt = other.m_pInt; // Steal the resource
        other.m_pInt = nullptr; // Leave the source in a valid state
    }
    return *this;
}

int Integer ::GetValue() const {
    return *m_pInt;
}

void Integer ::SetValue(int value) {
    *m_pInt = value;
}

Integer ::Integer(const Integer& other) {
    // const is used to prevent modification of the source object.
    // refference is used to avoid unnecessary copying of the object. if reference is not used, the copy constructor will be called again to copy the object, which will lead to infinite recursion and eventually a stack overflow.
    // Copy constructor - deep copy
    m_pInt = new int(*other.m_pInt); // get the value from other and allocate new memory for it
}

Integer& Integer ::operator=(const Integer& other) {
    // Copy assignment operator - deep copy
    if (this != &other) {  // Self-assignment check
        delete m_pInt;     // Free old memory
        m_pInt = new int(*other.m_pInt);  // Allocate and copy
    }
    return *this;
}

Integer ::~Integer() {
    delete m_pInt;
}

