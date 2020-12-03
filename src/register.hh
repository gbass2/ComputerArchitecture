#ifndef REGISTER_H
#define REGISTER_H

#include "simobject.hh"

#include <bitset>
#include <string>
#include <iostream>
#include <memory>

class CPU;

// Class for each register created
template<typename T>
class Register{
protected:
    T data; // Holds the 32 bit value for this register
    char regType; // Not currently used
    std::bitset<5> name; // Register name in binary

public:
    Register(std::bitset<5> _name) : data{0}, regType{'\0'}, name{_name} {}
    Register() :  data{0}, regType{'\0'}, name{0} {}

    T getData() const { return data; }
    void setData(int _data) { data = _data; } // Set the data in int
    void setData(float _data) { data = _data; } // Set the data in float
    void setName(std::bitset<5> _name) { name = _name; }
    std::bitset<5> getName() const { return name; }

    char getRegType() { return regType;} // Not currently used
    void setRegType(char regType) { this->regType = regType; } // Not currently used
};

// Holds the bank of integer and floating point registers
class RegisterBank : public SimObject{
private:
    CPU *cpu;
    bool read = true; // Set false for write and true for read
public:
    std::unordered_map<uint16_t, Register<int>> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint16_t, Register<float>> fpRegisters; // Example: <0101,"Temporary/alternate link register">

    RegisterBank(std::shared_ptr<System> s2, CPU *cpu);
    bool isRead() { return read; } // Used to determine if register read or write
    void setRead(bool _read) { read = _read; }
    void process();
    virtual void initialize() override {}// Initialzes MEQ with a fetch event
};

#endif // REGISTER_H
