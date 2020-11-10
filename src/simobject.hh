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
    std::shared_ptr<System> sysMain;
    Tick clkTick;

public:
    Tick currTick() { return sysMain->currTick(); } // Gets the current sysMaintem time
    void incTick(Tick t) { sysMain->incTick(t); } // Increments the sysMaintem time
    void schedule(Event *e, Tick t) { sysMain->schedule(e,t); } // Schedules an event
    void reschedule(Event *e, Tick t) { sysMain->reschedule(e,t); } // Reschedules an event
    void printMEQ() { sysMain->printMEQ(); }
    virtual void initialize() = 0; // Creates the first event
    SimObject(std::shared_ptr<System> s4) : sysMain(s4) {}
};

// Holds the instruction memory and the data memory
class Memory : public SimObject, public DataMemory, public Instruction{
private:
    // Port1 holds the instruction memory
    class Port1 : public Event{
    private:
        Instruction instructionMem[0x093]; // Instruction memory 0 - 0x093

    public:
        Memory *mem;
        // Creates and processes a port1 event
        Port1(Memory *m) : Event(), mem(m) {}
        virtual void process() override {
            std::cout << "scheduling Instruction Memory Access on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p1, mem->currTick() + 1);
        }
        virtual const char* description() override { return "Instruction Memory Access"; }
        Instruction getMemory(size_t PC) { return instructionMem[PC]; }
        void setMemory(size_t location, std::string binary, std::string type, std::string set) { (instructionMem[location]).setBinary(binary);  (instructionMem[location]).setInstType(type); (instructionMem[location]).setInstSet(type);}
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
            std::cout << "scheduling Data Memory Access on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p2, mem->currTick() + 1);
        }
        virtual const char* description() override {return "Data Memory Access"; }
        DataMemory getMemory(size_t PC) { return dataMemory[PC]; }
        void setMemory(size_t location, std::string data) { (dataMemory[location]).setData(data); }
    };
    Port1 *p1;
    Port2 *p2;
    friend class CPU; // Allows Send class to access these private variables
    friend class RunSim; // Allows Send class to access these private variables

public:
    Memory(std::shared_ptr<System> s3) : SimObject(s3), p1(new Port1(this)), p2(new Port2(this)){}
    virtual void initialize() override{ std::cout << "Memory initialization" << std::endl; }
};

class RegisterBank : public SimObject, public Register, public Event{
public:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

public:
    RegisterBank(std::shared_ptr<System> s2);

    // Scheduling the register event
    virtual void process() override{
        std::cout << "scheduling Register Access on Tick " << currTick() << std::endl;
        sysMain->schedule(this, currTick() + 1); // Scheduling new event
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
    protected:
        CPU *cpu;
        // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
        Instruction currentInstruction1;
        Instruction currentInstruction2;
        Instruction currentInstruction3;
        Instruction currentInstruction4;
        Instruction currentInstruction; // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
        bool isMemAccess = true;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Fetch(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Fetch on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->f, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Fetch";}
        void fetchInstruction(); // Gets the instruction from the instruction memory
        void setCurrentInstruction1(Instruction currentInstruction){ this->currentInstruction1 = currentInstruction; }
        void setCurrentInstruction2(Instruction currentInstruction){ this->currentInstruction2 = currentInstruction; }
        void setCurrentInstruction3(Instruction currentInstruction){ this->currentInstruction3 = currentInstruction; }
        void setCurrentInstruction4(Instruction currentInstruction){ this->currentInstruction4 = currentInstruction; }
        void setIsMemAccess(bool isMemAccess){ this->isMemAccess = isMemAccess; }
    };

    // Decode Stage
    class Decode : public Event{
    // Finds the data from the registers and passes it to the execution stage to be executed
    private:
        CPU *cpu;
        Instruction currentInstruction;
        std::string rs1, rs2, rs3, rd;
        std::string funct2, funct3, funct5, funct7, opcode;
        std::string imm;
        friend class CPU; // Allows Send class to access these private variables

    public:
        Decode(CPU *c) : Event(), cpu(c){}
        virtual void process() override {
        std::cout << "Scheduling Decode on Tick " << cpu->currTick() << std::endl;
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
        std::string rs1, rs2, rs3, rd;
        std::string funct2, funct3, funct5, funct7, opcode;
        std::string imm, immDestination;
        std::string result;
        bool isMemAccess = false;
        Instruction currentInstruction;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Execute on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override {return "Execute";}
        void executeInstruction();
        void setIsMemAccess(bool isMemAccess){ this->isMemAccess = isMemAccess; }
        void setInstruction(Instruction currentInstruction) { this->currentInstruction = currentInstruction; }
        Instruction getInstruction() { return currentInstruction;}

    };

    // Store Stage
    class Store : public Event{
    // Store back in memory if needed
    private:
        CPU *cpu;
        std::string rd;
        std::string immDestination;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Store(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Store on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
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
        bool isStalled = false;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Stall(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling stall on Tick " << cpu->currTick() << std::endl;
        std::cout << "Stalling for " << stallAmount + 2 << " ticks\n" << std::endl;
        cpu->schedule(cpu->stall, cpu->currTick() + 1); // Scheduling new event
        }
        virtual const char* description() override { return "Stall"; }
        void stallCPU();
        void setAmount(size_t amount) { stallAmount = amount; }
        bool getIsStalled(){ return isStalled; }
        void setIsStalled(bool isStalled){ this->isStalled = isStalled; }
    };

    // ALU for executing the instructions
    class ALU : public Event{
    private:
        CPU *cpu;
        std::deque<int> jump;
        // friend class CPU; // Allows CPU class to access these private variables
        friend class RunSim; // Allows RunSim class to access these private variables

    public:
        ALU(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "scheduling Alu on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
        cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        virtual const char* description() override { return "ALU"; }
        void aluOperations();
        void ADDI();
        void SLLI();
        void SW();
        void FSW();
        void FADDS();
        void J();
        void LW();
        void BLT();
        void LUI();
        void FLW();
        void ADD();
        void RET();
        void JALR();
    };

    // For creating a send data event to pass data through the pipeline
    class Send : public Event{
    private:
        CPU *cpu;
    public:
        Send(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
            std::cout << "scheduling Send Data on Tick " << cpu->currTick() << std::endl;
            cpu->schedule(cpu->send, cpu->currTick() + 5); // Scheduling new event
        }
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
    RegisterBank *reg;

    size_t PC = 0; // Program Counter
    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(std::shared_ptr<System> s1) : SimObject(s1), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), stall(new Stall(this)), a(new ALU(this)), send(new Send(this)), Iport(new Memory(s1)), Dport(new Memory(s1)), reg(new RegisterBank(s1)){}

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(f, currTick());
    }
};

// Runs the simulation
class RunSim : public Event, public CPU{
public:
    RunSim(std::shared_ptr<System> s) :  Event(), CPU(s) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation
    void setupSimulator(); // Sets up the instruction memory with the instructions from a file

    virtual void process() override{
        std::cout << "scheduling Setup Simulation on Tick " << currTick() << std::endl;
        sysMain->schedule(this, 0); // Scheduling new event

    }
    virtual const char* description() override {return "Setup Simulation";}
    virtual void initialize() override { // Initialzes MEQ with a fetch event
        process();
    }
};

#endif //SIMOBJECT_H
