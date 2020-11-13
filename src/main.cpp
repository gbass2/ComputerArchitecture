#include "event.hh"
#include "system.hh"
#include "simobject.hh"
#include <memory>

int main(){
    auto sys = std::make_shared<System>();
    auto mem = std::make_shared<Memory>(sys);

    CPU *cpu = new CPU(sys, mem);
    RunSim *sim = new RunSim(sys, mem);

    sim->initialize(); // Creates a Setup Simulation event
    sim->runSimulation(); // Sets up the instruction memory with the instructions
    cpu->initialize(); // Sets up the first event. Which is a fetch event
    sim->runSimulation(); // Runs the instructions

    return 0;
}
