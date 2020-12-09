#include "cpu.hh"
#include <sstream>
#include <limits.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include <bitset>

int Binary2Decimal(std::string binary, int significantBits);
std::string Decimal2Binary(std::string input);
float GetFloat32( std::string Binary );
std::string GetBinary32( float value );

// Determines what alu operation needs to be done based on the opcode and the width of the instruction
void CPU::ALU::aluOperations() {
    std::cout << "opcode: " << cpu->ex->intInst.opcode << " funct3: " << cpu->ex->intInst.funct3 << std::endl;
    // Integer Operations
    if(!cpu->ex->getIsFloat()){
        if(cpu->ex->intInst.opcode.to_string() == "1100100") {      // addi/mv + slli
            if(cpu->ex->intInst.funct3.to_string() == "000")      // Add immediate
                ADDI();
            //else if(cpu->ex->intInst.funct3.to_string() == "010")
            //     SLTI();
            // else if(cpu->ex->intInst.funct3.to_string() == "110")
            //     SLTIU();
            // else if(cpu->ex->intInst.funct3.to_string() == "001")
            //     XORI();
            // else if(cpu->ex->intInst.funct3.to_string() == "011")
            //     ORI();
            // else if(cpu->ex->intInst.funct3.to_string() == "111")
            //     ANDI();
            else if(cpu->ex->intInst.funct3.to_string() == "100") // Locgial Shift Left
                SLLI();
            else if(cpu->ex->intInst.funct3.to_string() == "101") // Logical Right Shift
                SRLI();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100010") { // Store
            if(cpu->ex->intInst.funct3.to_string() == "000")
                SB();
            else if(cpu->ex->intInst.funct3.to_string() == "100")
                SH();
            else if(cpu->ex->intInst.funct3.to_string() == "010")
                SW();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100000") { // Load
            if(cpu->ex->intInst.funct3.to_string() == "000")
                LB();
            else if(cpu->ex->intInst.funct3.to_string() == "100")
                LH();
            else if(cpu->ex->intInst.funct3.to_string() == "010")
                LW();
            else if(cpu->ex->intInst.funct3.to_string() == "001")
                LBU();
            else if(cpu->ex->intInst.funct3.to_string() == "101")
                LBH();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1111011") { // Jump and Link
            JAL();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110011") { // Jump and Link Register
            JALR();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100011") { // Branch
            if(cpu->ex->intInst.funct3.to_string() == "000")
                BEQ();
            else if(cpu->ex->intInst.funct3.to_string() == "100")
                BNE();
            else if(cpu->ex->intInst.funct3.to_string() == "001")
                BLT();
            else if(cpu->ex->intInst.funct3.to_string() == "101")
                BGE();
            else if(cpu->ex->intInst.funct3.to_string() == "011")
                BLTU();
            else if(cpu->ex->intInst.funct3.to_string() == "111")
                BGEU();

        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110110") { // Load Upper Immediate
            LUI();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110100"){ // AUIPC
            AUIPC();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100110") { // Add
            if(cpu->ex->fInst.funct7.to_string() == "0000000")
                ADD();
        }
        else
            std::cout << "NOP in execute" << std::endl;

    } else { // Floating Point Operations
        if(cpu->ex->fInst.opcode.to_string() == "1100101") {  // fadd.s
            if(cpu->ex->fInst.funct7.to_string() == "0000000"){
                std::cout << "FADDS" << std::endl;
                FADDS();
            }
            else if(cpu->ex->fInst.funct7.to_string() == "0000100") {
                std::cout << "FSUBS" << std::endl;
                FSUBS();
            }
        }
        else if(cpu->ex->fInst.opcode.to_string() == "1110010") { // fsw
            std::cout << "FSW" << std::endl;
            FSW();
        }
        else if(cpu->ex->fInst.opcode.to_string() == "1110000") { // flw
            std::cout << "FLW" << std::endl;
            FLW();
        }
    }
    if(!cpu->ex->isMemAccess()){
        // // std::cout << "# Set Busy to False" << std::endl;
        cpu->ex->setBusy(0); // Setting execute stage to not busy if the operation does not access the memory

    }

    // Scheduling execute release unless the program is done
    if(cpu->currAddrI < cpu->endAddrI)
        cpu->ex->release->releaseEvent();
}

