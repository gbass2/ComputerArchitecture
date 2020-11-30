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
    T *data; // Holds the 32 bit value for this register
    char regType; // Not currently used
    std::bitset<5> name; // Register name in binary

public:
    Register(T *_data, char regType) : data{_data}, regType{regType} {}
    Register() :  data(new T(0)), regType{'\0'}, name{0} {}

    T *getData() const { return data;}
    void setData(int *_data) { data = _data; } // Set the data in int
    void setData(float *_data) { data = _data; } // Set the data in float
    void setName(std::bitset<5> _name) { name = _name; }
    std::bitset<5> getName() const { return name; }

    char getRegType() { return regType;} // Not currently used
    void setRegType(char regType) { this->regType = regType; } // Not currently used

};

// Holds the bank of integer and floating point registers
class RegisterBank : public SimObject{
private:
    // Not currently used
    class RegisterReleaseEvent : public Event{
    private:
        RegisterBank *owner;
    public:
        RegisterReleaseEvent(RegisterBank *_owner) :  owner(_owner) {}
        virtual void process() override {} // Realse the register value
        virtual const char* description() override {return "Register Access";}
    };

    // Event for loading values from registers and storing values in registers
    class RegisterProcessEvent : public Event{
    private:
        RegisterBank *owner;
    public:
        RegisterProcessEvent(RegisterBank *_owner) :  owner(_owner) {}
        virtual void process() override { owner->process(); }
        virtual const char* description() override {return "Register Access";}
    };

    RegisterReleaseEvent *release;
    RegisterProcessEvent *regProcess;
    CPU *cpu;
    bool read = true; // Set false for write and true for read
public:
    std::unordered_map<uint8_t, Register<int>> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register<float>> fpRegisters; // Example: <0101,"Temporary/alternate link register">

    RegisterBank(std::shared_ptr<System> s2, CPU *cpu);
    bool isRead() { return read; } // Used to determine if register read or write
    void setRead(bool _read) { read = _read; }
    void process();
    virtual void initialize() override {}// Initialzes MEQ with a fetch event

    void scheduleRegisterEvent() {
        std::cout << "scheduling Register Access on Tick " << currTick() << std::endl;
        sysMain->schedule(regProcess, currTick() + 1); // Scheduling new event
    }
    void scheduleReleaseEvent() {
        std::cout << "scheduling Register Realse on Tick " << currTick() << std::endl;
        sysMain->schedule(release, currTick() + 1); // Scheduling new event
    }
};

#endif // REGISTER_H
