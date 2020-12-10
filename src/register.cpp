 #include "register.hh"
#include "simobject.hh"
#include "cpu.hh"

// Creating the integer registers and floating point registers using a hash table to hold the register and its name
RegisterBank::RegisterBank(std::shared_ptr<System> s2, CPU *c): SimObject(s2), cpu(c)  {
    intRegisters.insert(std::pair<uint16_t,Register<int>>(0, Register<int>(0)));    // x0 zero Hard-Wired Zero
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1, Register<int>(1)));    // x1 ra Return Address
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10, Register<int>(2)));    // x2 sp Stack Pointer
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11, Register<int>(3)));    // x3 gp Global Pointer
    intRegisters.insert(std::pair<uint16_t,Register<int>>(100, Register<int>(4)));    // x4 tp Thread Pointer
    intRegisters.insert(std::pair<uint16_t,Register<int>>(101, Register<int>(5)));    // x5 t0 Temporary/alternate Link Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(110, Register<int>(6)));    // x6 t1 Temporaries
    intRegisters.insert(std::pair<uint16_t,Register<int>>(111, Register<int>(7)));    // x7 t2 Temporaries
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1000, Register<int>(8)));    // x8 s0/fp Saved Register/Frame Pointer
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1001, Register<int>(9)));    // x9 s1 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1010, Register<int>(10)));    // x10 a0 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1011, Register<int>(11)));    // x11 a1 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1100, Register<int>(12)));    // x12 a2 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1101, Register<int>(13)));    // x13 a3 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1110, Register<int>(14)));    // x14 a4 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(1111, Register<int>(15)));    // x15 a5 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10000, Register<int>(16)));    // x16 a6 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10001, Register<int>(17)));    // x17 a7 Function Arguments
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10010, Register<int>(18)));    // x18 s2 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10011, Register<int>(19)));    // x19 s3 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10100, Register<int>(20)));    // x20 s4 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10101, Register<int>(21)));    // x21 s5 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10110, Register<int>(22)));    // x22 s6 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(10111, Register<int>(23)));    // x23 s7 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11000, Register<int>(24)));    // x24 s8 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11001, Register<int>(25)));    // x25 s9 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11010, Register<int>(26)));    // x26 s10 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11011, Register<int>(27)));    // x27 s11 Saved Register
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11100, Register<int>(28)));    // x28 t3 Temporaries
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11101, Register<int>(29)));    // x29 t4 Temporaries
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11110, Register<int>(30)));    // x30 t5 Temporaries
    intRegisters.insert(std::pair<uint16_t,Register<int>>(11111, Register<int>(31)));    // x31 t6 Temporaries

    fpRegisters.insert(std::pair<uint16_t,Register<float>>(0, Register<float>(0)));    // f0 ft0 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1, Register<float>(1)));    // f1 ft1 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10, Register<float>(2)));    // f2 ft2 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11, Register<float>(3)));    // f3 ft3 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(100, Register<float>(4)));    // f4 ft4 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(101, Register<float>(5)));    // f5 ft5 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(110, Register<float>(6)));    // f6 ft6 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(111, Register<float>(7)));    // f7 ft7 Temporary
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1000, Register<float>(8)));    // f8 fs0 Saved Register
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1001, Register<float>(9)));    // f9 fs1 Saved Register
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1010, Register<float>(10)));    // f10 fa0 FP arguments/return values
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1011, Register<float>(11)));    // f11 fa1 FP arguments/return values
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1100, Register<float>(12)));    // f12 fa2 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1101, Register<float>(13)));    // f13 fa3 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1110, Register<float>(14)));    // f14 fa4 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(1111, Register<float>(15)));    // f15 fa5 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10000, Register<float>(16)));    // f16 fa6 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10001, Register<float>(17)));    // f17 fa7 FP arguments
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10010, Register<float>(18)));    // f18 fs2 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10011, Register<float>(19)));    // f19 fs3 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10100, Register<float>(20)));    // f20 fs4 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10101, Register<float>(21)));    // f21 fs5 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10110, Register<float>(22)));    // f22 fs6 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(10111, Register<float>(23)));    // f23 fs7 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11000, Register<float>(24)));    // f24 fs8 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11001, Register<float>(25)));    // f25 fs9 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11010, Register<float>(26)));    // f26 fs10 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11011, Register<float>(27)));    // f27 fs11 FP saved registers
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11100, Register<float>(28)));    // f28 ft8 FP temporaries
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11101, Register<float>(29)));    // f29 ft9 FP temporaries
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11110, Register<float>(30)));    // f30 ft10 FP temporaries
    fpRegisters.insert(std::pair<uint16_t,Register<float>>(11111, Register<float>(31)));    // f31 ft11 FP temporaries
}

