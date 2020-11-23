#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>

int main(){
    auto sys = std::make_shared<System>();

    CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x13FF);

    RunSim *sim = new RunSim(sys);

    DRAM * ram = new DRAM(sys, AddrRange(0, 0x13FF), 20);    // 20 = response time
    Membus * bus = new Membus(sys, 5);                       // 5 = forwarding time

    // binding the devices port to the bus
    cpu0->getPort()->bind(bus->getUnboundCPUSidePort());
    ram->getPort()->bind(bus->getUnboundMemSidePort());

    // fill DRAM with random floating point values. 0x400 - 0xFFF
    for (auto i = ram->getAddrRange().first + 1024; i < ram->getAddrRange().second - 0x400; i+=4){
         float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
         uint32_t val = *(uint32_t *)(&r);
         ram->writeWordAtAddr(i, val);
         std::cout << "hello" << i << std::endl;
    }

    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    return 0;
}
