#include "simobject.hh"
#include <cstring>
#include <cassert>

using namespace std;

// Main function for running the simulation
void RunSim::runSimulation(){
    double numInstructions = 37; // Remove once ret is implemented

    while(!(sysMain->getMEQ()).empty()){
        // Counting the cycles
        if(currTick() % 10 == 0 && currTick() > 1)
            cycles++;

        printMEQ();

        cout << "Current Cycle: " << cycles  << endl;
        cout << "Current Tick: " << currTick() << endl;

        if ((sysMain->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        // These stages run on odd ticks and every 10 ticks. The data is sent to the next stage on even ticks every 10 ticks.
        // Schedules accesses the store stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Send Data")) && (sysMain->getMEQ().front()->getTime()) == currTick()) && (currTick() == 6 + (cycles)*10)){

        } if(!sysMain->getMEQ().empty() && (!(strcmp(sysMain->getMEQ().front()->description(), "Stall")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Register Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Memory Access")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        } if((!(strcmp(sysMain->getMEQ().front()->description(), "ALU")) && (sysMain->getMEQ().front()->getTime()) == currTick())){


        // Schedules and accesses the execute stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Execute")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        // Schedules and access the decode stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Decode")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        // Schedules and access the fetch stage
        } if((!(strcmp(sysMain->getMEQ().front()->description(), "Fetch")) && (sysMain->getMEQ().front()->getTime()) == currTick())){

        }

        incTick(1); // Increments currentTick by amount (t)
    }

    if(!(currTick() == 1))
        cout << "CPI: " << (cycles+1)/numInstructions << endl;
}
