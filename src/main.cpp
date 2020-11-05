#include "event.hh"
#include "system.hh"
#include "simobject.hh"

int main(){
    System *sys = new System();
    CPU *cpu = new CPU(sys);
    RunSim *sim = new RunSim(sys);
    sim->initialize(); // Filling the instruction memory locations in with the instructions from the assembly files
    sim->runSimulation();
    cpu->initialize(); // Sets up the first event. Which is a fetch event
    sim->runSimulation();
    
    return 0;
}
