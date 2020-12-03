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
    cout << endl << "Processing Fetch Stage for " << cpu->getName() << endl;
    setBusy(1);
    setRead(1);
    cpu->processInst();
}

// Decodes the instruction into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    cout << endl << "Processing Decode Stage for " << cpu->getName() << endl;
    setBusy(1);

    string instruction = intInst.currentInstruction.to_string();
    // Finds the instruction type, if its a float ot int, and what set of instructions it is from
    intInst.opcode = bitset<7>(instruction.substr(0,7));
    intInst.funct3 = bitset<3>(instruction.substr(12,3));
    findInstructionType();

    cout << "instruction: " << instruction << endl;
    if(!isFloat){
        if(intInst.type == "R"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            intInst.funct7 = bitset<7>(instruction.substr(25,7));
        } else if(intInst.type == "I"){
            intInst.opcode =  bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.immISB = bitset<12>(instruction.substr(20,12));
        } else if(intInst.type == "S"){
            intInst.opcode =  bitset<7>(instruction.substr(0,7));
            intInst.immISB = bitset<12>(instruction.substr(7,5) + instruction.substr(25,7));
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
        } else if(intInst.type == "U"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.immJU =  bitset<20>(instruction.substr(12,20));
        } else if(intInst.type == "B"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.immISB =  bitset<12>(instruction.substr(8,4) + instruction.substr(25,6) + instruction.substr(7,1) + instruction.substr(31,1));
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
        } else if(intInst.type == "J"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(15,5)));
            intInst.immJU = bitset<20>(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
        }
    } else {
            if(fInst.type == "R"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.funct7 = bitset<7>(instruction.substr(25,7));
            } else if(fInst.type == "I"){
                fInst.opcode =  bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.immISB = bitset<12>(instruction.substr(20,12));
            } else if(fInst.type == "S"){
                fInst.opcode =  bitset<7>(instruction.substr(0,7));
                fInst.immISB = bitset<12>(instruction.substr(7,5) + instruction.substr(25,7));
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            } else if(fInst.type == "U"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.immJU =  bitset<20>(instruction.substr(12,20));
            } else if(fInst.type == "B"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.immISB =  bitset<12>(instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            } else if(fInst.type == "J"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(15,5)));
                fInst.immJU = bitset<20>(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
        }
    }

        // hazards check goes here
        // If so then stall, come back and access registers values
        // See hazard table for how long to stall
        // ---------------------------
        cpu->d->setBusy(0);
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {
    cout << endl << "Processing Execute Stage for " << cpu->getName() << endl;

    setBusy(1);
    cpu->a->process();
    // Add the latencies in for the instructions. RV32I is 10 ticks, RV32M is 20 ticks, RV32F si 50 ticks
}

// Stores the data from execute into destination register
void CPU::Store::storeInstruction() {
    cout << endl << "Processing Store Stage for " << cpu->getName() << endl;
    cpu->reg->setRead(0);
    cpu->reg->process();
    cpu->s->setBusy(0); // Setting store stage to not busy
}

void CPU::Stall::stallCPU() {
    cout << endl << "Reshceduling Events for " << cpu->getName() << endl;
    // Rescheduling the events in the MEQ for a specified time in the future. 1 stall = 10 ticks, 2 stalls = 20 ticks, etc.
    for(size_t i = 0; i < (cpu->sysMain->getMEQ()).size(); i++){
        cpu->reschedule((cpu->sysMain->getMEQ()).at(i), (cpu->sysMain->getMEQ()).at(i)->getTime() + stallAmount);
    }
    cout << endl;
}

void CPU::Send::sendData() {
    cout << endl << "Sending data between stages for " << cpu->getName() << endl << endl;
    //If pipeline stages are not busy then pass between stages
    if(!(cpu->f->isBusy() && cpu->d->isBusy() && cpu->ex->isBusy() &&cpu->s->isBusy())){
        // For an int instrcution
        // Passing execute to store
        cpu->s->intInst = cpu->ex->intInst;

        // Passing decode to execute
        cpu->ex->intInst = cpu->d->intInst;

        // Passing fetch to decode
        cpu->d->intInst = cpu->f->intInst;

        // For a fp instruction
        // Passing execute to store
        cpu->s->fInst = cpu->ex->fInst;

        // Passing decode to execute
        cpu->ex->fInst = cpu->d->fInst;

        // Passing fetch to decode
        cpu->d->fInst = cpu->f->fInst;
    }

    // If pipeline stages are busy reschedule release event
    else
        cpu->schedule(this, cpu->currTick() + 1);
}

void CPU::Decode::findInstructionType(){
     // base 10 multiply 20 float 50
    if(intInst.opcode.to_string() == "1110110"){ // LUI
        intInst.type = "U";
        intInst.set = 10; // 10 sim ticks
        setRead(1);
        setMemAccess(1);
        setFloat(0);
   } else if(intInst.opcode.to_string() == "1110100") {  // AUIPC
         intInst.type = "U";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1111011"){  // JAL
         intInst.type = "J";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1110011"){  // JALR
         intInst.type = "I";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100011"){  // BEQ
         intInst.type = "B";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100000"){  // LB
         intInst.type = "I";
         intInst.set = 10; // 10 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100010"){  // SB
         intInst.type = "S";
         intInst.set = 10; // 10 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100100"){  // ADDI
         intInst.type = "I";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100110" && intInst.funct3.to_string() == "111"){  // ADD
         intInst.type = "R";
         intInst.set = 10; // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100110"){  // MUL
         intInst.type = "R";
         intInst.set = 20; // 20 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1110000"){  // FLW
         intInst.type = "I";
         intInst.set = 50; // 50 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1110010"){  // FSW
         intInst.type = "S";
         intInst.set = 50; // 50 sim ticks
         setMemAccess(1);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1100001"){  // FMADD.S
         intInst.type = "R4";
         intInst.set = 50; // 50 sim ticks
         setFloat(1);
    }
}

void CPU::recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;

        if(f->isBusy() && f->isRead()){      // only true for fetch stage
            // Reading from memory in binary
            std::bitset<32> instruction = *(uint32_t *)(pkt->getBuffer());
            std::cout << getName() << " read in binary: " << instruction << std::endl;
            f->intInst.currentInstruction = instruction;

            if(currAddrI < endAddrI){
                send->sendEvent();   // Scheduling send data
                d->e->decodeEvent(); // Scheduling decode
                f->e->fetchEvent();  // Scheduling fetch
            }
        } else if(ex->isBusy() && ex->isRead()){
            // Reading int from data memory
            if(!ex->getIsFloat())
                ex->intInst.rd.setData(*(int *)(pkt->getBuffer())); // Loading into rd. The store stage will get the data and store it in the rpoper location

            // Reading float from memory
            else
                ex->fInst.rd.setData(*(float *)(pkt->getBuffer()));
        }
            f->setBusy(0);  // Setting fetch stage to not busy
            ex->setBusy(0); // Setting execute stage to not busy
    }
