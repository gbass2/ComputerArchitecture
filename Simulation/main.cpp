#include "event.hh"
#include "system.hh"
#include "simobject.hh"

int main(){
    System *sys = new System();
    CPU *cpu = new CPU(sys);
    RunSim *sim = new RunSim(sys);
    cpu->initialize();
    sim->runSimulation(); // Remove the the end time for assignment 3 bc its not needed

    return 0;
}
