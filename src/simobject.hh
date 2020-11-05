#ifndef SIMOBJECT_H
#define SIMOBJECT_H
#define Tick uint64_t

#include <vector>
#include <deque>
#include <iostream>
#include <tuple>
#include <string>
#include <unordered_map>
#include "event.hh"
#include "system.hh"
#include "memory.hh"

class SimObject {
protected:
    System *sys;
    Tick clkTick;

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
class Memory : public SimObject, public DataMemory, public Instruction{
private:
    // Port1 holds the instruction memory
    class Port1 : public Event{
    private:
        Instruction instructionMem[0x093]; // Instruction memory 0 - 0x093
        Memory *mem;

    public:
        // Creates and processes a port1 event
        Port1(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "processing on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p1, mem->currTick() + mem->clkTick);
        }
        virtual const char* description() override { return "Instruction Memory Access"; }
    };

    // Port2 holds the data memory
    class Port2 : public Event{
    private:
        DataMemory dataMemory[0x1000]; // Memory for loactions 0x200 - 0xfff. Holds locations 0 - 0x200 due to the design but will not be used
        Memory *mem;

    public:
        // Creates and processes a port1 event
        Port2(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "scheduling on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p2, mem->currTick() + mem->clkTick);
        }
        virtual const char* description() override {return "Data Memory Access"; }
    };
    Port1 *p1;
    Port2 *p2;
    friend class CPU; // Allows Store class to access these private variables

public:
    Memory(System *s) : SimObject(s), p1(new Port1(this)), p2(new Port2(this)){}
    virtual void initialize() override{ std::cout << "Memory initialization" << std::endl; }
};

class RegisterBank : public SimObject, public Register, public Event{
private:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    // Register intRegisters[32];  // Bank of 32 integer registers
    // Register fpRegisters[32];   // Bank of 32 floating point registers

public:
    RegisterBank(System *s): SimObject(s){
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0001, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0010, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0011, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0100, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0101, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0110, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0111, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1001, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1010, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1011, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1100, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1101, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(1110, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
        // intRegisters.insert(std::pair<uint8_t,Register>(0000, Register()));
    }

    // Scheduling the register event
    virtual void process() override{
        std::cout << "scheduling on Tick " << currTick() << std::endl;
        sys->schedule(this, currTick() + 1); // Scheduling new event
    }

    virtual const char* description() override {return "Register Access";}
    virtual void initialize() override { // Initialzes MEQ with a fetch event
        std::cout << "Register Access" << std::endl;
    }
};

class CPU: public SimObject{
private:
    // Pipeline stages
    // Fetch Stage
    class Fetch : public Event{
    private:
        CPU *cpu;
        Instruction currentInstruction;

    public:
        Fetch(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->sys->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->f, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Fetch";}
        void fetchInstruction(); // Gets the instruction from the instruction memory
    };

    // Decode Stage
    class Decode : public Event{
    // Finds the data from the registers and passes it to the execution stage to be executed
    private:
        CPU *cpu;
        uint32_t rs1, rs2, rs3, rd;
        uint8_t fucnt2, funct3, funct5, funct7;
        uint32_t imm[32];


    public:
        Decode(CPU *c) : Event(), cpu(c){}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->schedule(cpu->d, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Decode";}
        void decodeInstruction();
    };

    // Execute Stage
    class Execute : public Event{
    // Passes the incoming registers or memory location to the ALU to be operated
    private:
        CPU *cpu;
        Instruction currentInstruction;
        uint32_t rs1, rs2, rs3, rd;
        uint8_t fucnt2, funct3, funct5, funct7;
        uint32_t imm[32];

    public:
        Execute(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->sys->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Execute";}
        void executeInstruction();
    };

    // Store Stage
    class Store : public Event{
    // Store back in memory if needed
    private:
        CPU *cpu;
        uint32_t destination;
    public:
        Store(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->sys->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick);
        }
        virtual const char* description() override {return "Store";}
        void storeInstruction();
    };

    // Stalls the pipeline
    class Stall : public Event{
        // Store back in memory if needed
    private:
        CPU *cpu;
    public:
        Stall(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->sys->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Stall";}
    };

    // ALU for executing the instructions
    class ALU : public Event{
    private:
        CPU *cpu;
    public:
        ALU(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "scheduling on Tick " << cpu->currTick() << std::endl;
        cpu->sys->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "ALU";}

    };

    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;
    ALU *a;
    Memory *Iport;
    Memory *Dport;

    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(System *s) : SimObject(s), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), a(new ALU(this)), Iport(new Memory(s)), Dport(new Memory(s)) {}

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(f, currTick());
    }
};

// Runs the simulation
class RunSim : public Event, public CPU{
private:
    size_t PC = 0; // Program Counter

public:
    RunSim(System *s) :  Event(), CPU(s) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation
    void setupSimulator(); // Sets up the instruction memory with the instructions from a file

    virtual void process() override{
        std::cout << "scheduling on Tick " << currTick() << std::endl;
        sys->schedule(this, 0); // Scheduling new event

    }
    virtual const char* description() override {return "Setup Simulation";}
    virtual void initialize() override { // Initialzes MEQ with a fetch event
        process();
    }
};

#endif //SIMOBJECT_H