void CPU::ALU::ADDI() {
    // rd = rs1 + imm
    int val, val2;
    std::cout << "imm: " << cpu->ex->intInst.immISB << std::endl; //  110000000000 = 3072
    val = cpu->ex->intInst.rs1.getData();    // rs1

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        val2 = cpu->ex->intInst.immISB.to_ulong();
    else
        val2 =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024


    std::cout << "imm: " << val2 << std::endl;

    cpu->ex->intInst.rd.setData(val + val2); // Adding an int to an immediate value
}

void CPU::ALU::SLLI() {   // Logical left shift (zeros are shifted into the lower bits) PAGE 14
    int val = cpu->ex->intInst.immJU.to_ulong();  // Immidate value
    int val2 = cpu->ex->intInst.rs1.getData();    // rs1
    int val3 = val2 << val;

    std::cout << "imm: " << val << std::endl;
    std::cout << "rs1: " << val2 << std::endl;
    std::cout << "val3: " << val3 << std::endl;

    cpu->ex->intInst.rd.setData(val3);
}

void CPU::ALU::SRLI() {   // Logical right shift (zeros are shifted into the upper bits)
    int val = cpu->ex->intInst.immJU.to_ulong();  // Immidate value
    int val2 = cpu->ex->intInst.rs1.getData();    // rs1
    int val3 = val2 >> val;

    std::cout << "imm: " << val << std::endl;
    std::cout << "rs1: " << val2 << std::endl;
    std::cout << "val3: " << val3 << std::endl;

    cpu->ex->intInst.rd.setData(val3);
}

void CPU::ALU::SRAI() {   // Logical right shift (zeros are shifted into the upper bits)
    std::string imm = cpu->ex->intInst.immJU.to_string().substr(0, 5); // immediate value
    reverse(imm.begin(), imm.end());
    int val = Binary2Decimal(imm, 5);
    int val2 = cpu->ex->intInst.rs1.getData();    // rs1
    int val3 = val2 >> val;

    std::cout << "imm: " << val << std::endl;
    std::cout << "rs1: " << val2 << std::endl;
    std::cout << "val3: " << val3 << std::endl;

    cpu->ex->intInst.rd.setData(val3);
}

void CPU::ALU::LB() { // Load Byte
    cpu->byteAmount = 1; // 8 bits for loading a byte
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Loading byte from memory
}

void CPU::ALU::LH() { // Load Half Word
    cpu->byteAmount = 2; //  16 bits for half word
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Loading half word from memory
}

void CPU::ALU::LW() { // Load Word
    std::cout << "LW" << std::endl;
    cpu->byteAmount = 4; // 32 bits for a word
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Loading word from memory
}

void CPU::ALU::LBU() { // Load Byte and zero extend it
    cpu->byteAmount = 1; // 8 bits for loading a byte
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Loading byte from memory
}
//
void CPU::ALU::LBH() { // Load Half Word and zero extend it
    cpu->byteAmount = 2; // 16 bits for half word
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Loading half word from memory and zero extend
}

void CPU::ALU::SB() { // Storing byte
    cpu->byteAmount = 1; // Specify an 8 bit value to be stored
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    if(cpu->ex->intInst.rs2.getName().to_string() == "00001"){
        int tmp = cpu->currAddrI;
        cpu->ex->intInst.rs2.setData(tmp);
    }

    cpu->ex->intInst.data = cpu->ex->intInst.rs2.getData();

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Storing word to memory
}

void CPU::ALU::SH() { // Storing half word
    cpu->byteAmount = 2; // Specify 16 bit value to be stored
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    if(cpu->ex->intInst.rs2.getName().to_string() == "00001"){
        int tmp = cpu->currAddrI;
        cpu->ex->intInst.rs2.setData(tmp);
    }

    cpu->ex->intInst.data = cpu->ex->intInst.rs2.getData();

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Storing word to memory
}

