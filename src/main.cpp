#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>

void setupSimulator(CPU *cpu, DRAM *ram1, DRAM *ram2);

int main(){
    auto sys = std::make_shared<System>();

    size_t stackEnd0 = 0x2FF;
    size_t stackBegin0 = 0x200;
    size_t stackEnd1 = 0x3FF;
    size_t stackBegin1 = 0x300;
    size_t dramEnd = 0x13FF;
    std::cout <<  "stack end: " << stackEnd0 << std::endl;
    std::cout << "stack begin: " << stackBegin0 << std::endl;

    CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x93, stackBegin0, dramEnd); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory
    CPU *cpu1 = new CPU(sys, "cpu1", 0x100, 0x193, stackBegin1, dramEnd); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory

    RunSim *sim = new RunSim(sys);

    DRAM * ram = new DRAM(sys, AddrRange(0x200, 0x13FF), 20);    // Data memory. 20 = response time
    DRAM * Iram1 = new DRAM(sys, AddrRange(0, 0x93), 20);         // Instruction memory for cpu0. 20 = response time
    DRAM * Iram2 = new DRAM(sys, AddrRange(0x100, 0x193), 20);         // Instruction memory for cpu1. 20 = response time
    Membus * bus = new Membus(sys, 5);                           //  5 = forwarding time

    // Binding the device's ports to the bus
    Iram1->getPort()->bind(bus->getUnboundMemSidePort());    // Binding Instruction memory (cpu0) to membus
    cpu0->getPort1()->bind(bus->getUnboundCPUSidePort());   // Binding cpu0 instruction port (port2) to membus

    Iram2->getPort()->bind(bus->getUnboundMemSidePort());    // Binding Instruction memory (cpu1) to membus
    cpu1->getPort1()->bind(bus->getUnboundCPUSidePort());   // Binding cpu1 instruction port (port2) to membus

    ram->getPort()->bind(bus->getUnboundMemSidePort());     // Binding instruction memory membus
    cpu0->getPort2()->bind(bus->getUnboundCPUSidePort());   // Binding cpu0 data port (port1) to membus
    cpu1->getPort2()->bind(bus->getUnboundCPUSidePort());   // Binding cpu1 data port (port1) to membus

    // Fill DRAM with random floating point values. 0x400 - 0xFFF
    for (auto i = ram->getAddrRange().first; i < ram->getAddrRange().second - 0x400; i+=4){
         float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
         uint32_t val = *(uint32_t *)(&r);
         ram->writeWordAtAddr(i, val);
    }

    // Setting the frame ptr to the starting value of the stack and the stack ptr to the ending point of the stack
    cpu0->setRegister(stackBegin0, stackEnd0);
    cpu1->setRegister(stackBegin1, stackEnd1);

    setupSimulator(cpu0, Iram1, Iram2);
    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    // cpu1->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    return 0;
}

