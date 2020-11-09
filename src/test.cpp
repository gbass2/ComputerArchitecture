#include <vector>
#include <deque>
#include <iostream>
#include <tuple>
#include <string>
#include <unordered_map>
#include "event.hh"
#include "system.hh"
#include "memory.hh"
using namespace std;

#define Tick uint64_t

class SimObject {
public:
    std::shared_ptr<System> sysMain;
    Tick clkTick;

public:
    Tick currTick() { return sysMain->currTick(); } // Gets the current sysMaintem time
    void incTick(Tick t) { sysMain->incTick(t); } // Increments the sysMaintem time
    void schedule(Event *e, Tick t) { sysMain->schedule(e,t); } // Schedules an event
    void reschedule(Event *e, Tick t) { sysMain->reschedule(e,t); } // Reschedules an event
    void printMEQ() { sysMain->printMEQ(); }
    virtual void initialize() = 0; // Creates the first event
    SimObject(std::shared_ptr<System> sys5) : sysMain(sys5) {}
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
            std::cout << "scheduling on Tick " << mem->currTick() << std::endl;
            mem->schedule(mem->p1, mem->currTick());
        }
        virtual const char* description() override { return "Instruction Memory Access"; }
        Instruction getMemory(size_t PC) { return instructionMem[PC]; }
        void setMemory(size_t location, std::string binary, std::string type) { (instructionMem[location]).setBinary(binary);  (instructionMem[location]).setInstType(type);}
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
            mem->schedule(mem->p2, mem->currTick());
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
    Memory(std::shared_ptr<System> sys4) : SimObject(sys4), p1(new Port1(this)), p2(new Port2(this)){}
    virtual void initialize() override{ std::cout << "Memory initialization" << std::endl; }
    void print(){
        cout << sysMain << endl;
        // cout << "sys Memory: " << s << endl;
    }
};

class RegisterBank : public SimObject, public Register, public Event{
private:
    std::unordered_map<uint8_t, Register> intRegisters; // Example: <0101,"Temporary/alternate link register">
    std::unordered_map<uint8_t, Register> fpRegisters; // Example: <0101,"Temporary/alternate link register">

public:
    RegisterBank(std::shared_ptr<System> sys6);