void CPU::ALU::SW() { // Storing Word
    std::cout << "SW" << std::endl;
    cpu->byteAmount = 4; // Specify 32 bit value to be stored
    int imm;

    // If previous instruction is lui then abs the imm
    if(cpu->s->intInst.opcode.to_string() == "1110110")
        imm = cpu->ex->intInst.immISB.to_ulong();
    else
        imm =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value = 10000000000 = 1024

    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    if(cpu->ex->intInst.rs2.getName().to_string() == "00001"){
        int tmp = cpu->currAddrI - 8;
        cpu->ex->intInst.rs2.setData(tmp);
    }

    cpu->ex->intInst.data = cpu->ex->intInst.rs2.getData();

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->setMemAccessFinished(0);
    cpu->processData(); // Storing word to memory
}

void CPU::ALU::LUI() {
    int val = cpu->ex->intInst.immJU.to_ulong() << 12;
    val = abs(val);

    std::cout << "imm: " << val << std::endl;
    cpu->ex->intInst.rd.setData(val); // Loading upper 20 bits to rd with lower 12 bits 0
}

void CPU::ALU::AUIPC() {
    int offset = cpu->ex->intInst.immJU.to_ulong() << 12; // Immidate value
    std::cout << "offset: " << offset << std::endl;

    std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
    cpu->currAddrI += offset; // Adding offset to pc
    int val2 = cpu->currAddrI;
    std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;

    cpu->ex->intInst.rd.setData(val2); // Saving the current address in rd

    // The PC changes so the pipeline stages need to be flushed
    cpu->f->intInst.flush();
    cpu->d->intInst.flush();
}

void CPU::ALU::ADD() {
    int val1 = cpu->ex->intInst.rs1.getData();    // rs1
    int val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->intInst.rd.setData(val1 + val2); // Adding 2 int values
}

