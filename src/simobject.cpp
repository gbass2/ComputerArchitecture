#include "simobject.hh"
#include <cstring>

using namespace std;

// Creating the integer registers and floating point registers
RegisterBank::RegisterBank(System *s): SimObject(s) {
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
    Memory::Port1 *p1 = cpu->Iport->p1;
    string binary;

    // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
    currentInstruction1 = p1->getMemory(cpu->PC);
    currentInstruction2 = p1->getMemory(cpu->PC + 1);
    currentInstruction3 = p1->getMemory(cpu->PC + 2);
    currentInstruction4 = p1->getMemory(cpu->PC + 3);

    // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
    binary = to_string(currentInstruction1.getBinary() + currentInstruction2.getBinary() + currentInstruction3.getBinary() + currentInstruction4.getBinary());
    currentInstruction.setBinary(stoi(binary));

    cpu->PC += 4;
}

// Deodes the instructions into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    string instruction = to_string(currentInstruction.getBinary());
    if(currentInstruction.getInstType() == "R"){
        opcode = stoi(instruction.substr(0,6));
        rd = stoi(instruction.substr(7,5));
        funct3 = stoi(instruction.substr(12,3));
        rs1 = stoi(instruction.substr(15,5));
        rs2 = stoi(instruction.substr(20,5));
        funct7 = stoi(instruction.substr(25,7));

    } else if(currentInstruction.getInstType() == "I"){
        opcode = stoi(instruction.substr(0,6));
        rd = stoi(instruction.substr(7,5));
        funct3 = stoi(instruction.substr(12,3));
        rs1 = stoi(instruction.substr(15,5));
        imm = stoi(instruction.substr(20,12));

    } else if(currentInstruction.getInstType() == "S"){
        opcode = stoi(instruction.substr(0,6));
        imm = stoi(instruction.substr(7,5) + instruction.substr(25,7));
        funct3 = stoi(instruction.substr(12,3));
        rs1 = stoi(instruction.substr(15,5));
        rs2 = stoi(instruction.substr(20,5));

    } else if(currentInstruction.getInstType() == "U"){
        opcode = stoi(instruction.substr(0,6));
        rd = stoi(instruction.substr(7,5));
        imm = stoi(instruction.substr(12,19));

    } else if(currentInstruction.getInstType() == "B"){
        opcode = stoi(instruction.substr(0,6));
        imm = stoi(instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
        funct3 = stoi(instruction.substr(12,3));
        rs1 = stoi(instruction.substr(15,5));
        rs2 = stoi(instruction.substr(20,5));

    } else if(currentInstruction.getInstType() == "J"){
        opcode = stoi(instruction.substr(0,6));
        rd = stoi(instruction.substr(15,5));
        imm = stoi(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
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
    cout << "hello" << endl;
    for(size_t i = 0; i < (cpu->sys->getMEQ()).size(); i++){
        cpu->reschedule((cpu->sys->getMEQ()).at(i), cpu->currTick() + stallAmount);
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

    while(!(sys->getMEQ()).empty()){
        printMEQ();
        if ((sys->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        }
        // These stages run on odd ticks and every 10 ticks. The data is sent to the next stage on even ticks every 10 ticks.
        // Schedules accesses the store stage
        if((!(strcmp(sys->getMEQ().front()->description(), "Store")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // s->storeInstruction();
            // sys->removeEvent();

        // Schedules and accesses the execute stage
        } if((!(strcmp(sys->getMEQ().front()->description(), "Execute")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // ex->executeInstruction();
            // s->process();

        // Schedules and access the decode stage
        } if((!(strcmp(sys->getMEQ().front()->description(), "Decode")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){
            // d->decodeInstruction();
            // ex->process();
            // cout << "odd: " << currTick() << endl;

        // Schedules and access the fetch stage
        } if((!(strcmp(sys->getMEQ().front()->description(), "Fetch")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycle)*10)){

            // f->fetchInstruction();


            // // Scheduling decode for next instruction
            // d->process();
            // // Scheduling fetch for next instruction
            // f->process();

        } if((!(strcmp(sys->getMEQ().front()->description(), "Register Access")) && (sys->getMEQ().front()->getTime()) == currTick())){
            // Access the registers associated with the instruction and place into decode stage
            sys->removeEvent();

        } if((!(strcmp(sys->getMEQ().front()->description(), "Instruction Memory Access")) && (sys->getMEQ().front()->getTime()) == currTick())){
            sys->removeEvent();

        } if((!(strcmp(sys->getMEQ().front()->description(), "Data Memory Access")) && (sys->getMEQ().front()->getTime()) == currTick())){
            // Stall the processor for a memory access
            sys->removeEvent();

        } if((!(strcmp(sys->getMEQ().front()->description(), "Send Data")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycle)*5)){
            send->sendData();
            sys->removeEvent();

        } if(!(strcmp(sys->getMEQ().front()->description(), "Setup Simulation")) && (sys->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory
            sys->removeEvent();
            stall->setAmount(10);
            stall->process();


        } if(!(strcmp(sys->getMEQ().front()->description(), "Stall")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // Stall the processor
            stall->stallCPU();
            sys->removeEvent();
        }

        if(currTick() % 10 == 0)
            cycle++;

        incTick(1); // Increments currentTick by amount (t)
    }
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
}