    // Scheduling the register event
    virtual void process() override{
        std::cout << "scheduling on Tick " << currTick() << std::endl;
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
    private:
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
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
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
        std::string rs1, rs2, rs3, rd;
        std::string funct2, funct3, funct5, funct7, opcode;
        std::string imm, immDestination;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
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
        std::string rd;
        std::string immDestination;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Store(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
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
        std::cout << "processing on Tick " << cpu->currTick() << std::endl;
        cpu->schedule(cpu->stall, cpu->currTick()); // Scheduling new event
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
    public:
        ALU(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
        std::cout << "scheduling on Tick " << cpu->currTick() << std::endl;
        cpu->sysMain->removeEvent(); // removing event that was just executed
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
    CPU(std::shared_ptr<System> sys3) : SimObject(sys3), f(new Fetch(this)), d(new Decode(this)), ex(new Execute(this)), s(new Store(this)), stall(new Stall(this)), a(new ALU(this)), send(new Send(this)), Iport(new Memory(sys3)), Dport(new Memory(sys3)) {}

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(f, currTick());
    }

    void print(){
        // cout << "sys3 CPU&: " << sys3 << endl;
        cout << sysMain << endl;
    }
};

// Runs the simulation
class RunSim : public Event, public CPU{
public:
    RunSim(std::shared_ptr<System> sys2) :  Event(), CPU(sys2) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation
    void setupSimulator(); // Sets up the instruction memory with the instructions from a file

    virtual void process() override{
        std::cout << "scheduling on Tick " << currTick() << std::endl;
        sysMain->schedule(this, 0); // Scheduling new event

    }
    virtual const char* description() override {return "Setup Simulation";}
    virtual void initialize() override { // Initialzes MEQ with a fetch event
        process();
    }
    void print() {
        cout << sysMain << endl;
        // cout << "sys RunSim: " << s << endl;
    }
};

// Creating the integer registers and floating point registers
RegisterBank::RegisterBank(std::shared_ptr<System> sys6): SimObject(sys6) {
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
    fpRegisters.insert(std::pair<uint8_t,Register>(00001, Register()));    // f1 ft1 Temporary
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

// Fetches the instruction from memory
void CPU::Fetch::fetchInstruction() {
    string binary;

    // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
    if(isMemAccess){
        cpu->Iport->p1->process();
        cpu->stall->isStalled = true;
        return;
    }

    // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
    currentInstruction.setBinary(currentInstruction1.getBinary() + currentInstruction2.getBinary() + currentInstruction3.getBinary() + currentInstruction4.getBinary());
    currentInstruction.setInstType(currentInstruction1.getInstType());

    cout << "instruction Bianry: "  << currentInstruction.getBinary() << endl;
    cout << "instruction Type: "  << currentInstruction.getInstType() << endl;

    cpu->PC += 4;
}

// Deodes the instructions into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    string instruction = currentInstruction.getBinary();
    if(currentInstruction.getInstType() == "R"){
        opcode = (instruction.substr(0,6));
        rd = (instruction.substr(7,5));
        funct3 = (instruction.substr(12,3));
        rs1 = (instruction.substr(15,5));
        rs2 = (instruction.substr(20,5));
        funct7 = (instruction.substr(25,7));

    } else if(currentInstruction.getInstType() == "I"){
        opcode = (instruction.substr(0,6));
        rd = (instruction.substr(7,5));
        funct3 = (instruction.substr(12,3));
        rs1 = (instruction.substr(15,5));
        imm = (instruction.substr(20,12));

    } else if(currentInstruction.getInstType() == "S"){
        opcode = (instruction.substr(0,6));
        imm = (instruction.substr(7,5) + instruction.substr(25,7));
        funct3 = (instruction.substr(12,3));
        rs1 = (instruction.substr(15,5));
        rs2 = (instruction.substr(20,5));

    } else if(currentInstruction.getInstType() == "U"){
        opcode = (instruction.substr(0,6));
        rd = (instruction.substr(7,5));
        imm = (instruction.substr(12,19));

    } else if(currentInstruction.getInstType() == "B"){
        opcode = (instruction.substr(0,6));
        imm = (instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
        funct3 = (instruction.substr(12,3));
        rs1 = (instruction.substr(15,5));
        rs2 = (instruction.substr(20,5));

    } else if(currentInstruction.getInstType() == "J"){
        opcode = (instruction.substr(0,6));
        rd = (instruction.substr(15,5));
        imm = (instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
    }

    // determine if the decoded instruction reads from a register that the instruction currently in the execution stage writes to
    // If so then stall the processor
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {

}

// Prints the store instruction
void CPU::Store::storeInstruction() {

}

void CPU::Stall::stallCPU() {
    // Rescheduling the events for a specified time in the future. 1 stall = 10 ticks, 2 stalls = 20 ticks, etc.
    for(size_t i = 0; i < (cpu->sysMain->getMEQ()).size(); i++){
        cpu->reschedule((cpu->sysMain->getMEQ()).at(i), cpu->currTick() + stallAmount);
    }
}

void CPU::Send::sendData() {
    cpu->d->currentInstruction = cpu->f->currentInstruction; // Passing fetch to decode

    // Passing decode to execute
    cpu->ex->rs1 = cpu->d->rs1;
    cpu->ex->rs2 = cpu->d->rs1;
    cpu->ex->rs3 = cpu->d->rs1;
    cpu->ex->rd = cpu->d->rd;
    cpu->ex->funct2 = cpu->d->funct2;
    cpu->ex->funct3 = cpu->d->funct3;
    cpu->ex->funct5 = cpu->d->funct5;
    cpu->ex->funct7= cpu->d->funct7;
    cpu->ex->opcode = cpu->d->opcode;
    cpu->ex->imm = cpu->d->imm;

    // Passing the rd of execute to the store stage
    cpu->s->rd = cpu->ex->rd;
    cpu->s->immDestination = cpu->ex->immDestination;

}

// Main function for running the simulation
void RunSim::runSimulation(){
    clkTick = 10; // How far in advance that the event is going to be scheduled
    size_t cycle = 0; // Cpu cycle count

    while(!(sysMain->getMEQ()).empty()){
        printMEQ();
        if ((sysMain->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        }
        // These stages run on odd ticks and every 10 ticks. The data is sent to the next stage on even ticks every 10 ticks.
        // Schedules accesses the store stage
        if((!(strcmp(sysMain->getMEQ().front()->description(), "Store")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // s->storeInstruction();
            // if(stall->getIsStalled() == false)
            //     sysMain->removeEvent();

        // Schedules and accesses the execute stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Execute")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // ex->executeInstruction();
            // if(stall->getIsStalled() == false)
            //     s->process();

        // Schedules and access the decode stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Decode")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // d->decodeInstruction();
            // if(stall->getIsStalled() == false)
            //     ex->process();

        // Schedules and access the fetch stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Fetch")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            f->fetchInstruction();

            // // Scheduling decode for next instruction
            // if(stall->getIsStalled() == false)
            //     d->process();
            // // Scheduling fetch for next instruction
            if(stall->getIsStalled() == false){
                f->process();

            f->setIsMemAccess(true);
            }

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Register Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Access the registers associated with the instruction and place into decode stage
            sysMain->removeEvent();

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Instruction Memory Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            f->setCurrentInstruction1(Iport->p1->getMemory(PC));
            f->setCurrentInstruction2(Iport->p1->getMemory(PC + 1));
            f->setCurrentInstruction3(Iport->p1->getMemory(PC + 2));
            f->setCurrentInstruction4(Iport->p1->getMemory(PC + 3));

            sysMain->removeEvent();
            f->setIsMemAccess(false);
            stall->process();
            stall->setAmount(20);

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Data Memory Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Stall the processor for a memory access
            sysMain->removeEvent();

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Send Data")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycle)*5)){
            send->sendData();
            sysMain->removeEvent();

        } if(!(strcmp(sysMain->getMEQ().front()->description(), "Setup Simulation")) && (sysMain->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory
            sysMain->removeEvent();

        } if(!sysMain->getMEQ().empty() && (!(strcmp(sysMain->getMEQ().front()->description(), "Stall")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Stall the processor
            sysMain->removeEvent();
            stall->stallCPU();
            stall->setIsStalled(1);
            // Set isStalled to 0 where ever the stall event was created the stall event
        }

        if(currTick() % 10 == 0)
            cycle++;

        incTick(1); // Increments currentTick by amount (t)
    }
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
    // Need to set the instruction type for each instruction.
    Iport->p1->setMemory(0, "11001000", "I"); // 2
    Iport->p1->setMemory(1, "10000000", "I");
    Iport->p1->setMemory(2, "10000000", "I");
    Iport->p1->setMemory(3, "11111111", "I");
    Iport->p1->setMemory(4, "11000100", "S"); // 3
    Iport->p1->setMemory(5, "01100100", "S");
    Iport->p1->setMemory(6, "10001000", "S");
    Iport->p1->setMemory(7, "00000000", "S");
    Iport->p1->setMemory(8, "11000100", "S"); // 4
    Iport->p1->setMemory(9, "00100100", "S");
    Iport->p1->setMemory(10, "10000001", "S");
    Iport->p1->setMemory(11, "00001000", "S");
    Iport->p1->setMemory(12, "11001000", "I"); // 5
    Iport->p1->setMemory(13, "00100000", "I");
    Iport->p1->setMemory(14, "10000000", "I");
    Iport->p1->setMemory(15, "10000000", "I");
    Iport->p1->setMemory(16, "11001000", "I"); // 6
    Iport->p1->setMemory(17, "10100000", "I");
    Iport->p1->setMemory(18, "00000000", "I");
    Iport->p1->setMemory(19, "00000000", "I");
    Iport->p1->setMemory(20, "11000100", "S"); // 7
    Iport->p1->setMemory(21, "01010100", "S");
    Iport->p1->setMemory(22, "00100101", "S");
    Iport->p1->setMemory(23, "01111111", "S");
    Iport->p1->setMemory(24, "11000100", "S"); // 8
    Iport->p1->setMemory(25, "00010100", "S");
    Iport->p1->setMemory(26, "00100101", "S");
    Iport->p1->setMemory(27, "01111111", "S");
    Iport->p1->setMemory(28, "11110110", "J"); // 9
    Iport->p1->setMemory(29, "00000000", "J");
    Iport->p1->setMemory(30, "00000000", "J");
    Iport->p1->setMemory(31, "01000000", "J");
    Iport->p1->setMemory(32, "11000000", "I"); // 11
    Iport->p1->setMemory(33, "10100100", "I");
    Iport->p1->setMemory(34, "00100000", "I");
    Iport->p1->setMemory(35, "11111111", "I");
    Iport->p1->setMemory(36, "11001001", "I"); // 12
    Iport->p1->setMemory(37, "10100000", "I");
    Iport->p1->setMemory(38, "00001111", "I");
    Iport->p1->setMemory(39, "11110000", "I");
    Iport->p1->setMemory(40, "11000110", "B"); // 13
    Iport->p1->setMemory(41, "00000011", "B");
    Iport->p1->setMemory(42, "10100101", "B");
    Iport->p1->setMemory(43, "00010000", "B");
    Iport->p1->setMemory(44, "11110110", "J"); // 14
    Iport->p1->setMemory(45, "00000000", "J");
    Iport->p1->setMemory(46, "00000000", "J");
    Iport->p1->setMemory(47, "01100000", "J");
    Iport->p1->setMemory(48, "11101100", "U"); // 16
    Iport->p1->setMemory(49, "10100000", "U");
    Iport->p1->setMemory(50, "00000000", "U");
    Iport->p1->setMemory(51, "00000000", "U");
    Iport->p1->setMemory(52, "11001000", "I"); // 17
    Iport->p1->setMemory(53, "10100000", "I");
    Iport->p1->setMemory(54, "10100000", "I");
    Iport->p1->setMemory(55, "11000000", "I");
    Iport->p1->setMemory(56, "11000001", "I"); // 18
    Iport->p1->setMemory(57, "10100100", "I");
    Iport->p1->setMemory(58, "00100000", "I");
    Iport->p1->setMemory(59, "11111111", "I");
    Iport->p1->setMemory(60, "11001001", "R"); // 19
    Iport->p1->setMemory(61, "10101001", "R");
    Iport->p1->setMemory(62, "10100100", "R");
    Iport->p1->setMemory(63, "00000000", "R");
    Iport->p1->setMemory(64, "11001100", "R"); // 20
    Iport->p1->setMemory(65, "10100000", "R");
    Iport->p1->setMemory(66, "10101101", "R");
    Iport->p1->setMemory(67, "00000000", "R");
    Iport->p1->setMemory(68, "11100000", "I"); // 21
    Iport->p1->setMemory(69, "00000100", "I");
    Iport->p1->setMemory(70, "10100000", "I");
    Iport->p1->setMemory(71, "00000000", "I");
    Iport->p1->setMemory(72, "11101100", "U"); // 22
    Iport->p1->setMemory(73, "10100000", "U");
    Iport->p1->setMemory(74, "00000000", "U");
    Iport->p1->setMemory(75, "00000000", "U");
    Iport->p1->setMemory(76, "11001000", "I"); // 23
    Iport->p1->setMemory(77, "10100000", "I");
    Iport->p1->setMemory(78, "10100000", "I");
    Iport->p1->setMemory(79, "00000001", "I");
    Iport->p1->setMemory(80, "11001100", "R"); // 24
    Iport->p1->setMemory(81, "10100000", "R");
    Iport->p1->setMemory(82, "10101101", "R");
    Iport->p1->setMemory(83, "00000000", "R");
    Iport->p1->setMemory(84, "11100001", "I"); // 25
    Iport->p1->setMemory(85, "00000100", "I");
    Iport->p1->setMemory(86, "10100000", "I");
    Iport->p1->setMemory(87, "00000000", "I");
    Iport->p1->setMemory(88, "11001010", "R"); // 26
    Iport->p1->setMemory(89, "00001110", "R");
    Iport->p1->setMemory(90, "00001000", "R");
    Iport->p1->setMemory(91, "00000000", "R");
    Iport->p1->setMemory(92, "11101100", "U"); // 27
    Iport->p1->setMemory(93, "10100000", "U");
    Iport->p1->setMemory(94, "00000000", "U");
    Iport->p1->setMemory(95, "00000000", "U");
    Iport->p1->setMemory(96, "11001000", "I"); // 28
    Iport->p1->setMemory(97, "10100000", "I");
    Iport->p1->setMemory(98, "10100000", "I");
    Iport->p1->setMemory(99, "00000011", "I");
    Iport->p1->setMemory(100, "11001100", "R"); // 29
    Iport->p1->setMemory(101, "10100000", "R");
    Iport->p1->setMemory(102, "10101101", "R");
    Iport->p1->setMemory(103, "00000000", "R");
    Iport->p1->setMemory(104, "11001010", "S"); // 30
    Iport->p1->setMemory(105, "00000100", "S");
    Iport->p1->setMemory(106, "10100000", "S");
    Iport->p1->setMemory(107, "00000000", "S");
    Iport->p1->setMemory(108, "11110110", "J"); // 31
    Iport->p1->setMemory(109, "00000000", "J");
    Iport->p1->setMemory(110, "00000000", "J");
    Iport->p1->setMemory(111, "01110000", "J");
    Iport->p1->setMemory(112, "11000000", "I"); // 33
    Iport->p1->setMemory(113, "10100100", "I");
    Iport->p1->setMemory(114, "00100000", "I");
    Iport->p1->setMemory(115, "11111111", "I");
    Iport->p1->setMemory(116, "11001000", "I"); // 34
    Iport->p1->setMemory(117, "10100000", "I");
    Iport->p1->setMemory(118, "10101000", "I");
    Iport->p1->setMemory(119, "00000000", "I");
    Iport->p1->setMemory(120, "11000100", "S"); // 35
    Iport->p1->setMemory(121, "00001000", "S");
    Iport->p1->setMemory(122, "01001010", "S");
    Iport->p1->setMemory(123, "11111111", "S");
    Iport->p1->setMemory(124, "11110110", "J"); // 36
    Iport->p1->setMemory(125, "00000000", "J");
    Iport->p1->setMemory(126, "00000000", "J");
    Iport->p1->setMemory(127, "01000000", "J");
    Iport->p1->setMemory(128, "11000000", "I"); // 38
    Iport->p1->setMemory(129, "10100100", "I");
    Iport->p1->setMemory(130, "00100010", "I");
    Iport->p1->setMemory(131, "11111111", "I");
    Iport->p1->setMemory(132, "11000000", "I"); // 39
    Iport->p1->setMemory(133, "00100100", "I");
    Iport->p1->setMemory(134, "10000001", "I");
    Iport->p1->setMemory(135, "00000000", "I");
    Iport->p1->setMemory(136, "11000001", "I"); // 40
    Iport->p1->setMemory(137, "00000100", "I");
    Iport->p1->setMemory(138, "10000011", "I");
    Iport->p1->setMemory(139, "00000000", "I");
    Iport->p1->setMemory(140, "11001000", "I"); // 41
    Iport->p1->setMemory(141, "10000000", "I");
    Iport->p1->setMemory(142, "10000000", "I");
    Iport->p1->setMemory(143, "10000000", "I");
    Iport->p1->setMemory(144, "11100110", "I"); // 42
    Iport->p1->setMemory(145, "00000001", "I");
    Iport->p1->setMemory(146, "00000000", "I");
    Iport->p1->setMemory(147, "00000000", "I");
}




















int main(){
    auto sys = std::shared_ptr<System>(new System());

    CPU *cpu = new CPU(sys);
    RunSim *sim = new RunSim(sys);
    Memory *mem = new Memory(sys);
    // sim->initialize(); // Filling the instruction memory locations in with the instructions from the assembly files
    // sim->runSimulation(); // Sets up the instruction memory with the instructions
    // cpu->initialize(); // Sets up the first event. Which is a fetch event
    // sim->runSimulation(); // Runs the instructions

    cout <<cpu->sysMain << endl;
    cout << sys << endl;
    sim->print();
    cpu->print();
    mem->print();

    // sim->print();
    // cpu->print();

    return 0;
}
