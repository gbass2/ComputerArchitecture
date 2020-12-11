#include "simobject.hh"
#include <cstring>
#include <cassert>

using namespace std;

// Main function for running the simulation
void RunSim::runSimulation(){

    while(!(sysMain->getMEQ().empty())){
        // Counting the cycles
        if(currTick() % 10 == 0 && currTick() > 1){
            cycles++;

            if(!sysMain->isFinished0)
                sysMain->cycles0++;
            if(!sysMain->isFinished0)
                sysMain->cycles1++;
        }

        printMEQ();

        cout << "Current Cycle: " << cycles  << endl;
        cout << "Current Tick: " << currTick() << endl;

        if ((sysMain->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        } else if(!(sysMain->getMEQ().empty()) && (sysMain->getMEQ().front()->getTime()) == currTick()){
            while(!(sysMain->getMEQ().empty()) && (sysMain->getMEQ().front()->getTime()) == currTick()){
                sysMain->popEvent()->process();
            }
        }
        incTick(1); // Increments currentTick by amount (t)
    }
}
