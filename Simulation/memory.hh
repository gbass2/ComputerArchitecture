#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <vector>
#include <deque>
#include <tuple>


// Class for each Ram locoation created
class DataMemory{
protected:
    uint32_t data; // Holds the 32 bit value for this location
};

// Class for each register created
class Register{
protected:
    uint32_t data; // Holds the 32 bit value for this register
    std::string regType;
};

// Class for each Instruction created
class Instruction{
protected:
    std::string binary; // Holds the 32 bit instruction for the memory loaction
    std::string instructionType;
    Register *operand;

public:
    Instruction(std::string b) : binary(b) {}
    Instruction();

};


#endif // MEMORY_H
