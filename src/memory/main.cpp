#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>

// int main(){
//     auto sys = std::make_shared<System>();
//
//     CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x93, 0x94, 0x13FF); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory
//
//     RunSim *sim = new RunSim(sys);
//
//     DRAM * ram = new DRAM(sys, AddrRange(0x94, 0x13FF), 20);    // Data memory. 20 = response time
//     DRAM * Iram = new DRAM(sys, AddrRange(0, 0x93), 20);        // Instruction memory 20 = response time
//     Membus * bus1 = new Membus(sys, 5);                         // Membus for Instruction memory. 5 = forwarding time
//     Membus * bus2 = new Membus(sys, 5);                         // Membus for data memory. 5 = forwarding time
//
//     // Binding the device's ports to the bus
//     cpu0->getPort1()->bind(bus1->getUnboundCPUSidePort()); // Binding cpu0 instruction port (por2) to membus
//     ram->getPort()->bind(bus1->getUnboundMemSidePort());  // Binding instruction memory membus
//     cpu0->getPort2()->bind(bus2->getUnboundCPUSidePort()); // Binding cpu0 data port (port1) to membus
//     Iram->getPort()->bind(bus2->getUnboundMemSidePort()); // Binding data memory membus
//
//     // Fill DRAM with random floating point values. 0x400 - 0xFFF
//     for (auto i = ram->getAddrRange().first + 1024; i < ram->getAddrRange().second - 0x400; i+=4){
//          float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//          uint32_t val = *(uint32_t *)(&r);
//          ram->writeWordAtAddr(i, val);
//     }
//
//     cpu0->initialize();       // Sets up the first event. Which is a fetch event
//     sim->runSimulation();     // Runs the instructions
//
//     return 0;
// }

#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>

int main(){
    auto sys = std::make_shared<System>();

    CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x93, 0, 1023); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory

    RunSim *sim = new RunSim(sys);

    DRAM * ram = new DRAM(sys, AddrRange(0, 1023), 20);    // Data memory. 20 = response time
    // DRAM * Iram = new DRAM(sys, AddrRange(0, 0x93), 20);        // Instruction memory 20 = response time
    // Membus * bus1 = new Membus(sys, 5);                         // Membus for Instruction memory. 5 = forwarding time
    Membus * bus2 = new Membus(sys, 5);                         // Membus for data memory. 5 = forwarding time

    // Binding the device's ports to the bus
    // cpu0->getPort1()->bind(bus1->getUnboundCPUSidePort()); // Binding cpu0 instruction port (por2) to membus
    // Iram->getPort()->bind(bus1->getUnboundMemSidePort());  // Binding instruction memory membus
    cpu0->getPort2()->bind(bus2->getUnboundCPUSidePort()); // Binding cpu0 data port (port1) to membus
    ram->getPort()->bind(bus2->getUnboundMemSidePort()); // Binding data memory membus

    // Fill DRAM with random floating point values. 0x400 - 0xFFF
    for (auto i = ram->getAddrRange().first; i < ram->getAddrRange().second; i+=4){
         float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
         uint32_t val = *(uint32_t *)(&r);
         ram->writeWordAtAddr(i, val);
    }

    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    return 0;
}