void CPU::ALU::BEQ(){ // Branch Equal
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();     // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;

    if(val == val2){
        int val3 = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value
        cpu->currAddrI += val3;

        // The PC changes so the pipeline stages need to be flushed
        cpu->f->intInst.flush();
        cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::BNE(){ // Branch Not Equal
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();     // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;

    if(val != val2){
        int val3 = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value
        cpu->currAddrI += val3;

        // The PC changes so the pipeline stages need to be flushed
        cpu->f->intInst.flush();
        cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::BLT() {  // Branch Rs1 Less Than Rs2
    std::cout << "BLT" << std::endl;
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();    // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "val: " << val << std::endl;
    std::cout << "val2: " << val2 << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;
    std::cout << "[keyword] imm: " << cpu->ex->intInst.immISB.to_ulong() << std::endl;

    if(val < val2){
        int val3 = cpu->ex->intInst.immISB.to_ulong();
        cpu->currAddrI = val3;

        // The PC changes so the pipeline stages need to be flushed
        // cpu->f->intInst.flush();
        // cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::BLTU() {  // Branch Rs1 Less Than Rs2 Unsigned
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();    // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;

    if(abs(val) < abs(val2)){
        int val3 = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value
        cpu->currAddrI += val3;

        // The PC changes so the pipeline stages need to be flushed
        // cpu->f->intInst.flush();
        // cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::BGE() {  // Branch Rs1 greater than Rs2
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();    // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;

    if(val > val2){
        int val3 = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value
        cpu->currAddrI += val3;

        // The PC changes so the pipeline stages need to be flushed
        // cpu->f->intInst.flush();
        // cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::BGEU() {  // Branch Rs1 greater than Rs2 Unsigned
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();    // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rs2.getName() << std::endl;

    if(abs(val) > abs(val2)){
        int val3 = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // Immidate value
        cpu->currAddrI += val3;

        // The PC changes so the pipeline stages need to be flushed
        // cpu->f->intInst.flush();
        // cpu->d->intInst.flush();
        cpu->f->setFlushed(1);
    }
}

void CPU::ALU::JAL() {
     // Jumping back to an address
     std::cout << "JAL" << std::endl;
     std::cout << "imm in bits: " << cpu->ex->intInst.immJU << std::endl;
     int imm = cpu->ex->intInst.immJU.to_ulong();
     std::cout << "imm: " << imm << std::endl;
     std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
     cpu->currAddrI = imm;
     std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;
     int val = cpu->currAddrI;

     // Storing address in rd
     cpu->ex->intInst.rd.setData(val);

     // The PC changes so the pipeline stages need to be flushed
     // cpu->f->intInst.flush();
     // cpu->d->intInst.flush();
     cpu->f->setFlushed(1);
}

void CPU::ALU::JALR() {
    std::cout << "rs1 name:" << cpu->ex->intInst.rs1.getName() << std::endl;

    int val = cpu->ex->intInst.rs1.getData();    // rs1
    int imm = cpu->ex->intInst.immJU.to_ulong();
    std::cout << "imm: " << imm << std::endl;
    std::bitset<32> addrs = val + imm;
    addrs[0] = 0;

    std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
    cpu->currAddrI = addrs.to_ulong();
    std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;

    int val2 = cpu->currAddrI;
    cpu->ex->intInst.rd.setData(val2);

    // The PC changes so the pipeline stages need to be flushed
    // cpu->f->intInst.flush();
    // cpu->d->intInst.flush();

    cpu->f->setFlushed(1);
}

void CPU::ALU::FADDS() {
    // rd = rs1 + rs2
    float val1, val2;

    val1 = cpu->ex->fInst.rs1.getData();    // rs1
    val2 = cpu->ex->fInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->fInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->fInst.rs2.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->fInst.rd.getName() << std::endl;

    cpu->ex->fInst.rd.setData(val1 + val2); // Adding 2 float values
}

void CPU::ALU::FSUBS() {
    // rd = rs1 + rs2
    float val1, val2;

    val1 = cpu->ex->fInst.rs1.getData();    // rs1
    val2 = cpu->ex->fInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->fInst.rs1.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->fInst.rs2.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->fInst.rd.getName() << std::endl;

    cpu->ex->fInst.rd.setData(val2 - val1); // Subtracting 2 float values
}

void CPU::ALU::FLW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->fInst.immISB.to_string(), 12); // Immidate value
    size_t addrs =  +  cpu->ex->fInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->fInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->fInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->processData(); // Loading word from memory
}

void CPU::ALU::FSW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->fInst.immISB.to_string(), 12); // Immidate value
    size_t addrs =  +  cpu->ex->fInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->fInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->fInst.rd.getName() << std::endl;

    cpu->ex->fInst.data = cpu->ex->fInst.rs2.getData();
    cpu->processData(); // Storing word to memory
}


double CONVERT_TYPE(std::string input){
    long double numberIn;

    std::stringstream ss(input);
    ss >> numberIn;

    return numberIn;
}

// Converts a passed in double to a string
std::string CONVERT_TYPE(long double input){
    std::ostringstream ss;

    ss << input;
    std::string numberIn = ss.str();

    return numberIn;
}

std::string Decimal2SignedBinary(double input){
    std::string result;
    // Convert to double
    int numberIn = input;

    int i = 0;
   for(i = 31; i >= 0; i--){
     if((numberIn & (1 << i)) != 0){
       result.push_back('1');
     }else{
       result.push_back('0');
     }
   }

   int n = result.length();

    for (size_t i = n-1 ; i >= 0 ; i--)
        if (result[i] == '1')
            break;

    if (i == -1)
        return '1' + result;

    for (int k = i-1 ; k >= 0; k--)
    {
        if (result[k] == '1')
            result[k] = '0';
        else
            result[k] = '1';
    }

    return result;
}

int Binary2Decimal(std::string binary, int significantBits){
  int power = pow(2,significantBits-1);
  int sum = 0;
  int i;

  for (i=0; i<significantBits; ++i)
  {
      if ( i==0 && binary[i]!='0')
      {
          sum = power * -1;
      }
      else
      {
          sum += (binary[i]-'0')*power;//The -0 is needed
      }
      power /= 2;
  }

  if(binary[0] == '0') {
    if(sum >= 0) {
      return sum;
    } else {
      return -1 * sum;
    }
  } else {
    if(sum >= 0) {
      return -1 * sum;
    } else {
      return sum;
    }
  }
}
