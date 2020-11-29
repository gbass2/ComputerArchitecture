#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <bitset>

// Class for each Ram locoation in data memory
class DataMemory{
protected:
    std::bitset<8> data; // Holds the 32 bit value for this location

public:
DataMemory() : data{0b00000000} {}
std::bitset<8> getData() { return data; }
void setData(uint32_t data) { this->data = data; }
};

// Class for each register created
class Register{
protected:
    std::bitset<32> data; // Holds the 32 bit value for this register
    char regType;

public:
    Register(uint32_t data, char regType) : data{data}, regType{regType} {}
    Register() : data{0b00000000000000000000000000000000}, regType{'\0'} {}

    std::bitset<32> getData() { return data;}
    void setData(uint32_t data) { this->data = data; }
    char getRegType() { return regType;}
    void setRegType(char regType) { this->regType = regType; }

};

// Class for each Instruction in the instruction memory
class Instruction{
protected:
    std::bitset<8> binary; // Holds the 8 bit instruction for the memory loaction. 4 memory locations equal 32 bits.
    std::string instructionType;
    std::string instructionSet;

public:
    Instruction(uint32_t b) : binary{b} {}
    Instruction(): binary{0b00000000} {}

    std::bitset<8> getBinary() { return binary; }
    void setBinary(uint32_t binary) { this->binary = binary; }
    std::string getInstType() { return instructionType;}
    void setInstType(std::string instructionType) { this->instructionType = instructionType; }
    std::string getInstSet() { return instructionSet;}
    void setInstSet(std::string instructionSet) { this->instructionSet = instructionSet; }
};

#endif // MEMORY_H