void setupSimulator(CPU* cpu, DRAM *ram1, DRAM *ram2){ // Sets up the instruction memory with the instructions
    std::cout << "Placing the instructions into memory" << std::endl << std::endl;

    // CPU0
    ram1->writeWordAtAddr(0, 0b11001000100000001000000011111111); // 2
    ram1->writeWordAtAddr(4, 0b11000100011001001000100000000000); // 3
    ram1->writeWordAtAddr(8, 0b11000100001001001000000100001000); // 4
    ram1->writeWordAtAddr(12, 0b11001000001000001000000010000000); // 5
    ram1->writeWordAtAddr(16, 0b11001000101000000000000000000000); // 6
    ram1->writeWordAtAddr(20, 0b11000100010101000010010101111111); // 7
    ram1->writeWordAtAddr(24, 0b11000100000101000010010101111111); // 8
    ram1->writeWordAtAddr(28, 0b11110110000000000000000001000000); // 9
    ram1->writeWordAtAddr(32, 0b11000000101001000010000011111111); // 11
    ram1->writeWordAtAddr(36, 0b11001001101000000000111111110000); // 12
    ram1->writeWordAtAddr(40, 0b11000110000000111010010100010000); // 13
    ram1->writeWordAtAddr(44, 0b11110110000000000000000011000000); // 14
    ram1->writeWordAtAddr(48, 0b11101100101000000000000000000000); // 16
    ram1->writeWordAtAddr(52, 0b11001000101000001010000000000010); // 17
    ram1->writeWordAtAddr(56, 0b11000001101001000010000011111111); // 18
    ram1->writeWordAtAddr(60, 0b11001001101010011010010000000000); // 19
    ram1->writeWordAtAddr(64, 0b11001100101000001010110100000000); // 20
    ram1->writeWordAtAddr(68, 0b11100000000001001010000000000000); // 21
    ram1->writeWordAtAddr(72, 0b11101100101000000000000000000000); // 22
    ram1->writeWordAtAddr(76, 0b11001000101000001010000000000001); // 23
    ram1->writeWordAtAddr(80, 0b11001100101000001010110100000000); // 24
    ram1->writeWordAtAddr(84, 0b11100001000001001010000000000000); // 25
    ram1->writeWordAtAddr(88, 0b11001010000011100000100000000000); // 26
    ram1->writeWordAtAddr(92, 0b11101100101000000000000000000000); // 27
    ram1->writeWordAtAddr(96, 0b11001000101000001010000000000011); // 28
    ram1->writeWordAtAddr(100, 0b11001100101000001010110100000000); // 29
    ram1->writeWordAtAddr(104, 0b11100100000001001010000000000000); // 30
    ram1->writeWordAtAddr(108, 0b11110110000000000000000011100000); // 31
    ram1->writeWordAtAddr(112, 0b11000000101001000010000011111111); // 33
    ram1->writeWordAtAddr(116, 0b11001000101000001010100000000000); // 34
    ram1->writeWordAtAddr(120, 0b11000100000101000010010101111111); // 35
    ram1->writeWordAtAddr(124, 0b11110110000000000000000001000000); // 36
    ram1->writeWordAtAddr(128, 0b11000000101001000010001011111111); // 38
    ram1->writeWordAtAddr(132, 0b11000000001001001000000100000000); // 39
    ram1->writeWordAtAddr(136, 0b11000001000001001000001100000000); // 40
    ram1->writeWordAtAddr(140, 0b11001000100000001000000010000000); // 41
    ram1->writeWordAtAddr(144, 0b11100110000000010000000000000000); // 42

    // CPU1
    ram2->writeWordAtAddr(256, 0b11001000100000001000000011111111); // 2
    ram2->writeWordAtAddr(260, 0b11000100011001001000100000000000); // 3
    ram2->writeWordAtAddr(264, 0b11000100001001001000000100001000); // 4
    ram2->writeWordAtAddr(268, 0b11001000001000001000000010000000); // 5
    ram2->writeWordAtAddr(272, 0b11001000101000000000000000000000); // 6
    ram2->writeWordAtAddr(276, 0b11000100010101000010010101111111); // 7
    ram2->writeWordAtAddr(280, 0b11000100000101000010010101111111); // 8
    ram2->writeWordAtAddr(284, 0b11110110000000000000000001000000); // 9
    ram2->writeWordAtAddr(288, 0b11000000101001000010000011111111); // 11
    ram2->writeWordAtAddr(292, 0b11001001101000000000111111110000); // 12
    ram2->writeWordAtAddr(296, 0b11000110000000111010010100010000); // 13
    ram2->writeWordAtAddr(300, 0b11110110000000000000000001100000); // 14
    ram2->writeWordAtAddr(304, 0b11101100101000000000000000000000); // 16
    ram2->writeWordAtAddr(308, 0b11001000101000001010000000000010); // 17
    ram2->writeWordAtAddr(312, 0b11000001101001000010000011111111); // 18
    ram2->writeWordAtAddr(316, 0b11001001101010011010010000000000); // 19
    ram2->writeWordAtAddr(320, 0b11001100101000001010110100000000); // 20
    ram2->writeWordAtAddr(324, 0b11100000000001001010000000000000); // 21
    ram2->writeWordAtAddr(328, 0b11101100101000000000000000000000); // 22
    ram2->writeWordAtAddr(332, 0b11001000101000001010000000000001); // 23
    ram2->writeWordAtAddr(336, 0b11001000101000001010000000000001); // 24
    ram2->writeWordAtAddr(340, 0b11100001000001001010000000000000); // 25
    ram2->writeWordAtAddr(342, 0b11001010000011100000100000000000); // 26
    ram2->writeWordAtAddr(344, 0b11101100101000000000000000000000); // 27
    ram2->writeWordAtAddr(348, 0b11001000101000001010000000000011); // 28
    ram2->writeWordAtAddr(352, 0b11001100101000001010110100000000); // 29
    ram2->writeWordAtAddr(356, 0b11001010000001001010000000000000); // 30
    ram2->writeWordAtAddr(360, 0b11110110000000000000000001110000); // 31
    ram2->writeWordAtAddr(364, 0b11000000101001000010000011111111); // 33
    ram2->writeWordAtAddr(368, 0b11001000101000001010100000000000); // 34
    ram2->writeWordAtAddr(372, 0b11000100000010000100101011111111); // 35
    ram2->writeWordAtAddr(376, 0b11110110000000000000000001000000); // 36
    ram2->writeWordAtAddr(380, 0b11000000101001000010001011111111); // 38
    ram2->writeWordAtAddr(384, 0b11000000001001001000000100000000); // 39
    ram2->writeWordAtAddr(388, 0b11000001000001001000001100000000); // 40
    ram2->writeWordAtAddr(392, 0b11001000100000001000000010000000); // 41
    ram2->writeWordAtAddr(394, 0b11100110000000010000000000000000); // 42
}
