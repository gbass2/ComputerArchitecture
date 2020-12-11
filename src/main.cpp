#include "event.hh"
#include "system.hh"
#include "cpu.hh"
#include "simobject.hh"
#include "DRAM.hh"
#include "membus.hh"
#include <memory>
#include <fstream>

void setupSimulator(DRAM *ram1, DRAM *ram2);

int main(){
    auto sys = std::make_shared<System>();

    size_t stackEnd0 = 0x2FF;
    size_t stackBegin0 = 0x200;
    size_t stackEnd1 = 0x3FF;
    size_t stackBegin1 = 0x300;
    size_t dramEnd = 0xA1DC;
    size_t memLatency = 100;

    CPU *cpu0 = new CPU(sys, "cpu0", 0, 0x177, stackBegin0, dramEnd); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory
    CPU *cpu1 = new CPU(sys, "cpu1", 0x200, 0x317, stackBegin0, dramEnd); // Passes in the device name, the starting and end addrs for instruction memory, andd the stating and end addrs for data memory

    RunSim *sim = new RunSim(sys);

    DRAM * ram = new DRAM(sys, AddrRange(0x200, dramEnd), memLatency);    // Data memory. 20 = response time
    DRAM * Iram1 = new DRAM(sys, AddrRange(0, 0x177), memLatency);         // Instruction memory for cpu0. 20 = response time
    DRAM * Iram2 = new DRAM(sys, AddrRange(0x200, 0x317), memLatency);         // Instruction memory for cpu1. 20 = response time
    Membus * bus = new Membus(sys, 5);                           //  5 = forwarding time

    // Binding the device's ports to the bus
    Iram1->getPort()->bind(bus->getUnboundMemSidePort());    // Binding Instruction memory (cpu0) to membus
    cpu0->getPort1()->bind(bus->getUnboundCPUSidePort());   // Binding cpu0 instruction port (port2) to membus

    Iram2->getPort()->bind(bus->getUnboundMemSidePort());    // Binding Instruction memory (cpu1) to membus
    cpu1->getPort1()->bind(bus->getUnboundCPUSidePort());   // Binding cpu1 instruction port (port2) to membus

    ram->getPort()->bind(bus->getUnboundMemSidePort());     // Binding instruction memory membus
    cpu0->getPort2()->bind(bus->getUnboundCPUSidePort());   // Binding cpu0 data port (port1) to membus
    cpu1->getPort2()->bind(bus->getUnboundCPUSidePort());   // Binding cpu1 data port (port1) to membus

    // Filling in Mat A
    std::ifstream MAT_A;
    MAT_A.open("MAT_A_DATA.txt");
    std::string line;
    int i = 0x400;
    while(getline(MAT_A, line)) {
        int r = stoi(line);
        uint8_t val = *(uint8_t *)(&r);
        ram->writeWordAtAddr(i, val);
        i++;
    }

    std::ifstream MAT_B;
    MAT_B.open("MAT_B_DATA.txt");
    // Filling in Mat B
    i = 0x2CA4;
    while(getline(MAT_B, line)) {
        int r = stoi(line);
        uint8_t val = *(uint8_t *)(&r);
        ram->writeWordAtAddr(i, val);
        i++;
    }

    // Filling in Mat C
    for (auto i = 0x2CAD; i <= 0x7ACC; i+=2){
         int16_t r = -1;
         uint16_t val = *(uint16_t *)(&r);
         ram->writeHalfwordAtAddr(i, val);
    }

    // Setting the frame ptr to the starting value of the stack and the stack ptr to the ending point of the stack
    cpu0->setRegister(stackBegin0, stackEnd0);
    cpu1->setRegister(stackBegin1, stackEnd1);

    setupSimulator(Iram1, Iram2);

    cpu0->initialize();       // Sets up the first event. Which is a fetch event
    cpu1->initialize();       // Sets up the first event. Which is a fetch event
    sim->runSimulation();     // Runs the instructions

    // Outputing the results
    std::cout << std::endl << std::endl << "Output cpu0: " << std::endl;
    for(auto x: cpu0->output)
        std::cout << x << std::endl;

    std::cout << std::endl << std::endl << "Output cpu1: " << std::endl;
    for(auto x: cpu1->output)
        std::cout << x << std::endl;

    // Outputing CPI
    std::cout << std::endl << "CPI for CPU0: " << (sys->cycles0+1)/cpu0->numInstructions << std::endl;
    std::cout << "CPI for CPU1: " << (sys->cycles1 +1)/cpu1->numInstructions << std::endl;

    return 0;
}

