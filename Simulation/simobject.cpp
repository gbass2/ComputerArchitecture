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
    clk = 10; // How far in advance that the event is going to be scheduled

    while(!(sys->getMEQ()).empty()){
        printMEQ();
        if ((sys->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        }

        // Schedules accesses the store stage
        if(!(strcmp(sys->getMEQ().front()->description(), "Store")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // s->storeInstruction();
            // sys->removeEvent();

        // Schedules and accesses the execute stage
        } if(!(strcmp(sys->getMEQ().front()->description(), "Execute")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // ex->executeInstruction();
            // s->setpipelineOpcode(ex->getExecuteStage().getpipelineOpcode());
            // s->setRegisters(ex->getExecuteStage().getRegisters());
            // s->process();

        // Schedules and access the decode stage
        } if(!(strcmp(sys->getMEQ().front()->description(), "Decode")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // d->decodeInstruction();
            // ex->setpipelineOpcode(d->getDecodeStage().getpipelineOpcode());
            // ex->setRegisters(d->getDecodeStage().getRegisters());
            // ex->process();

        // Schedules and access the fetch stage
        } if(!(strcmp(sys->getMEQ().front()->description(), "Fetch")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // f->fetchInstruction();
            // d->setpipelineOpcode(f->getFetchStage().getpipelineOpcode());
            // d->setRegisters(f->getFetchStage().getRegisters());

            // Scheduling decode for next instruction
            // d->process();
            // Scheduling fetch for next instruction
            // f->process();

        // Reading from file and placing the instructions in memeory
        } if(!(strcmp(sys->getMEQ().front()->description(), "Stall")) && (sys->getMEQ().front()->getTime()) == currTick()){

        } if(!(strcmp(sys->getMEQ().front()->description(), "Setup Simulation")) && (sys->getMEQ().front()->getTime()) == currTick()){
            setupSimulator(); // load the instructions into memory
        }

        incTick(1); // Increments currentTick by amount (t)
    }
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
}
