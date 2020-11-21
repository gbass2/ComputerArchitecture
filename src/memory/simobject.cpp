#include "simobject.hh"
#include <cstring>
#include <cassert>

using namespace std;

// Main function for running the simulation
void RunSim::runSimulation(){
    double numInstructions = 37; // Remove once ret is implemented

    while(!(sysMain->getMEQ()).empty()){
        // Counting the cycles
        if(currTick() % 10 == 0 && currTick() > 1)
            cycles++;

        printMEQ();

        cout << "Current Cycle: " << cycles  << endl;
        cout << "Current Tick: " << currTick() << endl;

        if ((sysMain->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        }

        incTick(1); // Increments currentTick by amount (t)
    }

    if(!(currTick() == 1))
        cout << "CPI: " << (cycles+1)/numInstructions << endl;
}

void RunSim::setupSimulator(){
    // Convert the asm by hand and place them into the correct memory locations
    Iport->p1->setMemory(0, 0b11001000, "I", "Base"); // 2
    Iport->p1->setMemory(1, 0b10000000, "I", "Base");
    Iport->p1->setMemory(2, 0b10000000, "I", "Base");
    Iport->p1->setMemory(3, 0b11111111, "I", "Base");
    Iport->p1->setMemory(4, 0b11000100, "S", "Base"); // 3
    Iport->p1->setMemory(5, 0b01100100, "S", "Base");
    Iport->p1->setMemory(6, 0b10001000, "S", "Base");
    Iport->p1->setMemory(7, 0b00000000, "S", "Base");
    Iport->p1->setMemory(8, 0b11000100, "S", "Base"); // 4
    Iport->p1->setMemory(9, 0b00100100, "S", "Base");
    Iport->p1->setMemory(10,0b10000001, "S", "Base");
    Iport->p1->setMemory(11, 0b00001000, "S", "Base");
    Iport->p1->setMemory(12, 0b11001000, "I", "Base"); // 5
    Iport->p1->setMemory(13, 0b00100000, "I", "Base");
    Iport->p1->setMemory(14, 0b10000000, "I", "Base");
    Iport->p1->setMemory(15, 0b10000000, "I", "Base");
    Iport->p1->setMemory(16, 0b11001000, "I", "Base"); // 6
    Iport->p1->setMemory(17, 0b10100000, "I", "Base");
    Iport->p1->setMemory(18, 0b00000000, "I", "Base");
    Iport->p1->setMemory(19, 0b00000000, "I", "Base");
    Iport->p1->setMemory(20, 0b11000100, "S", "Base"); // 7
    Iport->p1->setMemory(21, 0b01010100, "S", "Base");
    Iport->p1->setMemory(22, 0b00100101, "S", "Base");
    Iport->p1->setMemory(23, 0b01111111, "S", "Base");
    Iport->p1->setMemory(24, 0b11000100, "S", "Base"); // 8
    Iport->p1->setMemory(25, 0b00010100, "S", "Base");
    Iport->p1->setMemory(26, 0b00100101, "S", "Base");
    Iport->p1->setMemory(27, 0b01111111, "S", "Base");
    Iport->p1->setMemory(28, 0b11110110, "J", "Base"); // 9
    Iport->p1->setMemory(29, 0b00000000, "J", "Base");
    Iport->p1->setMemory(30, 0b00000000, "J", "Base");
    Iport->p1->setMemory(31, 0b01000000, "J", "Base");
    Iport->p1->setMemory(32, 0b11000000, "I", "Base"); // 11
    Iport->p1->setMemory(33, 0b10100100, "I", "Base");
    Iport->p1->setMemory(34, 0b00100000, "I", "Base");
    Iport->p1->setMemory(35, 0b11111111, "I", "Base");
    Iport->p1->setMemory(36, 0b11001001, "I", "Base"); // 12
    Iport->p1->setMemory(37, 0b10100000, "I", "Base");
    Iport->p1->setMemory(38, 0b00001111, "I", "Base");
    Iport->p1->setMemory(39, 0b11110000, "I", "Base");
    Iport->p1->setMemory(40, 0b11000110, "B", "Base"); // 13
    Iport->p1->setMemory(41, 0b00000011, "B", "Base");
    Iport->p1->setMemory(42, 0b10100101, "B", "Base");
    Iport->p1->setMemory(43, 0b00010000, "B", "Base");
    Iport->p1->setMemory(44, 0b11110110, "J", "Base"); // 14
    Iport->p1->setMemory(45, 0b00000000, "J", "Base");
    Iport->p1->setMemory(46, 0b00000000, "J", "Base");
    Iport->p1->setMemory(47, 0b01100000, "J", "Base");
    Iport->p1->setMemory(48, 0b11101100, "U", "Base"); // 16
    Iport->p1->setMemory(49, 0b10100000, "U", "Base");
    Iport->p1->setMemory(50, 0b00000000, "U", "Base");
    Iport->p1->setMemory(51, 0b00000000, "U", "Base");
    Iport->p1->setMemory(52, 0b11001000, "I", "Base"); // 17
    Iport->p1->setMemory(53, 0b10100000, "I", "Base");
    Iport->p1->setMemory(54, 0b10100000, "I", "Base");
    Iport->p1->setMemory(55, 0b00000010, "I", "Base");
    Iport->p1->setMemory(56, 0b11000001, "I", "Base"); // 18
    Iport->p1->setMemory(57, 0b10100100, "I", "Base");
    Iport->p1->setMemory(58, 0b00100000, "I", "Base");
    Iport->p1->setMemory(59, 0b11111111, "I", "Base");
    Iport->p1->setMemory(60, 0b11001001, "R", "Base"); // 19
    Iport->p1->setMemory(61, 0b10101001, "R", "Base");
    Iport->p1->setMemory(62, 0b10100100, "R", "Base");
    Iport->p1->setMemory(63, 0b00000000, "R", "Base");
    Iport->p1->setMemory(64, 0b11001100, "R", "Base"); // 20
    Iport->p1->setMemory(65, 0b10100000, "R", "Base");
    Iport->p1->setMemory(66, 0b10101101, "R", "Base");
    Iport->p1->setMemory(67, 0b00000000, "R", "Base");
    Iport->p1->setMemory(68, 0b11100000, "I", "Float"); // 21
    Iport->p1->setMemory(69, 0b00000100, "I", "Float");
    Iport->p1->setMemory(70, 0b10100000, "I", "Float");
    Iport->p1->setMemory(71, 0b00000000, "I", "Float");
    Iport->p1->setMemory(72, 0b11101100, "U", "Base"); // 22
    Iport->p1->setMemory(73, 0b10100000, "U", "Base");
    Iport->p1->setMemory(74, 0b00000000, "U", "Base");
    Iport->p1->setMemory(75, 0b00000000, "U", "Base");
    Iport->p1->setMemory(76, 0b11001000, "I", "Base"); // 23
    Iport->p1->setMemory(77, 0b10100000, "I", "Base");
    Iport->p1->setMemory(78, 0b10100000, "I", "Base");
    Iport->p1->setMemory(79, 0b00000001, "I", "Base");
    Iport->p1->setMemory(80, 0b11001100, "R", "Base"); // 24
    Iport->p1->setMemory(81, 0b10100000, "R", "Base");
    Iport->p1->setMemory(82, 0b10101101, "R", "Base");
    Iport->p1->setMemory(83, 0b00000000, "R", "Base");
    Iport->p1->setMemory(84, 0b11100001, "I", "Float"); // 25
    Iport->p1->setMemory(85, 0b00000100, "I", "Float");
    Iport->p1->setMemory(86, 0b10100000, "I", "Float");
    Iport->p1->setMemory(87, 0b00000000, "I", "Float");
    Iport->p1->setMemory(88, 0b11001010, "R", "Float"); // 26
    Iport->p1->setMemory(89, 0b00001110, "R", "Float");
    Iport->p1->setMemory(90, 0b00001000, "R", "Float");
    Iport->p1->setMemory(91, 0b00000000, "R", "Float");
    Iport->p1->setMemory(92, 0b11101100, "U", "Base"); // 27
    Iport->p1->setMemory(93, 0b10100000, "U", "Base");
    Iport->p1->setMemory(94, 0b00000000, "U", "Base");
    Iport->p1->setMemory(95, 0b00000000, "U", "Base");
    Iport->p1->setMemory(96, 0b11001000, "I", "Base"); // 28
    Iport->p1->setMemory(97, 0b10100000, "I", "Base");
    Iport->p1->setMemory(98, 0b10100000, "I", "Base");
    Iport->p1->setMemory(99, 0b00000011, "I", "Base");
    Iport->p1->setMemory(100, 0b11001100, "R", "Base"); // 29
    Iport->p1->setMemory(101, 0b10100000, "R", "Base");
    Iport->p1->setMemory(102, 0b10101101, "R", "Base");
    Iport->p1->setMemory(103, 0b00000000, "R", "Base");
    Iport->p1->setMemory(104, 0b11001010, "S", "Float"); // 30
    Iport->p1->setMemory(105, 0b00000100, "S", "Float");
    Iport->p1->setMemory(106, 0b10100000, "S", "Float");
    Iport->p1->setMemory(107, 0b00000000, "S", "Float");
    Iport->p1->setMemory(108, 0b11110110, "J", "Base"); // 31
    Iport->p1->setMemory(109, 0b00000000, "J", "Base");
    Iport->p1->setMemory(110, 0b00000000, "J", "Base");
    Iport->p1->setMemory(111, 0b01110000, "J", "Base");
    Iport->p1->setMemory(112, 0b11000000, "I", "Base"); // 33
    Iport->p1->setMemory(113, 0b10100100, "I", "Base");
    Iport->p1->setMemory(114, 0b00100000, "I", "Base");
    Iport->p1->setMemory(115, 0b11111111, "I", "Base");
    Iport->p1->setMemory(116, 0b11001000, "I", "Base"); // 34
    Iport->p1->setMemory(117, 0b10100000, "I", "Base");
    Iport->p1->setMemory(118, 0b10101000, "I", "Base");
    Iport->p1->setMemory(119, 0b00000000, "I", "Base");
    Iport->p1->setMemory(120, 0b11000100, "S", "Base"); // 35
    Iport->p1->setMemory(121, 0b00001000, "S", "Base");
    Iport->p1->setMemory(122, 0b01001010, "S", "Base");
    Iport->p1->setMemory(123, 0b11111111, "S", "Base");
    Iport->p1->setMemory(124, 0b11110110, "J", "Base"); // 36
    Iport->p1->setMemory(125, 0b00000000, "J", "Base");
    Iport->p1->setMemory(126, 0b00000000, "J", "Base");
    Iport->p1->setMemory(127, 0b01000000, "J", "Base");
    Iport->p1->setMemory(128, 0b11000000, "I", "Base"); // 38
    Iport->p1->setMemory(129, 0b10100100, "I", "Base");
    Iport->p1->setMemory(130, 0b00100010, "I", "Base");
    Iport->p1->setMemory(131, 0b11111111, "I", "Base");
    Iport->p1->setMemory(132, 0b11000000, "I", "Base"); // 39
    Iport->p1->setMemory(133, 0b00100100, "I", "Base");
    Iport->p1->setMemory(134, 0b10000001, "I", "Base");
    Iport->p1->setMemory(135, 0b00000000, "I", "Base");
    Iport->p1->setMemory(136, 0b11000001, "I", "Base"); // 40
    Iport->p1->setMemory(137, 0b00000100, "I", "Base");
    Iport->p1->setMemory(138, 0b10000011, "I", "Base");
    Iport->p1->setMemory(139, 0b00000000, "I", "Base");
    Iport->p1->setMemory(140, 0b11001000, "I", "Base"); // 41
    Iport->p1->setMemory(141, 0b10000000, "I", "Base");
    Iport->p1->setMemory(142, 0b10000000, "I", "Base");
    Iport->p1->setMemory(143, 0b10000000, "I", "Base");
    Iport->p1->setMemory(144, 0b11100110, "I", "Base"); // 42
    Iport->p1->setMemory(145, 0b00000001, "I", "Base");
    Iport->p1->setMemory(146, 0b00000000, "I", "Base");
    Iport->p1->setMemory(147, 0b00000000, "I", "Base");

    a->jump.push_back(0x20);
    a->jump.push_back(0x30);
    a->jump.push_back(0x70);
    a->jump.push_back(0x80);

}
