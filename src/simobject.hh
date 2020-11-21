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



class RegisterBank : public SimObject, public Register, public Event{
private:
    bool registerAccess = false;
public:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

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
    void setRegiserAccess(bool registerAccess) { this->registerAccess = registerAccess; }
    bool getRegisterAccess() { return registerAccess; }
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
        // Instruction currentInstruction; // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
        bool isMemAccess = true;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Fetch(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Fetch on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed

        if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            cpu->schedule(cpu->f, cpu->currTick() + cpu->clkTick); // Scheduling new event
        else
            cpu->schedule(cpu->f, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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
        Instruction currentInstruction1;
        Instruction currentInstruction2;
        Instruction currentInstruction3;
        Instruction currentInstruction4;
        std::bitset<32> rs1, rs2, rs3;
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        std::bitset<32> result;
        std::bitset<3> funct3;
        std::bitset<7> opcode, funct7;
        std::string funct2, funct5, imm;
        friend class CPU; // Allows Send class to access these private variables

    public:
        Decode(CPU *c) : Event(0), cpu(c){}
        virtual void process() override {
        std::cout << "Scheduling Decode on Tick " << cpu->currTick() << std::endl;

        if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            cpu->schedule(cpu->d, cpu->currTick() + cpu->clkTick); // Scheduling new event
        else
            cpu->schedule(cpu->d, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
        }
        virtual const char* description() override {return "Decode";}
        void decodeInstruction();
    };

    // Execute Stage
    class Execute : public Event{
    // Passes the incoming registers or memory location to the ALU to be operated
    private:
        CPU *cpu;
        std::bitset<32> rs1, rs2, rs3;
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        std::bitset<32> result;
        std::bitset<3> funct3;
        std::bitset<7> opcode, funct7;
        std::string funct2, funct5, imm;
        bool isMemAccess = false;
        Instruction currentInstruction;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Execute on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed

        if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
        else
            cpu->schedule(cpu->ex, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Store(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling Store on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed

        if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
        else
            cpu->schedule(cpu->s, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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
        Stall(CPU *c) : Event(1), cpu(c) {}
        virtual void process() override {
        std::cout << "Scheduling stall on Tick " << cpu->currTick() << std::endl;
        std::cout << "Stalling for " << stallAmount << " ticks\n" << std::endl;
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
        ALU(CPU *c) : Event(1), cpu(c) {}
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
    RegisterBank *reg;

    size_t PC = 0; // Program Counter
    size_t cycles = 0; // Cpu cycle count
    friend class RunSim; // Allows RunSim class to access these private variables

public:
    CPU(std::shared_ptr<System> s1) : SimObject(s1), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), stall(new Stall(this)), a(new ALU(this)), send(new Send(this)), reg(new RegisterBank(s1)){}

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
