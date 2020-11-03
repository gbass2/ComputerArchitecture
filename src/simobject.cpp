#include "simobject.hh"

using namespace std;

// Overhaul pipeline stages for assignment 3

// Fetching the instruction and putting it in the Pipeline class. Also printing it out.
void CPU::Fetch::fetchInstruction() { // Retrives the instruction from the instruction queue at the current program counter. Needs restructuring once Instructions class is implemented
}

// Prints the decode stage
void CPU::Decode::decodeInstruction() {
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

        // These stages run on odd ticks and every 5 ticks. The data is sent to the next stage on even ticks every 5 ticks.
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

        } if(!(strcmp(sys->getMEQ().front()->description(), "Stall")) && (sys->getMEQ().front()->getTime()) == currTick()){

        } if(!(strcmp(sys->getMEQ().front()->description(), "Setup Simulation")) && (sys->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory

        } if((!(strcmp(sys->getMEQ().front()->description(), "Send Data")) && (sys->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycle)*5)){
            // Check to see if each stage is done processing and if ready to send data to the next stage
        }

        incTick(1); // Increments currentTick by amount (t)

        if(currTick() % 10 == 0)
            cycle++;

    }
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
}
