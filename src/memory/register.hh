#ifndef REGISTER_H
#define REGISTER_H

#include "simobject.hh"

#include <string>
#include <iostream>
#include <memory>

// Class for each register created
class Register{
protected:
    uint32_t data; // Holds the 32 bit value for this register
    char regType;

public:
    Register(uint32_t data, char regType) : data{data}, regType{regType} {}
    Register() : data{0b00000000000000000000000000000000}, regType{'\0'} {}

    uint32_t getData() { return data;}
    void setData(uint32_t data) { this->data = data; }
    char getRegType() { return regType;}
    void setRegType(char regType) { this->regType = regType; }

};

class RegisterBank : public SimObject, public Register{
private:
    class RegisterReleaseEvent : public Event{
    private:
        RegisterBank *owner;
    public:
        RegisterReleaseEvent(RegisterBank *_owner) :  owner(_owner) {}
        virtual void process() override { } // Realse the register value
        virtual const char* description() override {return "Register Access";}
        void scheduleReleaseEvent() {
            std::cout << "scheduling Register Realse on Tick " << owner->currTick() << std::endl;
            owner->sysMain->schedule(owner->release, owner->currTick() + 1); // Scheduling new event
        }

    };
    class RegisterProcessEvent : public Event{
    private:
        RegisterBank *owner;
    public:
        RegisterProcessEvent(RegisterBank *_owner) :  owner(_owner) {}
        virtual void process() override {}
        virtual const char* description() override {return "Register Access";}
        void scheduleRegisterEvent() {
            std::cout << "scheduling Register Access on Tick " << owner->currTick() << std::endl;
            owner->sysMain->schedule(owner->regProcess, owner->currTick() + 1); // Scheduling new event
        }
    };

    RegisterReleaseEvent *release;
    RegisterProcessEvent *regProcess;
    bool registerAccess = false;
public:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

    RegisterBank(std::shared_ptr<System> s2);
    void setRegiserAccess(bool registerAccess) { this->registerAccess = registerAccess; }
    bool getRegisterAccess() { return registerAccess; }

    void process(){
        std::cout << "Register Access" << std::endl;
        // Access the registers associated with the instruction and place into decode stage
        sysMain->removeEvent();

        // Access the registers and place them into decode stage

        setRegiserAccess(1);
    }
    virtual void initialize() override { // Initialzes MEQ with a fetch event

    }
};

#endif // REGISTER_H
