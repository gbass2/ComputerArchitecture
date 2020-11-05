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
        friend class CPU; // Allows class to access these private variables

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
        friend class CPU; // Allows class to access these private variables

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
    friend class CPU; // Allows class to access these private variables

public:
    Memory(System *s) : SimObject(s), p1(new Port1(this)), p2(new Port2(this)){}
    virtual void initialize() override{ std::cout << "Memory initialization" << std::endl; }
};

class RegisterBank : public SimObject, public Register, public Event{
private:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

public:
    RegisterBank(System *s): SimObject(s){
        intRegisters.insert(std::pair<uint8_t,Register>(00000, Register()));    // x0 zero Hard-Wired Zero
        intRegisters.insert(std::pair<uint8_t,Register>(00001, Register()));    // x1 ra Return Address
        intRegisters.insert(std::pair<uint8_t,Register>(00010, Register()));    // x2 sp Stack Pointer
        intRegisters.insert(std::pair<uint8_t,Register>(00011, Register()));    // x3 gp Global Pointer
        intRegisters.insert(std::pair<uint8_t,Register>(00100, Register()));    // x4 tp Thread Pointer
        intRegisters.insert(std::pair<uint8_t,Register>(00101, Register()));    // x5 t0 Temporary/alternate Link Register
        intRegisters.insert(std::pair<uint8_t,Register>(00110, Register()));    // x6 t1 Temporaries
        intRegisters.insert(std::pair<uint8_t,Register>(00111, Register()));    // x7 t2 Temporaries
        intRegisters.insert(std::pair<uint8_t,Register>(01000, Register()));    // x8 s0/fp Saved Register/Frame Pointer
        intRegisters.insert(std::pair<uint8_t,Register>(01001, Register()));    // x9 s1 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(01010, Register()));    // x10 a0 Function Arguments/Return Values
        intRegisters.insert(std::pair<uint8_t,Register>(01011, Register()));    // x11 a1 Function Arguments/Return Values
        intRegisters.insert(std::pair<uint8_t,Register>(01100, Register()));    // x12 a2 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(01101, Register()));    // x13 a3 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(01110, Register()));    // x14 a4 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(01111, Register()));    // x15 a5 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(10000, Register()));    // x16 a6 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(10001, Register()));    // x17 a7 Function Arguments
        intRegisters.insert(std::pair<uint8_t,Register>(10010, Register()));    // x18 s2 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(10011, Register()));    // x19 s3 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(10100, Register()));    // x20 s4 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(10101, Register()));    // x21 s5 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(10110, Register()));    // x22 s6 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(10111, Register()));    // x23 s7 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(11000, Register()));    // x24 s8 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(11001, Register()));    // x25 s9 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(11010, Register()));    // x26 s10 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(11011, Register()));    // x27 s11 Saved Register
        intRegisters.insert(std::pair<uint8_t,Register>(11100, Register()));    // x28 t3 Temporaries
        intRegisters.insert(std::pair<uint8_t,Register>(11101, Register()));    // x29 t4 Temporaries
        intRegisters.insert(std::pair<uint8_t,Register>(11110, Register()));    // x30 t5 Temporaries
        intRegisters.insert(std::pair<uint8_t,Register>(11111, Register()));    // x31 t6 Temporaries

        fpRegisters.insert(std::pair<uint8_t,Register>(00000, Register()));    // f0 ft0 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00010, Register()));    // f2 ft2 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00011, Register()));    // f3 ft3 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00100, Register()));    // f4 ft4 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00101, Register()));    // f5 ft5 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00110, Register()));    // f6 ft6 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(00111, Register()));    // f7 ft7 Temporary
        fpRegisters.insert(std::pair<uint8_t,Register>(01000, Register()));    // f8 fs0 Saved Register
        fpRegisters.insert(std::pair<uint8_t,Register>(01001, Register()));    // f9 fs1 Saved Register
        fpRegisters.insert(std::pair<uint8_t,Register>(01010, Register()));    // f10 fa0 FP arguments/return values
        fpRegisters.insert(std::pair<uint8_t,Register>(01011, Register()));    // f11 fa1 FP arguments/return values
        fpRegisters.insert(std::pair<uint8_t,Register>(01100, Register()));    // f12 fa2 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(01101, Register()));    // f13 fa3 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(01110, Register()));    // f14 fa4 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(01111, Register()));    // f15 fa5 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(10000, Register()));    // f16 fa6 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(10001, Register()));    // f17 fa7 FP arguments
        fpRegisters.insert(std::pair<uint8_t,Register>(10010, Register()));    // f18 fs2 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(10011, Register()));    // f19 fs3 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(10100, Register()));    // f20 fs4 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(10101, Register()));    // f21 fs5 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(10110, Register()));    // f22 fs6 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(10111, Register()));    // f23 fs7 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(11000, Register()));    // f24 fs8 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(11001, Register()));    // f25 fs9 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(11010, Register()));    // f26 fs10 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(11011, Register()));    // f27 fs11 FP saved registers
        fpRegisters.insert(std::pair<uint8_t,Register>(11100, Register()));    // f28 ft8 FP temporaries
        fpRegisters.insert(std::pair<uint8_t,Register>(11101, Register()));    // f29 ft9 FP temporaries
        fpRegisters.insert(std::pair<uint8_t,Register>(11110, Register()));    // f30 ft10 FP temporaries
        fpRegisters.insert(std::pair<uint8_t,Register>(11111, Register()));    // f31 ft11 FP temporaries
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
        // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
        Instruction currentInstruction1;
        Instruction currentInstruction2;
        Instruction currentInstruction3;
        Instruction currentInstruction4;
        Instruction currentInstruction; // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
        friend class RunSim; // Allows RunSim class to access these private variables

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
        friend class RunSim; // Allows RunSim class to access these private variables

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
        friend class RunSim; // Allows RunSim class to access these private variables


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
        friend class RunSim; // Allows RunSim class to access these private variables
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

    // For creating a send data event to pass data through the pipeline
    class Send : public Event{
    public:
        virtual const char* description() override {return "Send Data";}
    };

    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;
    ALU *a;
    Memory *Iport;
    Memory *Dport;

    size_t PC = 0; // Program Counter
    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(System *s) : SimObject(s), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), a(new ALU(this)), Iport(new Memory(s)), Dport(new Memory(s)) {}

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
