#include "register.hh"
#include "simobject.hh"
#include "cpu.hh"

// Creating the integer registers and floating point registers using a hash table to hold the register and its name
RegisterBank::RegisterBank(std::shared_ptr<System> s2, CPU *c): SimObject(s2), release(new RegisterReleaseEvent(this)), regProcess(new RegisterProcessEvent(this)), cpu(c)  {
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00000, Register<int>()));    // x0 zero Hard-Wired Zero
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00001, Register<int>()));    // x1 ra Return Address
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00010, Register<int>()));    // x2 sp Stack Pointer
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00011, Register<int>()));    // x3 gp Global Pointer
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00100, Register<int>()));    // x4 tp Thread Pointer
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00101, Register<int>()));    // x5 t0 Temporary/alternate Link Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00110, Register<int>()));    // x6 t1 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register<int>>(00111, Register<int>()));    // x7 t2 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01000, Register<int>()));    // x8 s0/fp Saved Register/Frame Pointer
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01001, Register<int>()));    // x9 s1 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01010, Register<int>()));    // x10 a0 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01011, Register<int>()));    // x11 a1 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01100, Register<int>()));    // x12 a2 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01101, Register<int>()));    // x13 a3 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01110, Register<int>()));    // x14 a4 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(01111, Register<int>()));    // x15 a5 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10000, Register<int>()));    // x16 a6 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10001, Register<int>()));    // x17 a7 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10010, Register<int>()));    // x18 s2 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10011, Register<int>()));    // x19 s3 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10100, Register<int>()));    // x20 s4 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10101, Register<int>()));    // x21 s5 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10110, Register<int>()));    // x22 s6 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(10111, Register<int>()));    // x23 s7 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11000, Register<int>()));    // x24 s8 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11001, Register<int>()));    // x25 s9 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11010, Register<int>()));    // x26 s10 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11011, Register<int>()));    // x27 s11 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11100, Register<int>()));    // x28 t3 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11101, Register<int>()));    // x29 t4 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11110, Register<int>()));    // x30 t5 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register<int>>(11111, Register<int>()));    // x31 t6 Temporaries

    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00000, Register<float>()));    // f0 ft0 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00001, Register<float>()));    // f1 ft1 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00010, Register<float>()));    // f2 ft2 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00011, Register<float>()));    // f3 ft3 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00100, Register<float>()));    // f4 ft4 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00101, Register<float>()));    // f5 ft5 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00110, Register<float>()));    // f6 ft6 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(00111, Register<float>()));    // f7 ft7 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01000, Register<float>()));    // f8 fs0 Saved Register
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01001, Register<float>()));    // f9 fs1 Saved Register
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01010, Register<float>()));    // f10 fa0 FP arguments/return values
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01011, Register<float>()));    // f11 fa1 FP arguments/return values
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01100, Register<float>()));    // f12 fa2 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01101, Register<float>()));    // f13 fa3 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01110, Register<float>()));    // f14 fa4 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(01111, Register<float>()));    // f15 fa5 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10000, Register<float>()));    // f16 fa6 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10001, Register<float>()));    // f17 fa7 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10010, Register<float>()));    // f18 fs2 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10011, Register<float>()));    // f19 fs3 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10100, Register<float>()));    // f20 fs4 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10101, Register<float>()));    // f21 fs5 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10110, Register<float>()));    // f22 fs6 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(10111, Register<float>()));    // f23 fs7 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11000, Register<float>()));    // f24 fs8 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11001, Register<float>()));    // f25 fs9 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11010, Register<float>()));    // f26 fs10 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11011, Register<float>()));    // f27 fs11 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11100, Register<float>()));    // f28 ft8 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11101, Register<float>()));    // f29 ft9 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11110, Register<float>()));    // f30 ft10 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register<float>>(11111, Register<float>()));    // f31 ft11 FP temporaries
}

void RegisterBank::process(){
    std::cout << "Register Access" << std::endl << std::endl;

    std::bitset<5> name; // The name of the register in bitset
    uint8_t nameInInt; // The name of the register in uint8_t. Needed because the unordered_map uses a uint8_t

    if(cpu->d->isRead()){ // Read from register
        if(!(cpu->d->getIsFloat())){ // For a int read
            // Retrieving Rs1
            name = cpu->d->intInst.rs1.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->intInst.rs1 = intRegisters[nameInInt];
            cpu->d->intInst.rs1.setName(name);

            // Retrieving Rs2
            name = cpu->d->intInst.rs2.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->intInst.rs2 = intRegisters[nameInInt];
            cpu->d->intInst.rs2.setName(name);

            // Retrieving Rs3
            name= cpu->d->intInst.rs3.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->intInst.rs3 = intRegisters[nameInInt];
            cpu->d->intInst.rs3.setName(name);


        } else if((cpu->d->getIsFloat())){ // For a floating point read
            // Retrieving Rs1
            name = cpu->d->fInst.rs1.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->fInst.rs1.setData(fpRegisters[nameInInt].getData());

            // Retrieving Rs2
            name = cpu->d->fInst.rs2.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->fInst.rs2.setData(fpRegisters[nameInInt].getData());

            // Retrieving Rs3
            name = cpu->d->fInst.rs3.getName();
            nameInInt = stoi(name.to_string());
            cpu->d->fInst.rs3.setData(fpRegisters[nameInInt].getData());
        }
    } else { // Write to register
        if(!(cpu->s->getIsFloat())){ // For a int write
            // Storing Rd
            name = cpu->s->intInst.rd.getName();    // 5bit value
            nameInInt = stoi(name.to_string());
            intRegisters[nameInInt] = cpu->s->intInst.rd;
        } else { // For a floating point write
            // Storing Rd
            name = cpu->s->fInst.rd.getName();
            nameInInt = stoi(name.to_string());
            fpRegisters[nameInInt].setData(cpu->s->fInst.rd.getData());
            fpRegisters[nameInInt].setName(name);
        }
        cpu->d->setBusy(0);
        cpu->ex->setBusy(0);
    }
}