void RegisterBank::process(){
    std::cout << "Register Access" << std::endl;

    std::bitset<5> name; // The name of the register in bitset
    int nameInInt; // The name of the register in uint8_t. Needed because the unordered_map uses a uint8_t

    if(isRead()){ // Read from register
        if(!(cpu->ex->getIsFloat())){ // For a int read
            // Retrieving Rs1
            name = cpu->ex->intInst.rs1.getName();
            nameInInt = stoi(name.to_string());
            cpu->ex->intInst.rs1 = intRegisters[nameInInt];
            std::cout << "rs1 data: " << intRegisters[nameInInt].getData() << std::endl;

            // Retrieving Rs2
            name = cpu->ex->intInst.rs2.getName();
            nameInInt = stoi(name.to_string());
            cpu->ex->intInst.rs2 = intRegisters[nameInInt];
            std::cout << "rs2 data: " << intRegisters[nameInInt].getData() << std::endl;

            // Retrieving Rs3
            name = cpu->ex->intInst.rs3.getName();
            nameInInt = stoi(name.to_string());
            cpu->ex->intInst.rs3 = intRegisters[nameInInt];

        } else if((cpu->ex->getIsFloat())){ // For a floating point read
            // Retrieving Rs1
            name = cpu->ex->fInst.rs1.getName();
            nameInInt = stoi(name.to_string());

            if(cpu->ex->fInst.opcode.to_string() == "1110000"  ||  cpu->ex->fInst.opcode.to_string() == "1110010" ){ // If flw or fsw we need rs1 to be form the integer register bank
                cpu->ex->fInst.rs1.setData(intRegisters[nameInInt].getData());
                std::cout << "rs1 data: " << intRegisters[nameInInt].getData() << std::endl;
            } else {
                cpu->ex->fInst.rs1.setData(fpRegisters[nameInInt].getData());
                std::cout << "rs1 data: " << fpRegisters[nameInInt].getData() << std::endl;
            }

            // Retrieving Rs2
            name = cpu->ex->fInst.rs2.getName();
            nameInInt = stoi(name.to_string());
            cpu->ex->fInst.rs2.setData(fpRegisters[nameInInt].getData());
            std::cout << "rs2 data: " << fpRegisters[nameInInt].getData() << std::endl << std::endl;

            // Retrieving Rs3
            name = cpu->ex->fInst.rs3.getName();
            nameInInt = stoi(name.to_string());
            cpu->ex->fInst.rs3.setData(fpRegisters[nameInInt].getData());
            // std::cout << "rs3 data: " << fpRegisters[nameInInt].getData() << std::endl;
        }
    } else { // Write to register
        if(!(cpu->s->getIsFloat())){ // For a int write
            // Storing Rd
            name = cpu->s->intInst.rd.getName();    // 5bit value
            nameInInt = stoi(name.to_string());

            std::cout << "rd name: " << name << std::endl;
            std::cout << "rd val: " << cpu->s->intInst.rd.getData() << std::endl << std::endl;

            if(!cpu->s->intInst.rd.getName().none()) // Store rd if the register is not the zeros register
                intRegisters[nameInInt] = cpu->s->intInst.rd;

            // If the instruction is a return then set isFinished to 1
            if(cpu->s->intInst.opcode.to_string() == "1110011"){
                if((!(cpu->getName(), "CPU0")))
                    cpu->sysMain->isFinished0 = true;
                else
                    cpu->sysMain->isFinished1 = true;
            }

        } else { // For a floating point write
            // Storing Rd
            name = cpu->s->fInst.rd.getName();
            nameInInt = stoi(name.to_string());
            fpRegisters[nameInInt].setData(cpu->s->fInst.rd.getData());
            std::cout << "rd name: " << name << std::endl;
            std::cout << "rd val: " << fpRegisters[nameInInt].getData() << std::endl << std::endl;
        }
    }
}
