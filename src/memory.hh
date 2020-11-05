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

public:
uint32_t getData() { return data;}
void setData(uint32_t data) { this->data = data; }
};

// Class for each register created
class Register{
protected:
    uint32_t data; // Holds the 32 bit value for this register
    char regType;

public:
    Register(uint32_t data, char regType) : data{data}, regType{regType} {}
    Register() : data{00000000000000000000000000000000}, regType{'x'} {}

    uint32_t getData() { return data;}
    void setData(uint32_t data) { this->data = data; }
    char getRegType() { return regType;}
    void setRegType(char regType) { this->regType = regType; }

};

// Class for each Instruction in the instruction memory
class Instruction{
protected:
    uint32_t binary; // Holds the 8 bit instruction for the memory loaction. 4 memory locations equal 32 bits.
    std::string instructionType;

public:
    Instruction(uint32_t b) : binary(b) {}
    Instruction() { binary = 00000000; instructionType = ""; }

    uint32_t getBinary() { return binary;}
    void setBinary(uint8_t binary) { this->binary = binary; }
    std::string getInstType() { return instructionType;}
    void setInstType(std::string instructionType) { this->instructionType = instructionType; }
};

#endif // MEMORY_H
