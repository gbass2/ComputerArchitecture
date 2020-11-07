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
        Instruction getMemory(size_t PC) { return instructionMem[PC]; }
    };

    // Port2 holds the data memory
    class Port2 : public Event{
    private:
        DataMemory dataMemory[0x1000]; // Memory for loactions 0x200 - 0xfff. Holds locations 0 - 0x200 due to the design but will not be used
        Memory *mem;
        friend class CPU; // Allows class to access these private variables

    public:
        // Creates and processes a port1 event
        Port2(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "scheduling on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p2, mem->currTick() + mem->clkTick);
        }
        virtual const char* description() override {return "Data Memory Access"; }
        DataMemory getMemory(size_t PC) { return dataMemory[PC]; }
        void setMemory(size_t location, uint8_t data) { (dataMemory[location]).setData(data); }
    };
    Port1 *p1;
    Port2 *p2;
    friend class CPU; // Allows Send class to access these private variables

public:
    Memory(System *s) : SimObject(s), p1(new Port1(this)), p2(new Port2(this)){}
    virtual void initialize() override{ std::cout << "Memory initialization" << std::endl; }
};

class RegisterBank : public SimObject, public Register, public Event{
private:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

public:
    RegisterBank(System *s);

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
        // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
        Instruction currentInstruction1;
        Instruction currentInstruction2;
        Instruction currentInstruction3;
        Instruction currentInstruction4;
        Instruction currentInstruction; // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
        friend class CPU; // Allows CPU class to access these private variables

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
        Instruction currentInstruction;
        uint8_t rs1, rs2, rs3, rd;
        uint8_t funct2, funct3, funct5, funct7, opcode;
        uint32_t imm;
        friend class CPU; // Allows Send class to access these private variables

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
        uint8_t rs1, rs2, rs3, rd;
        uint8_t funct2, funct3, funct5, funct7, opcode;
        uint32_t imm, immDestination;
        friend class CPU; // Allows CPU class to access these private variables

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
        uint8_t rd;
        uint32_t immDestination;
        friend class CPU; // Allows CPU class to access these private variables
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
    private:
        CPU *cpu;
        size_t stallAmount;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Stall(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->schedule(cpu->stall, cpu->currTick()); // Scheduling new event
        }
        virtual const char* description() override { return "Stall"; }
        void stallCPU();
        void setAmount(size_t amount) { stallAmount = amount; }
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
        virtual const char* description() override { return "ALU"; }
        void aluOperation();

    };

    // For creating a send data event to pass data through the pipeline
    class Send : public Event{
    private:
        CPU *cpu;
    public:
        Send(CPU *c) : Event(), cpu(c) {}
        virtual void process() override { std::cout << "Send" << std::endl; }
        virtual const char* description() override { return "Send Data"; }
        void sendData();
    };

    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;
    Stall *stall;
    ALU *a;
    Send *send;
    Memory *Iport;
    Memory *Dport;

    size_t PC = 0; // Program Counter
    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(System *s) : SimObject(s), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), stall(new Stall(this)), a(new ALU(this)), send(new Send(this)), Iport(new Memory(s)), Dport(new Memory(s)) {}

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(f, currTick());
    }
};

// Runs the simulation
class RunSim : public Event, public CPU{
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
