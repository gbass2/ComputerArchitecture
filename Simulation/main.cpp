#include "event.hh"
#include "system.hh"
#include "simobject.hh"

int main(){
    System *sys = new System();
    CPU *cpu = new CPU(sys);
    RunSim *sim = new RunSim(sys);
    // cpu->initialize(); // Sets up the first event. Which is a fetch event
    sim->setupSimulator(); // Reading the contents of the file and filling the memory
    sim->runSimulation();

    return 0;
}
