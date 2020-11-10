#include "simobject.hh"
#include <cstring>
#include <cassert>

using namespace std;

// Creating the integer registers and floating point registers using a hash table to hold the register and its name
RegisterBank::RegisterBank(std::shared_ptr<System> s2): SimObject(s2) {
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

    if(isMemAccess){
        cout << "Fetch Stage" << endl << endl;
        // cpu->Iport->p1->process(); // The code does not like how mem is accessing the simobject
        cpu->schedule(cpu->Iport->p1, cpu->currTick() + 1);
        cpu->stall->isStalled = true;
        return;
    }

    // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
    currentInstruction.setBinary(currentInstruction1.getBinary() + currentInstruction2.getBinary() + currentInstruction3.getBinary() + currentInstruction4.getBinary());
    currentInstruction.setInstType(currentInstruction1.getInstType());

    cout << "instruction fetched: "  << currentInstruction.getBinary() << endl;
    cout << "instruction Type: "  << currentInstruction.getInstType() << endl;

    cpu->PC += 4;
}

// Deodes the instructions into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    cout << "Decode Stage" << endl << endl;
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
    cpu->a->process();
}

// Prints the store instruction
void CPU::Store::storeInstruction() {
    // Create a register access events
    // Store back into register
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
    cpu->ex->rs2 = cpu->d->rs2;
    cpu->ex->rs3 = cpu->d->rs3;
    cpu->ex->rd = cpu->d->rd;
    cpu->ex->funct2 = cpu->d->funct2;
    cpu->ex->funct3 = cpu->d->funct3;
    cpu->ex->funct5 = cpu->d->funct5;
    cpu->ex->funct7= cpu->d->funct7;
    cpu->ex->opcode = cpu->d->opcode;
    cpu->ex->imm = cpu->d->imm;
    cpu->ex->setInstruction(cpu->d->currentInstruction);

    // Passing the rd of execute to the store stage
    cpu->s->rd = cpu->ex->rd;
    cpu->s->immDestination = cpu->ex->immDestination;

}

