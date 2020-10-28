#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <vector>
#include <deque>
#include <tuple>

// Class for each Ram locoation in data memory
class DataMemory{
protected:
    uint32_t data; // Holds the 32 bit value for this location
};

// Class for each register created
class Register{
protected:
    uint32_t data; // Holds the 32 bit value for this register
    char regType;
};

// Class for each Instruction in the instruction memory
class Instruction{
protected:
    std::string binary; // Holds the 32 bit instruction for the memory loaction
    std::string instructionType;
    Register *operand; // May not need this

public:
    Instruction(std::string b) : binary(b) {}
    Instruction();

};


#endif // MEMORY_H
