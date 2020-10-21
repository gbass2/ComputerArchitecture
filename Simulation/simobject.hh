#ifndef SIMOBJECT_H
#define SIMOBJECT_H

#define Tick uint64_t

#include <vector>
#include <deque>
#include <iostream>
#include <tuple>
#include <string>
#include "event.hh"
#include "system.hh"
#include "memory.hh"

class SimObject {
protected:
    System *sys;

public:
    Tick currTick() { return sys->currTick(); } // Gets the current system time
    void incTick(Tick t) { sys->incTick(t); } // Increments the system time
    void schedule(Event *e, Tick t) { sys->schedule(e,t); } // Schedules an event
    void reschedule(Event *e, Tick t) { sys->reschedule(e,t); } // Reschedules an event
    void printMEQ() { sys->printMEQ(); }
    virtual void initialize() = 0; // Creates the first event
    SimObject(System *s) : sys(s) {}
};

// Memory SimObject. It holds the instruction memory and the data memory
class Memory : public SimObject, public RAM, public Instruction{
private:
    // Port1 holds the instruction memory
    class Port1 : public Event{
    private:
        Instruction instructionMem[0x093]; // Instruction memory 0 - 0x093
        Memory *mem;
        friend class Fetch; // Allows Store class to access these private variables

    public:
        // Creates and processes a port1 event
        Port1(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "processing on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p1, mem->currTick() + mem->clk);
        }
        virtual const char* description() override {return "Instruction Memory Access"; }
    };

    // Port2 holds the data memory
    class Port2 : public Event{
    private:
        RAM dataMemory[0x1000]; // Memory for loactions 0x200 - 0x1000
        Memory *mem;
        friend class CPU; // Allows Store class to access these private variables

    public:
        // Creates and processes a port1 event
        Port2(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "processing on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p2, mem->currTick() + mem->clk);
        }
        virtual const char* description() override {return "Data Memory Access"; }
    };
    Port1 *p1;
    Port2 *p2;
    Tick clk;

public:
    Memory(System *s) : SimObject(s), p1(new Port1(this)), p2(new Port2(this)){}
};

class RegisterBank :  public Register{
private:
    Register intRegisters[32]; // Bank of 32 integer registers
    Register fpRegisters[32]; // Bank of 32 floating point registers
};

class CPU: public SimObject{
private:
    // Pipeline stages
    class Fetch : public Event{
    private:
        CPU *cpu;

    public:
        Fetch(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {cpu->process();}
        virtual const char* description() override {return "Fetch";}
        void fetchInstruction(size_t PC); // Gets the instruction from the instruction memory
    };

    class Decode : public Event{
    // Finds the data from the registers and passes it to the execution stage to be executed
    private:
        CPU *cpu;

    public:
        Decode(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {cpu->process();}
        virtual const char* description() override {return "Decode";}
        void decodeInstruction(); // Prints out the decode stage *****
    };

    class Execute : public Event{
    // Passes the incoming registers or memory location to the ALU to be operated
    private:
        CPU *cpu;
    public:
        Execute(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {cpu->process();}
        virtual const char* description() override {return "Execute";}
        void executeInstruction(); // Prints execute stage *****
    };

    class Store : public Event{
    // Store back in memory if needed
    private:
        CPU *cpu;
    public:
        Store(CPU *c) : Event(), cpu(c) {}
        virtual void process() override { cpu->process(); }
        virtual const char* description() override {return "Store";}
        void storeInstruction(); // Prints store stage *****
    };

    class Stall : public Event{
        // Store back in memory if needed
    private:
        CPU *cpu;
    public:
        Stall(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {cpu->process();}
        virtual const char* description() override {return "Stall";}
    };

    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;

    Tick clk;
    Event *e; // Used for processing a Tick Might not need for assignement 3
    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(System *s) : SimObject(s), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)) {}

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(f, currTick());
    }

    // Not sure if this is needed
    void process() {
        std::cout << "processing on Tick " << currTick() << std::endl;
        schedule(e, currTick() + clk);
    }
};

// Runs the simulatiom
class RunSim : public CPU{
public:
    RunSim(System *s) : CPU(s) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation
};

#endif //SIMOBJECT_H
