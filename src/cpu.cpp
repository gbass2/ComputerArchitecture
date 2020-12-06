#include "cpu.hh"

using namespace std;

template<size_t N>
bitset<N> reverse(const bitset<N> &bit_set);

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
            intInst.rd.setName(reverse(intInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            intInst.rs1.setName(reverse(intInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.rs2.setName(reverse(intInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.funct7 = bitset<7>(instruction.substr(25,7));
        } else if(intInst.type == "I"){
            intInst.opcode =  bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.rd.setName(reverse(intInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs1.setName(reverse(intInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.immISB = bitset<12>(instruction.substr(20,12)); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.immISB = reverse(intInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
        } else if(intInst.type == "S"){
            intInst.opcode =  bitset<7>(instruction.substr(0,7));
            intInst.immISB = bitset<12>(instruction.substr(7,5) + instruction.substr(25,7));
            intInst.immISB = reverse(intInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));

            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            intInst.rs1.setName(reverse(intInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.rs2.setName(reverse(intInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
        } else if(intInst.type == "U"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.rd.setName(reverse(intInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.immJU =  bitset<20>(instruction.substr(12,20));
            intInst.immJU = reverse(intInst.immJU);  // reversing the because the instruction reads left to right and the risc v doc reads right to left
        } else if(intInst.type == "B"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.immISB =  bitset<12>(instruction.substr(8,4) + instruction.substr(25,6) + instruction.substr(7,1) + instruction.substr(31,1));
            intInst.immISB = reverse(intInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.funct3 = bitset<3>(instruction.substr(12,3));
            intInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
            intInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
            intInst.rs1.setName(reverse(intInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            intInst.rs2.setName(reverse(intInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
        } else if(intInst.type == "J"){
            intInst.opcode = bitset<7>(instruction.substr(0,7));
            intInst.rd.setName(bitset<5>(instruction.substr(7,5)));
            intInst.immJU = bitset<20>(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
            intInst.immJU = reverse(intInst.immJU);  // reversing the because the instruction reads left to right and the risc v doc reads right to left
        } else {
            // cout << "ERROR Code 1234: No Integer Instruction Type." << endl;
            // assert(0);
            cout << "NOP in decode" << endl;
        }
    } else {
            if(fInst.type == "R"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.rs2.setName(reverse(fInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            } else if(fInst.type == "I"){
                fInst.opcode =  bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.immISB = bitset<12>(instruction.substr(20,12)); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.immISB = reverse(fInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            } else if(fInst.type == "S"){
                fInst.opcode =  bitset<7>(instruction.substr(0,7));
                fInst.immISB = bitset<12>(instruction.substr(7,5) + instruction.substr(25,7));
                fInst.immISB = reverse(fInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.rs2.setName(reverse(fInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            } else if(fInst.type == "U"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.immJU =  bitset<20>(instruction.substr(12,20));
                fInst.immJU = reverse(fInst.immJU);  // reversing the because the instruction reads left to right and the risc v doc reads right to left
            } else if(fInst.type == "B"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.immISB =  bitset<12>(instruction.substr(8,4) + instruction.substr(25,6) + instruction.substr(7,1) + instruction.substr(31,1));
                fInst.immISB = reverse(fInst.immISB); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.rs2.setName(reverse(fInst.rs2.getName())); // reversing the because the instruction reads left tion reads left to right and the risc v doc reads right to left
            } else if(fInst.type == "J"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.immJU = bitset<20>(instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
            } else if(fInst.type == "R4"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.rs2.setName(reverse(fInst.rs2.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct2 = bitset<2>(instruction.substr(25,2));
                fInst.rs3.setName(bitset<5>(instruction.substr(27,5)));
                fInst.rs3.setName(reverse(fInst.rs1.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
            } else {
                // cout << "ERROR Code 1234: No Floating Point Instruction Type." << endl;
                // assert(0);
                cout << "NOP in decode" << endl;
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

    // Accessing the registers to get the Values
    cpu->reg->setRead(1);
    cpu->reg->process();

    cpu->a->process();
    // Add the latencies in for the instructions. RV32I is 10 ticks, RV32M is 20 ticks, RV32F si 50 ticks
}

// Stores the data from execute into destination register
void CPU::Store::storeInstruction() {
    cout << endl << "Processing Store Stage for " << cpu->getName() << endl;
    cout << "rd name: "  << cpu->s->intInst.rd.getName() << endl;
    cout << "rd data: "  << cpu->s->intInst.rd.getData() << endl << endl;

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

        // Passing the instruction of execute to store for int instruction
        cpu->s->intInst = cpu->ex->intInst;

        // Passing the instruction of decode to execute for int instruction
        cpu->ex->intInst = cpu->d->intInst;

        // Passing the instruction of fetch to decode for int instruction
        cpu->d->intInst = cpu->f->intInst;

        // Passing the instruction of execute to store for float instruction
        cpu->s->fInst = cpu->ex->fInst;

        // Passing the instruction of decode to execute for float instruction
        cpu->ex->fInst = cpu->d->fInst;

        // Passing the instruction of fetch to decode for float instruction
        cpu->d->fInst = cpu->f->fInst;

        // Passing the rest of the pipeline parameters to next stages
        cpu->s->setRead(cpu->ex->isRead());
        cpu->s->setMemAccess(cpu->ex->isMemAccess());
        cpu->s->setFloat(cpu->ex->getIsFloat());

        cpu->ex->setRead(cpu->d->isRead());
        cpu->ex->setMemAccess(cpu->d->isMemAccess());
        cpu->ex->setFloat(cpu->d->getIsFloat());

        cpu->d->setRead(cpu->f->isRead());
        cpu->d->setMemAccess(cpu->f->isMemAccess());
        cpu->d->setFloat(cpu->f->getIsFloat());
    }

    // If pipeline stages are busy reschedule release event
    else{
        cout << "Stage is busy. Rescheduling send data" << endl;
        cpu->schedule(this, cpu->currTick() + 1);
    }
}

void CPU::Decode::findInstructionType(){
     // base 10 multiply 20 float 50
    if(intInst.opcode.to_string() == "1110110"){ // LUI
        intInst.type = "U";
        intInst.set = 10; // 10 sim ticks
        setRead(1);
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
         setRead(0);
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
         fInst.type = "I";
         fInst.set = 50; // 50 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1110010"){  // FSW
         fInst.type = "S";
         fInst.set = 50; // 50 sim ticks
         setMemAccess(1);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1100001"){  // FMADD.S
         fInst.type = "R4";
         fInst.set = 50; // 50 sim ticks
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1100101"){  // FADD.S
         fInst.type = "R";
         fInst.set = 50; // 50 sim ticks
         setFloat(1);
    }
}

void CPU::recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;
        pkt->printHeader();

        if((f->isBusy() && f->isRead()) && !ex->isBusy()){      // only true for fetch stage
            // Reading from memory in binary
            bitset<32> instruction = *(uint32_t *)(pkt->getBuffer());
            cout << getName() << " read in binary: " << instruction << endl;
            f->intInst.currentInstruction = instruction;

            if(currAddrI < endAddrI){
                send->sendEvent();   // Scheduling send data
                d->e->decodeEvent(); // Scheduling decode
                f->e->fetchEvent();  // Scheduling fetch
            }
            f->setBusy(0);  // Setting fetch stage to not busy
        } else if(ex->isBusy() && ex->isRead()){
            // Reading int from data memory
            if(!ex->getIsFloat()){
                int val = (*(int *)(pkt->getBuffer())); // Loading into rd. The store stage will get the data and store it in the proper location
                cout << "Loaded value: " << val << endl;

                if(ex->intInst.funct3.to_string() == "101"){ // LBH Zero Extend
                    val = val << 16;
                }
                else if(ex->intInst.funct3.to_string() == "100"){ // LBU Zero extend
                    val = val << 24;
                }
                ex->intInst.rd.setData(val);
            }
            // Reading float from memory
            else
                ex->fInst.rd.setData(*(float *)(pkt->getBuffer()));

            ex->setBusy(0); // Setting execute stage to not busy
        } else {
            cout << "Successfully Stored Data to Memory" << std::endl;
            ex->setBusy(0); // Setting execute stage to not busy
        }
            delete pkt;

    }

template<size_t N>
bitset<N> reverse(const bitset<N> &bit_set) {
  std::bitset<N> reversed;
  for (int i = 0, j = N - 1; i < N; i++, j--) {
    reversed[j] = bit_set[i];
  }
  return reversed;
}