void setupSimulator(DRAM *ram1, DRAM *ram2){ // Sets up the instruction memory with the instructions
    std::cout << "Placing the instructions into memory" << std::endl << std::endl;

    // A5_CPU0
    ram1->writeWordAtAddr(0, 0b11001000100000001000000001111111); // 2
    ram1->writeWordAtAddr(4, 0b11000100011101001000100000000000); // 3
    ram1->writeWordAtAddr(8, 0b11000100001101001000000100000000); // 4
    ram1->writeWordAtAddr(12, 0b11001000001000001000000001000000); // 2
    ram1->writeWordAtAddr(16, 0b11001000101000000000000000000000); // 6
    ram1->writeWordAtAddr(20, 0b11000100010101000010010101111111); // 7
    ram1->writeWordAtAddr(24, 0b11000100100110000010010101111111); // 8
    ram1->writeWordAtAddr(28, 0b11110110000000000000000001000000); // 9
    ram1->writeWordAtAddr(32, 0b11000000101010100010010011111111); // 11
    ram1->writeWordAtAddr(36, 0b11001001101000000000110001100000); // 12
    ram1->writeWordAtAddr(40, 0b11000110010000111010010101101000); // 13
    ram1->writeWordAtAddr(44, 0b11110110000000000000000011000000); // 14
    ram1->writeWordAtAddr(48, 0b11001000101000000000000000000000); // 16
    ram1->writeWordAtAddr(52, 0b11000100000110000010010101111111); // 17
    ram1->writeWordAtAddr(56, 0b11110110000000000000001111000000); // 18
    ram1->writeWordAtAddr(60, 0b11000000101010100010000011111111); // 20
    ram1->writeWordAtAddr(64, 0b11001001101000000000110001100000); // 21
    ram1->writeWordAtAddr(68, 0b11000110000100111010010100101000); // 22
    ram1->writeWordAtAddr(72, 0b11110110000000000000001100100000); // 23
    ram1->writeWordAtAddr(76, 0b11001000101000000000000000000000); // 25
    ram1->writeWordAtAddr(80, 0b11000100101010000010010101111111); // 26
    ram1->writeWordAtAddr(84, 0b11000100111010000010010101111111); // 27
    ram1->writeWordAtAddr(88, 0b11110110000000000000001110100000); // 28
    ram1->writeWordAtAddr(92, 0b11000000101010100010011101111111); // 30
    ram1->writeWordAtAddr(96, 0b11001001101000000000010000000000); // 31
    ram1->writeWordAtAddr(100, 0b11000110000100111010010100001000); // 32
    ram1->writeWordAtAddr(104, 0b11110110000000000000001101100000); // 33
    ram1->writeWordAtAddr(108, 0b11001000101000000000000000000000); // 35
    ram1->writeWordAtAddr(112, 0b11000100011010000010010101111111); // 36
    ram1->writeWordAtAddr(116, 0b01100000000000000001111000000000); // 37
    ram1->writeWordAtAddr(120, 0b11000000101010100010001101111111); // 39
    ram1->writeWordAtAddr(124, 0b11001001101000000000010000000000); // 40
    ram1->writeWordAtAddr(128, 0b11000110011100111010010101110000); // 41
    ram1->writeWordAtAddr(132, 0b11110110000000000000000100010000); // 42
    ram1->writeWordAtAddr(136, 0b11101100101000000000000000000000); // 44
    ram1->writeWordAtAddr(140, 0b11001000101000001010000000000010); // 45
    ram1->writeWordAtAddr(144, 0b11000001101010100010010011111111); // 46
    ram1->writeWordAtAddr(148, 0b11000000011010100010011101111111); // 47
    ram1->writeWordAtAddr(152, 0b11001101101000011010001100000000); // 48
    ram1->writeWordAtAddr(156, 0b11001001011000000000011001100000); // 49
    ram1->writeWordAtAddr(160, 0b11001101101000011010101101000000); // 50
    ram1->writeWordAtAddr(164, 0b11000001011010100010000011111111); // 51
    ram1->writeWordAtAddr(168, 0b11000000111010100010001101111111); // 52
    ram1->writeWordAtAddr(172, 0b11001101011000010110011100000000); // 53
    ram1->writeWordAtAddr(176, 0b11001101101000011010101100000000); // 54
    ram1->writeWordAtAddr(180, 0b11001100101000001010110100000000); // 55
    ram1->writeWordAtAddr(184, 0b11000000101000101010000000000000); // 56
    ram1->writeWordAtAddr(188, 0b11101101101001000000000000000000); // 57
    ram1->writeWordAtAddr(192, 0b11001001101000011010001001010011); // 58
    ram1->writeWordAtAddr(196, 0b11001001011000000000110000000000); // 59
    ram1->writeWordAtAddr(200, 0b11001100011000000110101101000000); // 60
    ram1->writeWordAtAddr(204, 0b11001100011000000110011100000000); // 61
    ram1->writeWordAtAddr(208, 0b11001101101000011010001100000000); // 62
    ram1->writeWordAtAddr(212, 0b11000001101000111010000000000000); // 63
    ram1->writeWordAtAddr(216, 0b11001100101000001010110101000000); // 64
    ram1->writeWordAtAddr(220, 0b11000001101010000010010101111111); // 65
    ram1->writeWordAtAddr(224, 0b11001100101000011010010100000000); // 66
    ram1->writeWordAtAddr(228, 0b11000100101010000010010101111111); // 67
    ram1->writeWordAtAddr(232, 0b11110110000000000000001101110000); // 68
    ram1->writeWordAtAddr(236, 0b11000000101010000010001101111111); // 70
    ram1->writeWordAtAddr(240, 0b11001000101000001010100000000000); // 71
    ram1->writeWordAtAddr(244, 0b11000100011010000010010101111111); // 72
    ram1->writeWordAtAddr(248, 0b11110110000000000000001111110000); // 73
    ram1->writeWordAtAddr(252, 0b11110110000000000000000000001000); // 75
    ram1->writeWordAtAddr(256, 0b11000000101010000010011101111111); // 77
    ram1->writeWordAtAddr(260, 0b11001000101000001010100000000000); // 78
    ram1->writeWordAtAddr(264, 0b11000100111010000010010101111111); // 79
    ram1->writeWordAtAddr(268, 0b11110110000000000000000010001000); // 80
    ram1->writeWordAtAddr(272, 0b11000000101010000010010101111111); // 82
    ram1->writeWordAtAddr(276, 0b11101101101001000000000000000000); // 83
    ram1->writeWordAtAddr(280, 0b11001001101000011010101101010011); // 84
    ram1->writeWordAtAddr(284, 0b11000000011010100010010011111111); // 85
    ram1->writeWordAtAddr(288, 0b11001001011000000000001001100000); // 86
    ram1->writeWordAtAddr(292, 0b11001100011000000110101101000000); // 87
    ram1->writeWordAtAddr(296, 0b11000001011010100010000011111111); // 88
    ram1->writeWordAtAddr(300, 0b11001100011000000110101100000000); // 89
    ram1->writeWordAtAddr(304, 0b11001000011010000110100000000000); // 90
    ram1->writeWordAtAddr(308, 0b11001101101000011010001100000000); // 91
    ram1->writeWordAtAddr(312, 0b11000100000010011010010100000000); // 92
    ram1->writeWordAtAddr(316, 0b11110110000000000000000000101000); // 93
    ram1->writeWordAtAddr(320, 0b11000000101010000010000011111111); // 95
    ram1->writeWordAtAddr(324, 0b11001000101000001010100000000000); // 96
    ram1->writeWordAtAddr(328, 0b11000100000110000010010101111111); // 97
    ram1->writeWordAtAddr(332, 0b11110110000000000000001010101000); // 98
    ram1->writeWordAtAddr(336, 0b11110110000000000000001010101000); // 100
    ram1->writeWordAtAddr(340, 0b11000000101010000010010011111111); // 102
    ram1->writeWordAtAddr(344, 0b11001000101000001010100000000000); // 103
    ram1->writeWordAtAddr(348, 0b11000100100110000010010101111111); // 104
    ram1->writeWordAtAddr(352, 0b11110110000000000000001001101000); // 105
    ram1->writeWordAtAddr(356, 0b11000000101001000010001011111111); // 107
    ram1->writeWordAtAddr(360, 0b11000000001001001000000110000000); // 108
    ram1->writeWordAtAddr(364, 0b11000001000001001000001110000000); // 109
    ram1->writeWordAtAddr(368, 0b11001000100000001000000111101000); // 110
    ram1->writeWordAtAddr(372, 0b11100110000000010000000000000000); // 111

    // A5_CPU1
    ram2->writeWordAtAddr(2048, 0b11001000100000001000000001111111); //2
    ram2->writeWordAtAddr(2052, 0b11000100011101001000100000000000); //3
    ram2->writeWordAtAddr(2056, 0b11000100001101001000000100000000); //4
    ram2->writeWordAtAddr(2060, 0b11001000001000001000000001000000); //5
    ram2->writeWordAtAddr(2064, 0b11001000101000000000000000000000); //6
    ram2->writeWordAtAddr(2068, 0b11000100010101000010010101111111); //7
    ram2->writeWordAtAddr(2072, 0b11000100100110000010010101111111); //8
    ram2->writeWordAtAddr(2076, 0b11000000101010100010010011111111); //9
    ram2->writeWordAtAddr(2080, 0b11001001101000000000110001100000); //11
    ram2->writeWordAtAddr(2084, 0b11110110000000000000000001000100); //12
    ram2->writeWordAtAddr(2088, 0b11000110100000111010010100001100); //13
    ram2->writeWordAtAddr(2092, 0b11110110000000000000000011000100); //14
    ram2->writeWordAtAddr(2096, 0b11001000101000000000000000000000); //16
    ram2->writeWordAtAddr(2100, 0b11000100000110000010010101111111); //17
    ram2->writeWordAtAddr(2104, 0b11110110000000000000001111000100); //18
    ram2->writeWordAtAddr(2108, 0b11000000101010100010000011111111); //20
    ram2->writeWordAtAddr(2112, 0b11001001101000000000110001100000); //21
    ram2->writeWordAtAddr(2116, 0b11000110000100111010010101110100); //22
    ram2->writeWordAtAddr(2120, 0b11110110000000000000001100100100); //23
    ram2->writeWordAtAddr(2124, 0b11110110000000000000000010100100); //25
    ram2->writeWordAtAddr(2128, 0b11101100101001000000000000000000); //27
    ram2->writeWordAtAddr(2132, 0b11001000101000001010101101010011); //28
    ram2->writeWordAtAddr(2136, 0b11000001101010100010010011111111); //29
    ram2->writeWordAtAddr(2140, 0b11001000011000000000001001100000); //30
    ram2->writeWordAtAddr(2144, 0b11001101101000011010001101000000); //31
    ram2->writeWordAtAddr(2148, 0b11000000011010100010000011111111); //32
    ram2->writeWordAtAddr(2152, 0b11001101101000011010001100000000); //33
    ram2->writeWordAtAddr(2156, 0b11001001101010011010100000000000); //34
    ram2->writeWordAtAddr(2160, 0b11001100101000001010110100000000); //35
    ram2->writeWordAtAddr(2164, 0b11000000101010001010000000000000); //36
    ram2->writeWordAtAddr(2168, 0b11001001101000000000111111111111); //37
    ram2->writeWordAtAddr(2172, 0b11000110001010001010110100010100); //38
    ram2->writeWordAtAddr(2176, 0b11110110000000000000001000010100); //39
    ram2->writeWordAtAddr(2180, 0b11110110000000000000000010100100); //41
    ram2->writeWordAtAddr(2184, 0b11101100101001000000000000000000); //43
    ram2->writeWordAtAddr(2188, 0b11001000101000001010101101010011); //44
    ram2->writeWordAtAddr(2192, 0b11000001101010100010010011111111); //45
    ram2->writeWordAtAddr(2196, 0b11001000011000000000001001100000); //46
    ram2->writeWordAtAddr(2200, 0b11001101101000011010001101000000); //47
    ram2->writeWordAtAddr(2204, 0b11000001011010100010000011111111); //48
    ram2->writeWordAtAddr(2208, 0b11001101101000011010101100000000); //49
    ram2->writeWordAtAddr(2212, 0b11001001101010011010100000000000); //50
    ram2->writeWordAtAddr(2216, 0b11001100101000001010110100000000); //51
    ram2->writeWordAtAddr(2220, 0b11000000101010001010000000000000); //52
    ram2->writeWordAtAddr(2224, 0b11000100011010000010010101111111); //53
    ram2->writeWordAtAddr(2228, 0b11000000101010000010001101111111); //54
    ram2->writeWordAtAddr(2232, 0b11001000101010101010001000000000); //55
    ram2->writeWordAtAddr(2236, 0b11101101101011100000000000000000); //56
    ram2->writeWordAtAddr(2240, 0b11001001101000011010101100110101); //57
    ram2->writeWordAtAddr(2244, 0b11000001011010100010010011111111); //58
    ram2->writeWordAtAddr(2248, 0b11001100011000010110001101000000); //59
    ram2->writeWordAtAddr(2252, 0b11000001011010100010000011111111); //60
    ram2->writeWordAtAddr(2256, 0b11001100011000000110101100000000); //61
    ram2->writeWordAtAddr(2260, 0b11001101101000011010001100000000); //62
    ram2->writeWordAtAddr(2264, 0b11000100000000011010010100000000); //63
    ram2->writeWordAtAddr(2268, 0b11110110000000000000000001110100); //64
    ram2->writeWordAtAddr(2272, 0b11000000101010000010000011111111); //66
    ram2->writeWordAtAddr(2276, 0b11001000101000001010100000000000); //67
    ram2->writeWordAtAddr(2280, 0b11000100000110000010010101111111); //68
    ram2->writeWordAtAddr(2284, 0b11110110000000000000001111000100); //69
    ram2->writeWordAtAddr(2288, 0b11110110000000000000001011110100); //71
    ram2->writeWordAtAddr(2292, 0b11000000101010000010010011111111); //73
    ram2->writeWordAtAddr(2296, 0b11001000101000001010100000000000); //74
    ram2->writeWordAtAddr(2300, 0b11000100100110000010010101111111); //75
    ram2->writeWordAtAddr(2304, 0b11110110000000000000000001000100); //76
    ram2->writeWordAtAddr(2308, 0b11000000101001000010001011111111); //78
    ram2->writeWordAtAddr(2312, 0b11000000001001001000000110000000); //79
    ram2->writeWordAtAddr(2316, 0b11000001000001001000001110000000); //80
    ram2->writeWordAtAddr(2320, 0b11001000100000001000000001000000); //81
    ram2->writeWordAtAddr(2324, 0b11100110000000010000000000000000); //82


}
