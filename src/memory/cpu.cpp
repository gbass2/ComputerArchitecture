#include "cpu.hh"

using namespace std;

CPU::CPU(std::shared_ptr<System> s1, const char* name, size_t start1, size_t end1, size_t start2, size_t end2) :
    SimObject(s1, name),
    f(new Fetch(this)),
    d(new Decode(this)),
    ex(new Execute(this)),
    s(new Store(this)),
    stall(new Stall(this)),
    a(new ALU(this)),
    send(new Send(this)),
    reg(new RegisterBank(s1)),
    e1(new RequestInstEvent(this)),
    e2(new RequestDataEvent(this)),
    port1(new RequestInstPort(this)),
    port2(new RequestDataPort(this)),
    clkTick(10),
    currAddrI(start1),
    currAddrD(start2),
    endAddrI(end1),
    endAddrD(end2) {}

// Fetches the instruction from memory
void CPU::Fetch::fetchInstruction() {
    cout << "Processing Fetch Stage" << endl << endl;
    setBusy(1);
    cpu->processInst();
}

// Deodes the instructions into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    cout << "Processing Decode Stage" << endl << endl;
    // string instruction = currentInstruction1.getBinary().to_string() + currentInstruction2.getBinary().to_string() + currentInstruction3.getBinary().to_string() + currentInstruction4.getBinary().to_string();
    // if(cpu->reg->getRegisterAccess() == false){
    //     if(currentInstruction1.getInstType() == "R"){
    //         opcode = stoi((instruction.substr(0,6)));
    //         rd =  stoi((instruction.substr(7,5)));
    //         funct3 =  stoi((instruction.substr(12,3)));
    //         rs1 =  stoi((instruction.substr(15,5)));
    //         rs2 =  stoi((instruction.substr(20,5)));
    //         funct7 =  stoi((instruction.substr(25,7)));
    //
    //     } else if(currentInstruction1.getInstType() == "I"){
    //         opcode =  stoi((instruction.substr(0,6)));
    //         rd =  stoi((instruction.substr(7,5)));
    //         funct3 =  stoi((instruction.substr(12,3)));
    //         rs1 =  stoi((instruction.substr(15,5)));
    //         imm = (instruction.substr(20,12));
    //
    //     } else if(currentInstruction1.getInstType() == "S"){
    //         opcode =  stoi((instruction.substr(0,6)));
    //         imm = (instruction.substr(7,5) + instruction.substr(25,7));
    //         funct3 =  stoi((instruction.substr(12,3)));
    //         rs1 =  stoi((instruction.substr(15,5)));
    //         rs2 =  stoi((instruction.substr(20,5)));
    //
    //     } else if(currentInstruction1.getInstType() == "U"){
    //         opcode =  stoi((instruction.substr(0,6)));
    //         rd =  stoi((instruction.substr(7,5)));
    //         imm =  stoi((instruction.substr(12,19)));
    //
    //     } else if(currentInstruction1.getInstType() == "B"){
    //         opcode =  stoi((instruction.substr(0,6)));
    //         imm = (instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
    //         funct3 =  stoi((instruction.substr(12,3)));
    //         rs1 =  stoi((instruction.substr(15,5)));
    //         rs2 =  stoi((instruction.substr(20,5)));
    //
    //     } else if(currentInstruction1.getInstType() == "J"){
    //         opcode =  stoi((instruction.substr(0,6)));
    //         rd =  stoi((instruction.substr(15,5)));
    //         imm = (instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
    //     }
    //
    //     // hazards check goes here
    //     // If so then stall, come back and access registers values
    //     // See hazard table for how long to stall
    //     // ---------------------------
    //
    //     cpu->reg->process();
    //     cpu->stall->isStalled = true;
    //     return;
    // }
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {
    cout << "Processing Execute Stage" << endl << endl;
    // cpu->a->process();
}

// Prints the store instruction
void CPU::Store::storeInstruction() {
    cout << "Processing Store Stage" << endl << endl;
    // Create a register access event
    // Store back into register
}

void CPU::Stall::stallCPU() {
    cout << endl << "Reshceduling Events" << endl;
    // Rescheduling the events in the MEQ for a specified time in the future. 1 stall = 10 ticks, 2 stalls = 20 ticks, etc.
    for(size_t i = 0; i < (cpu->sysMain->getMEQ()).size(); i++){
        cpu->reschedule((cpu->sysMain->getMEQ()).at(i), (cpu->sysMain->getMEQ()).at(i)->getTime() + stallAmount);
    }
    cout << endl;
}

void CPU::Send::sendData() {
    //If pipeline stages are not busy then pass between stages
    if(!(cpu->f->isBusy() && cpu->d->isBusy() && cpu->ex->isBusy() &&cpu->s->isBusy())){
        // Passing fetch to decode
        cpu->d->inst = cpu->f->inst;
        // Passing decode to execute
        cpu->ex->inst = cpu->d->inst;
        // Passing execute to store
        cpu->s->inst = cpu->ex->inst;
    }

    // If pipeline stages are busy reschedule release event
    else
        cpu->schedule(this, cpu->currTick() + 1);
}
