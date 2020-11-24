#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>

void setupSimulator(CPU *cpu, DRAM *ram);

int main(){
    auto sys = std::make_shared<System>();

    CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x93, 0x200, 0x13FF); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory

    RunSim *sim = new RunSim(sys);

    DRAM * ram = new DRAM(sys, AddrRange(0x200, 0x13FF), 20);    // Data memory. 20 = response time
    DRAM * Iram = new DRAM(sys, AddrRange(0, 0x93), 20);        // Instruction memory 20 = response time
    Membus * bus = new Membus(sys, 5);                         //  5 = forwarding time
    // Membus * bus2 = new Membus(sys, 5);                         // Membus for data memory. 5 = forwarding time

    // Binding the device's ports to the bus
    Iram->getPort()->bind(bus->getUnboundMemSidePort()); // Binding Instruction memory membus
    cpu0->getPort1()->bind(bus->getUnboundCPUSidePort()); // Binding cpu0 instruction port (por2) to membus
    ram->getPort()->bind(bus->getUnboundMemSidePort());  // Binding instruction memory membus
    cpu0->getPort2()->bind(bus->getUnboundCPUSidePort()); // Binding cpu0 data port (port1) to membus

    // Fill DRAM with random floating point values. 0x400 - 0xFFF
    for (auto i = ram->getAddrRange().first; i < ram->getAddrRange().second - 0x400; i+=4){
         float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
         uint32_t val = *(uint32_t *)(&r);
         ram->writeWordAtAddr(i, val);
    }

    setupSimulator(cpu0, Iram);
    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    return 0;
}

void setupSimulator(CPU* cpu, DRAM *ram){ // Sets up the instruction memory with the instructions
    std::cout << "Placing the instructions into memory" << std::endl << std::endl;

    ram->writeAtAddr(0, 0b11001000100000001000000011111111); // 2
    ram->writeAtAddr(0, 0b11000100011001001000100000000000); // 3
    ram->writeAtAddr(0, 0b11000100001001001000000100001000); // 4
    ram->writeAtAddr(0, 0b11001000001000001000000010000000); // 5
    ram->writeAtAddr(0, 0b11001000101000000000000000000000); // 6
    ram->writeAtAddr(0, 0b11000100010101000010010101111111); // 7
    ram->writeAtAddr(0, 0b11000100000101000010010101111111); // 8
    ram->writeAtAddr(0, 0b11110110000000000000000001000000); // 9
    ram->writeAtAddr(0, 0b11000000101001000010000011111111); // 11
    ram->writeAtAddr(0, 0b11001001101000000000111111110000); // 12
    ram->writeAtAddr(0, 0b11000110000000111010010100010000); // 13
    ram->writeAtAddr(0, 0b11110110000000000000000001100000); // 14
    ram->writeAtAddr(0, 0b11101100101000000000000000000000); // 16
    ram->writeAtAddr(0, 0b11001000101000001010000000000010); // 17
    ram->writeAtAddr(0, 0b11000001101001000010000011111111); // 18
    ram->writeAtAddr(0, 0b11001001101010011010010000000000); // 19
    ram->writeAtAddr(0, 0b11001100101000001010110100000000); // 20
    ram->writeAtAddr(0, 0b11100000000001001010000000000000); // 21
    ram->writeAtAddr(0, 0b11101100101000000000000000000000); // 22
    ram->writeAtAddr(0, 0b11001000101000001010000000000001); // 23
    ram->writeAtAddr(0, 0b11001000101000001010000000000001); // 24
    ram->writeAtAddr(0, 0b11001100101000001010110100000000); // 25
    ram->writeAtAddr(0, 0b11001010000011100000100000000000); // 26
    ram->writeAtAddr(0, 0b11101100101000000000000000000000); // 27
    ram->writeAtAddr(0, 0b11001000101000001010000000000011); // 28
    ram->writeAtAddr(0, 0b11001100101000001010110100000000); // 29
    ram->writeAtAddr(0, 0b11001010000001001010000000000000); // 30
    ram->writeAtAddr(0, 0b11110110000000000000000001110000); // 31
    ram->writeAtAddr(0, 0b11000000101001000010000011111111); // 33
    ram->writeAtAddr(0, 0b11001000101000001010100000000000); // 34
    ram->writeAtAddr(0, 0b11000100000010000100101011111111); // 35
    ram->writeAtAddr(0, 0b11110110000000000000000001000000); // 36
    ram->writeAtAddr(0, 0b11000000101001000010001011111111); // 38
    ram->writeAtAddr(0, 0b11000000001001001000000100000000); // 39
    ram->writeAtAddr(0, 0b11000001000001001000001100000000); // 40
    ram->writeAtAddr(0, 0b11001000100000001000000010000000); // 42
    ram->writeAtAddr(0, 0b11100110000000010000000000000000); //

    // Setting the branching locations
    cpu->getALU()->setJump(0x20);
    cpu->getALU()->setJump(0x30);
    cpu->getALU()->setJump(0x70);
    cpu->getALU()->setJump(0x80);
}
