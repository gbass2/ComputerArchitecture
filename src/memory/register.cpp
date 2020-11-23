#include "register.hh"
#include "simobject.hh"

// Creating the integer registers and floating point registers using a hash table to hold the register and its name
RegisterBank::RegisterBank(std::shared_ptr<System> s2): SimObject(s2), Register(), release(new RegisterReleaseEvent(this)), regProcess(new RegisterProcessEvent(this))  {
    intRegisters.insert(std::pair<uint8_t,Register>(00000, Register()));    // x0 zero Hard-Wired Zero
    intRegisters.insert(std::pair<uint8_t,Register>(00001, Register()));    // x1 ra Return Address
    intRegisters.insert(std::pair<uint8_t,Register>(00010, Register()));    // x2 sp Stack Pointer
    intRegisters.insert(std::pair<uint8_t,Register>(00011, Register()));    // x3 gp Global Pointer
    intRegisters.insert(std::pair<uint8_t,Register>(00100, Register()));    // x4 tp Thread Pointer
    intRegisters.insert(std::pair<uint8_t,Register>(00101, Register()));    // x5 t0 Temporary/alternate Link Register
    intRegisters.insert(std::pair<uint8_t,Register>(00110, Register()));    // x6 t1 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register>(00111, Register()));    // x7 t2 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register>(01000, Register()));    // x8 s0/fp Saved Register/Frame Pointer
    intRegisters.insert(std::pair<uint8_t,Register>(01001, Register()));    // x9 s1 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(01010, Register()));    // x10 a0 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint8_t,Register>(01011, Register()));    // x11 a1 Function Arguments/Return Values
    intRegisters.insert(std::pair<uint8_t,Register>(01100, Register()));    // x12 a2 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(01101, Register()));    // x13 a3 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(01110, Register()));    // x14 a4 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(01111, Register()));    // x15 a5 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(10000, Register()));    // x16 a6 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(10001, Register()));    // x17 a7 Function Arguments
    intRegisters.insert(std::pair<uint8_t,Register>(10010, Register()));    // x18 s2 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(10011, Register()));    // x19 s3 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(10100, Register()));    // x20 s4 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(10101, Register()));    // x21 s5 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(10110, Register()));    // x22 s6 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(10111, Register()));    // x23 s7 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(11000, Register()));    // x24 s8 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(11001, Register()));    // x25 s9 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(11010, Register()));    // x26 s10 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(11011, Register()));    // x27 s11 Saved Register
    intRegisters.insert(std::pair<uint8_t,Register>(11100, Register()));    // x28 t3 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register>(11101, Register()));    // x29 t4 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register>(11110, Register()));    // x30 t5 Temporaries
    intRegisters.insert(std::pair<uint8_t,Register>(11111, Register()));    // x31 t6 Temporaries

    fpRegisters.insert(std::pair<uint8_t,Register>(00000, Register()));    // f0 ft0 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00001, Register()));    // f1 ft1 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00010, Register()));    // f2 ft2 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00011, Register()));    // f3 ft3 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00100, Register()));    // f4 ft4 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00101, Register()));    // f5 ft5 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00110, Register()));    // f6 ft6 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(00111, Register()));    // f7 ft7 Temporary
    fpRegisters.insert(std::pair<uint8_t,Register>(01000, Register()));    // f8 fs0 Saved Register
    fpRegisters.insert(std::pair<uint8_t,Register>(01001, Register()));    // f9 fs1 Saved Register
    fpRegisters.insert(std::pair<uint8_t,Register>(01010, Register()));    // f10 fa0 FP arguments/return values
    fpRegisters.insert(std::pair<uint8_t,Register>(01011, Register()));    // f11 fa1 FP arguments/return values
    fpRegisters.insert(std::pair<uint8_t,Register>(01100, Register()));    // f12 fa2 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(01101, Register()));    // f13 fa3 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(01110, Register()));    // f14 fa4 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(01111, Register()));    // f15 fa5 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(10000, Register()));    // f16 fa6 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(10001, Register()));    // f17 fa7 FP arguments
    fpRegisters.insert(std::pair<uint8_t,Register>(10010, Register()));    // f18 fs2 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(10011, Register()));    // f19 fs3 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(10100, Register()));    // f20 fs4 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(10101, Register()));    // f21 fs5 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(10110, Register()));    // f22 fs6 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(10111, Register()));    // f23 fs7 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(11000, Register()));    // f24 fs8 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(11001, Register()));    // f25 fs9 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(11010, Register()));    // f26 fs10 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(11011, Register()));    // f27 fs11 FP saved registers
    fpRegisters.insert(std::pair<uint8_t,Register>(11100, Register()));    // f28 ft8 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register>(11101, Register()));    // f29 ft9 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register>(11110, Register()));    // f30 ft10 FP temporaries
    fpRegisters.insert(std::pair<uint8_t,Register>(11111, Register()));    // f31 ft11 FP temporaries
}
