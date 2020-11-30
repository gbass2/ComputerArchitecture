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
    reg(new RegisterBank(s1, this)),
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
    setRead(1);
    cpu->processInst();
}

// Decodes the instruction into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    cout << "Processing Decode Stage" << endl << endl;

    string instruction = inst->currentInstruction.to_string();
    inst->opcode = bitset<7>(instruction.substr(0,7));
    inst->funct3 = bitset<3>(instruction.substr(12,3));
    findInstructionType();

    cout << "instruction: " << instruction << endl;
    if(inst->isFloat == 0){
        if(inst->type == "R"){
            inst->opcode = bitset<7>(instruction.substr(0,7));
            inst->rd.setName(bitset<5>(instruction.substr(7,5)));
            inst->funct3 = bitset<3>(instruction.substr(12,3));
            inst->rs1.setName(bitset<5>(instruction.substr(15,5)));
            inst->rs2.setName(bitset<5>(instruction.substr(20,5)));
            inst->funct7 = bitset<7>(instruction.substr(25,7));
        } else if(inst->type == "I"){
            inst->opcode =  bitset<7>(instruction.substr(0,7));
            inst->rd.setName(bitset<5>(instruction.substr(7,5)));
            inst->funct3 = bitset<3>(instruction.substr(12,3));
            inst->rs1.setName(bitset<5>(instruction.substr(15,5)));
            inst->imm = bitset<20>(instruction.substr(20,12));
        } else if(inst->type == "S"){
            inst->opcode =  bitset<7>(instruction.substr(0,7));
            inst->imm = bitset<20>(instruction.substr(7,5) + instruction.substr(25,7));
            inst->funct3 = bitset<3>(instruction.substr(12,3));
            inst->rs1.setName(bitset<5>(instruction.substr(15,5)));
            inst->rs2.setName(bitset<5>(instruction.substr(20,5)));
        } else if(inst->type == "U"){
            inst->opcode = bitset<7>(instruction.substr(0,7));
            inst->rd.setName(bitset<5>(instruction.substr(7,5)));
            inst->imm =  bitset<20>(instruction.substr(12,19));
        } else if(inst->type == "B"){
            inst->opcode = bitset<7>(instruction.substr(0,7));
            inst->imm =  bitset<20>(instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
            inst->funct3 = bitset<3>(instruction.substr(12,3));
            inst->rs1.setName(bitset<5>(instruction.substr(15,5)));
            inst->rs2.setName(bitset<5>(instruction.substr(20,5)));
        } else if(inst->type == "J"){
            inst->opcode = bitset<7>(instruction.substr(0,7));
            inst->rd.setName(bitset<5>(instruction.substr(15,5)));
            inst->imm = bitset<20>(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
        }
    }
//
//         // hazards check goes here
//         // If so then stall, come back and access registers values
//         // See hazard table for how long to stall
//         // ---------------------------
//
    cpu->reg->setRead(1);
    cpu->reg->scheduleRegisterEvent();
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
    cout << "Sending data between stages" << endl << endl;
    //If pipeline stages are not busy then pass between stages
    if(!(cpu->f->isBusy() && cpu->d->isBusy() && cpu->ex->isBusy() &&cpu->s->isBusy())){
        // Passing fetch to decode
        cpu->d->inst = cpu->f->inst;
        // Passing decode to execute
        cpu->ex->inst = cpu->d->inst;
        cpu->ex->inst = cpu->d->inst;
        // Passing execute to store
        cpu->s->inst = cpu->ex->inst;
    }

    // If pipeline stages are busy reschedule release event
    else
        cpu->schedule(this, cpu->currTick() + 1);
}

void CPU::Decode::findInstructionType(){
     // base 10 multiply 20 float 50
     cout << inst->opcode << std::endl;
    if(inst->opcode.to_string() == "1110110"){ // LUI
        inst->type = "U";
        inst->isFloat = false;
        inst->set = 10; // 10 sim ticks
   } else if(inst->opcode.to_string() == "1110100") {  // AUIPC
         inst->type = "U";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1111011"){  // JAL
         inst->type = "J";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1110011"){  // JALR
         inst->type = "I";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100011"){  // BEQ
         inst->type = "B";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100000"){  // LB
         inst->type = "I";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100010"){  // SB
         inst->type = "S";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100100"){  // ADDI
         inst->type = "I";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100110" && inst->funct3.to_string() == "111"){  // ADD
         inst->type = "R";
         inst->isFloat = false;
         inst->set = 10; // 10 sim ticks
    } else if(inst->opcode.to_string() == "1100110"){  // MUL
         inst->type = "R";
         inst->isFloat = false;
         inst->set = 20; // 20 sim ticks
    } else if(inst->opcode.to_string() == "1110000"){  // FLW
         inst->type = "I";
         inst->isFloat = true;
         inst->set = 50; // 50 sim ticks
    } else if(inst->opcode.to_string() == "1110010"){  // FSW
         inst->type = "S";
         inst->isFloat = true;
         inst->set = 50; // 50 sim ticks
    } else if(inst->opcode.to_string() == "1100001"){  // FMADD.S
         inst->type = "R4";
         inst->isFloat = true;
         inst->set = 50; // 50 sim ticks
    }
}
