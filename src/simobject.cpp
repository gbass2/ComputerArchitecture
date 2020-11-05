#include "simobject.hh"
#include <cassert>
#include <cstring>

using namespace std;

// Fetches the instruction from memory
void CPU::Fetch::fetchInstruction() {
    Instruction *instructionMem = cpu->Iport->p1->instructionMem;
    string binary;
    // Since a 4th of the instruction is held in one memory location, we need to pull 4 memory locations for one instruction
    currentInstruction1 = instructionMem[cpu->PC];
    currentInstruction2 = instructionMem[cpu->PC+1];
    currentInstruction3 = instructionMem[cpu->PC+2];
    currentInstruction4 = instructionMem[cpu->PC+3];

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

}

// Prints the execute stage
void CPU::Execute::executeInstruction() {

}

// Prints the store instruction
void CPU::Store::storeInstruction() {

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

        } if((!(strcmp(sys->getMEQ().front()->description(), "Register Access")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycle)*5)){
            // Access the registers associated with the instruction and place into decode stage
            sys->removeEvent();

        } if((!(strcmp(sys->getMEQ().front()->description(), "Send Data")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycle)*5)){
            d->currentInstruction = f->currentInstruction; // Passing fetch to decode

            // Passing decode to execute
            ex->rs1 = d->rs1;
            ex->rs2 = d->rs1;
            ex->rs3 = d->rs1;
            ex->rd = d->rd;
            ex->funct2 = d->funct2;
            ex->funct3 = d->funct3;
            ex->funct5 = d->funct5;
            ex->funct7= d->funct7;
            ex->opcode = d->opcode;
            ex->imm = d->imm;

            // Passing the rd of execute to the store stage
            s->rd = ex->rd;
            s->immDestination = ex->immDestination;

            sys->removeEvent();

        } if(!(strcmp(sys->getMEQ().front()->description(), "Setup Simulation")) && (sys->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory

        } if(!(strcmp(sys->getMEQ().front()->description(), "Stall")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // Stall the processor
        }

        if(currTick() % 10 == 0)
            cycle++;
        if(currTick() < 1) // Removing the setup simulation event
            sys->removeEvent();

        incTick(1); // Increments currentTick by amount (t)
    }
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
}
