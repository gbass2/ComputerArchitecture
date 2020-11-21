#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include <memory>

int main(){
    auto sys = std::make_shared<System>();

    auto cpu0 = std::make_shared<CPU>(sys, "cpu0", 0, 0x13FF);
    // auto cpu1 = std::make_shared<CPU>(sys, "cpu1", 0, 1023);

    RunSim *sim = new RunSim(sys, cpu0);

    DRAM * ram = new DRAM(sys, AddrRange(0, 0x13FF), 20);    // 20 = response time
    Membus * bus = new Membus(sys, 5);                       // 5 = forwarding time

    // dev0->getPort()->bind(ram->getPort());

    // binding the devices port to the bus
    cpu0->getPort()->bind(bus->getUnboundCPUSidePort());
    cpu1->getPort()->bind(bus->getUnboundCPUSidePort());
    ram->getPort()->bind(bus->getUnboundCPUSidePort());

    // fill DRAM with random floating point values. 0x400 - 0xFFF
    for (auto i = ram->getAddrRange().first; i < ram->getAddrRange().second; i+=4){
         float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
         uint32_t val = *(uint32_t *)(&r);
         ram->writeWordAtAddr(i, val);
    }


    sim->initialize();        // Creates a Setup Simulation event
    sim->runSimulation();     // Sets up the instruction memory with the instructions
    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    return 0;
}
