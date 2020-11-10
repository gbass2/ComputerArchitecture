#include "event.hh"
#include "system.hh"
#include "simobject.hh"

int main(){
    auto sys = std::shared_ptr<System>(new System());
    CPU *cpu = new CPU(sys);
    RunSim *sim = new RunSim(sys);

    sim->initialize(); // Filling the instruction memory locations in with the instructions from the assembly files
    sim->runSimulation(); // Sets up the instruction memory with the instructions
    cpu->initialize(); // Sets up the first event. Which is a fetch event
    sim->runSimulation(); // Runs the instructions

    return 0;
}