// Main function for running the simulation
void RunSim::runSimulation(){
    clkTick = 10; // How far in advance that the event is going to be scheduled
    size_t cycles = 0; // Cpu cycle count
    double numInstructions = 37; // Remove once ret is implemented

    while(!(sysMain->getMEQ()).empty() && Iport->p1->getMemory(PC).getBinary() != "11100110"){
        printMEQ();
        cout << "Current Cycle: " << cycles + 1 << endl;

        if ((sysMain->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);

        // These stages run on odd ticks and every 10 ticks. The data is sent to the next stage on even ticks every 10 ticks.
        // Schedules accesses the store stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Send Data")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycles)*10)){
            send->sendData();
            sysMain->removeEvent();

        } if(!sysMain->getMEQ().empty() && (!(strcmp(sysMain->getMEQ().front()->description(), "Stall")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Stall the processor
            sysMain->removeEvent();
            stall->stallCPU();
            stall->setIsStalled(0);
            // Set isStalled to 0 where ever the stall event was created the stall event

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Register Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Access the registers associated with the instruction and place into decode stage
            sysMain->removeEvent();

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Instruction Memory Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
            f->setCurrentInstruction1(Iport->p1->getMemory(PC));
            f->setCurrentInstruction2(Iport->p1->getMemory(PC + 1));
            f->setCurrentInstruction3(Iport->p1->getMemory(PC + 2));
            f->setCurrentInstruction4(Iport->p1->getMemory(PC + 3));


            sysMain->removeEvent();
            f->setIsMemAccess(false);

            // Creating the memory latency by stalling
            stall->setAmount(18);
            stall->process();

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Data Memory Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){
            // Stall the processor for a memory access
            sysMain->removeEvent();

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "ALU")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycles)*10)){
            a->aluOperations();
            sysMain->removeEvent();

        // Schedules and accesses the execute stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Execute")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycles)*10)){
            ex->executeInstruction();
            // // if(stall->getIsStalled() == false){
            //     // // s->process();
                sysMain->removeEvent();
            //     //
            //     if(ex->getInstruction().getInstSet() == "Base"){
            //         // Creating the Instruction latency by stalling
            //         stall->setIsStalled(1);
            //         stall->setAmount(8);
            //         stall->process();
            //     }
            // // }

        // Schedules and access the decode stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Decode")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycles)*10)){
            d->decodeInstruction();
            sysMain->removeEvent();

            // if(stall->getIsStalled() == false)
            //     ex->process();

        // Schedules and access the fetch stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Fetch")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 1 + (cycles)*10)){
            f->fetchInstruction();

            if(stall->getIsStalled() == false){
                // Creating a send data event
                send->process();

                // Scheduling decode for next instruction
                d->process();

                // Scheduling fetch for next instruction
                f->process();

                f->setIsMemAccess(true);
            }

        } if(!(strcmp(sysMain->getMEQ().front()->description(), "Setup Simulation")) && (sysMain->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory
            sysMain->removeEvent();
        }

        if(currTick() % 10 == 0)
            cycles++;

        incTick(1); // Increments currentTick by amount (t)
    }

    if(!(currTick() == 1))
        cout << "CPI: " << (cycles+1)/numInstructions << endl;
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
    Iport->p1->setMemory(0, "11001000", "I", "Base"); // 2
    Iport->p1->setMemory(1, "10000000", "I", "Base");
    Iport->p1->setMemory(2, "10000000", "I", "Base");
    Iport->p1->setMemory(3, "11111111", "I", "Base");
    Iport->p1->setMemory(4, "11000100", "S", "Base"); // 3
    Iport->p1->setMemory(5, "01100100", "S", "Base");
    Iport->p1->setMemory(6, "10001000", "S", "Base");
    Iport->p1->setMemory(7, "00000000", "S", "Base");
    Iport->p1->setMemory(8, "11000100", "S", "Base"); // 4
    Iport->p1->setMemory(9, "00100100", "S", "Base");
    Iport->p1->setMemory(10, "10000001", "S","Base");
    Iport->p1->setMemory(11, "00001000", "S", "Base");
    Iport->p1->setMemory(12, "11001000", "I", "Base"); // 5
    Iport->p1->setMemory(13, "00100000", "I", "Base");
    Iport->p1->setMemory(14, "10000000", "I", "Base");
    Iport->p1->setMemory(15, "10000000", "I", "Base");
    Iport->p1->setMemory(16, "11001000", "I", "Base"); // 6
    Iport->p1->setMemory(17, "10100000", "I", "Base");
    Iport->p1->setMemory(18, "00000000", "I", "Base");
    Iport->p1->setMemory(19, "00000000", "I", "Base");
    Iport->p1->setMemory(20, "11000100", "S", "Base"); // 7
    Iport->p1->setMemory(21, "01010100", "S", "Base");
    Iport->p1->setMemory(22, "00100101", "S", "Base");
    Iport->p1->setMemory(23, "01111111", "S", "Base");
    Iport->p1->setMemory(24, "11000100", "S", "Base"); // 8
    Iport->p1->setMemory(25, "00010100", "S", "Base");
    Iport->p1->setMemory(26, "00100101", "S", "Base");
    Iport->p1->setMemory(27, "01111111", "S", "Base");
    Iport->p1->setMemory(28, "11110110", "J", "Base"); // 9
    Iport->p1->setMemory(29, "00000000", "J", "Base");
    Iport->p1->setMemory(30, "00000000", "J", "Base");
    Iport->p1->setMemory(31, "01000000", "J", "Base");
    Iport->p1->setMemory(32, "11000000", "I", "Base"); // 11
    Iport->p1->setMemory(33, "10100100", "I", "Base");
    Iport->p1->setMemory(34, "00100000", "I", "Base");
    Iport->p1->setMemory(35, "11111111", "I", "Base");
    Iport->p1->setMemory(36, "11001001", "I", "Base"); // 12
    Iport->p1->setMemory(37, "10100000", "I", "Base");
    Iport->p1->setMemory(38, "00001111", "I", "Base");
    Iport->p1->setMemory(39, "11110000", "I", "Base");
    Iport->p1->setMemory(40, "11000110", "B", "Base"); // 13
    Iport->p1->setMemory(41, "00000011", "B", "Base");
    Iport->p1->setMemory(42, "10100101", "B", "Base");
    Iport->p1->setMemory(43, "00010000", "B", "Base");
    Iport->p1->setMemory(44, "11110110", "J", "Base"); // 14
    Iport->p1->setMemory(45, "00000000", "J", "Base");
    Iport->p1->setMemory(46, "00000000", "J", "Base");
    Iport->p1->setMemory(47, "01100000", "J", "Base");
    Iport->p1->setMemory(48, "11101100", "U", "Base"); // 16
    Iport->p1->setMemory(49, "10100000", "U", "Base");
    Iport->p1->setMemory(50, "00000000", "U", "Base");
    Iport->p1->setMemory(51, "00000000", "U", "Base");
    Iport->p1->setMemory(52, "11001000", "I", "Base"); // 17
    Iport->p1->setMemory(53, "10100000", "I", "Base");
    Iport->p1->setMemory(54, "10100000", "I", "Base");
    Iport->p1->setMemory(55, "00000010", "I", "Base");
    Iport->p1->setMemory(56, "11000001", "I", "Base"); // 18
    Iport->p1->setMemory(57, "10100100", "I", "Base");
    Iport->p1->setMemory(58, "00100000", "I", "Base");
    Iport->p1->setMemory(59, "11111111", "I", "Base");
    Iport->p1->setMemory(60, "11001001", "R", "Base"); // 19
    Iport->p1->setMemory(61, "10101001", "R", "Base");
    Iport->p1->setMemory(62, "10100100", "R", "Base");
    Iport->p1->setMemory(63, "00000000", "R", "Base");
    Iport->p1->setMemory(64, "11001100", "R", "Base"); // 20
    Iport->p1->setMemory(65, "10100000", "R", "Base");
    Iport->p1->setMemory(66, "10101101", "R", "Base");
    Iport->p1->setMemory(67, "00000000", "R", "Base");
    Iport->p1->setMemory(68, "11100000", "I", "Float"); // 21 ffffffffffffffffffffffffffffffffffffffffff
    Iport->p1->setMemory(69, "00000100", "I", "Float");
    Iport->p1->setMemory(70, "10100000", "I", "Float");
    Iport->p1->setMemory(71, "00000000", "I", "Float");
    Iport->p1->setMemory(72, "11101100", "U", "Base"); // 22
    Iport->p1->setMemory(73, "10100000", "U", "Base");
    Iport->p1->setMemory(74, "00000000", "U", "Base");
    Iport->p1->setMemory(75, "00000000", "U", "Base");
    Iport->p1->setMemory(76, "11001000", "I", "Base"); // 23
    Iport->p1->setMemory(77, "10100000", "I", "Base");
    Iport->p1->setMemory(78, "10100000", "I", "Base");
    Iport->p1->setMemory(79, "00000001", "I", "Base");
    Iport->p1->setMemory(80, "11001100", "R", "Base"); // 24
    Iport->p1->setMemory(81, "10100000", "R", "Base");
    Iport->p1->setMemory(82, "10101101", "R", "Base");
    Iport->p1->setMemory(83, "00000000", "R", "Base");
    Iport->p1->setMemory(84, "11100001", "I", "Float"); // 25 ffffffffffffffffffffffffffffffffffffffff
    Iport->p1->setMemory(85, "00000100", "I", "Float");
    Iport->p1->setMemory(86, "10100000", "I", "Float");
    Iport->p1->setMemory(87, "00000000", "I", "Float");
    Iport->p1->setMemory(88, "11001010", "R", "Float"); // 26 fffffffffffffffffffffffffffffffffff
    Iport->p1->setMemory(89, "00001110", "R", "Float");
    Iport->p1->setMemory(90, "00001000", "R", "Float");
    Iport->p1->setMemory(91, "00000000", "R", "Float");
    Iport->p1->setMemory(92, "11101100", "U", "Base"); // 27
    Iport->p1->setMemory(93, "10100000", "U", "Base");
    Iport->p1->setMemory(94, "00000000", "U", "Base");
    Iport->p1->setMemory(95, "00000000", "U", "Base");
    Iport->p1->setMemory(96, "11001000", "I", "Base"); // 28
    Iport->p1->setMemory(97, "10100000", "I", "Base");
    Iport->p1->setMemory(98, "10100000", "I", "Base");
    Iport->p1->setMemory(99, "00000011", "I", "Base");
    Iport->p1->setMemory(100, "11001100", "R", "Base"); // 29
    Iport->p1->setMemory(101, "10100000", "R", "Base");
    Iport->p1->setMemory(102, "10101101", "R", "Base");
    Iport->p1->setMemory(103, "00000000", "R", "Base");
    Iport->p1->setMemory(104, "11001010", "S", "Float"); // 30 ffffffffffffffffffffffffffffffffffffffffffff
    Iport->p1->setMemory(105, "00000100", "S", "Float");
    Iport->p1->setMemory(106, "10100000", "S", "Float");
    Iport->p1->setMemory(107, "00000000", "S", "Float");
    Iport->p1->setMemory(108, "11110110", "J", "Base"); // 31
    Iport->p1->setMemory(109, "00000000", "J", "Base");
    Iport->p1->setMemory(110, "00000000", "J", "Base");
    Iport->p1->setMemory(111, "01110000", "J", "Base");
    Iport->p1->setMemory(112, "11000000", "I", "Base"); // 33
    Iport->p1->setMemory(113, "10100100", "I", "Base");
    Iport->p1->setMemory(114, "00100000", "I", "Base");
    Iport->p1->setMemory(115, "11111111", "I", "Base");
    Iport->p1->setMemory(116, "11001000", "I", "Base"); // 34
    Iport->p1->setMemory(117, "10100000", "I", "Base");
    Iport->p1->setMemory(118, "10101000", "I", "Base");
    Iport->p1->setMemory(119, "00000000", "I", "Base");
    Iport->p1->setMemory(120, "11000100", "S", "Base"); // 35
    Iport->p1->setMemory(121, "00001000", "S", "Base");
    Iport->p1->setMemory(122, "01001010", "S", "Base");
    Iport->p1->setMemory(123, "11111111", "S", "Base");
    Iport->p1->setMemory(124, "11110110", "J", "Base"); // 36
    Iport->p1->setMemory(125, "00000000", "J", "Base");
    Iport->p1->setMemory(126, "00000000", "J", "Base");
    Iport->p1->setMemory(127, "01000000", "J", "Base");
    Iport->p1->setMemory(128, "11000000", "I", "Base"); // 38
    Iport->p1->setMemory(129, "10100100", "I", "Base");
    Iport->p1->setMemory(130, "00100010", "I", "Base");
    Iport->p1->setMemory(131, "11111111", "I", "Base");
    Iport->p1->setMemory(132, "11000000", "I", "Base"); // 39
    Iport->p1->setMemory(133, "00100100", "I", "Base");
    Iport->p1->setMemory(134, "10000001", "I", "Base");
    Iport->p1->setMemory(135, "00000000", "I", "Base");
    Iport->p1->setMemory(136, "11000001", "I", "Base"); // 40
    Iport->p1->setMemory(137, "00000100", "I", "Base");
    Iport->p1->setMemory(138, "10000011", "I", "Base");
    Iport->p1->setMemory(139, "00000000", "I", "Base");
    Iport->p1->setMemory(140, "11001000", "I", "Base"); // 41
    Iport->p1->setMemory(141, "10000000", "I", "Base");
    Iport->p1->setMemory(142, "10000000", "I", "Base");
    Iport->p1->setMemory(143, "10000000", "I", "Base");
    Iport->p1->setMemory(144, "11100110", "I", "Base"); // 42
    Iport->p1->setMemory(145, "00000001", "I", "Base");
    Iport->p1->setMemory(146, "00000000", "I", "Base");
    Iport->p1->setMemory(147, "00000000", "I", "Base");

    a->jump.push_back(0x20);
    a->jump.push_back(0x30);
    a->jump.push_back(0x70);
    a->jump.push_back(0x80);

}
