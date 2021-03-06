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
    setMemAccessFinished(0);
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
            cout << "NOP in decode" << endl;
        }
    } else {
            if(fInst.type == "R"){
                fInst.opcode = bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName()));    // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct3 = bitset<3>(instruction.substr(12,3));
                fInst.rs1.setName(bitset<5>(instruction.substr(15,5)));
                fInst.rs2.setName(bitset<5>(instruction.substr(20,5)));
                fInst.rs1.setName(reverse(fInst.rs1.getName()));  // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.rs2.setName(reverse(fInst.rs2.getName()));  // reversing the because the instruction reads left to right and the risc v doc reads right to left
                fInst.funct7 = bitset<7>(instruction.substr(25,7));
            } else if(fInst.type == "I"){
                fInst.opcode =  bitset<7>(instruction.substr(0,7));
                fInst.rd.setName(bitset<5>(instruction.substr(7,5)));
                fInst.rd.setName(reverse(fInst.rd.getName())); // reversing the because the instruction reads left to right and the risc v doc reads right to left
                cout << "rd: " << fInst.rd.getName() << endl;
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
                cout << "NOP in decode" << endl;
            }
    }

    release->releaseEvent();
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {
    cout << endl << "Processing Execute Stage for " << cpu->getName() << endl;
    setBusy(1);

    // Accessing the registers to get the Values
    cpu->reg->setRead(1);
    cpu->reg->process();
    cpu->a->process();  // calls ALU
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

void CPU::Decode::findInstructionType(){
     // base 10 multiply 20 float 50
    if(intInst.opcode.to_string() == "1110110"){ // LUI
        intInst.type = "U";
        setLatency(10); // 10 sim ticks
        setRead(1);
        setFloat(0);
   } else if(intInst.opcode.to_string() == "1110100") {  // AUIPC
         intInst.type = "U";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1111011"){  // JAL
         intInst.type = "J";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1110011"){  // JALR
         intInst.type = "I";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100011"){  // BEQ
         intInst.type = "B";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100000"){  // LB
         intInst.type = "I";
         setLatency(10); // 10 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100010"){  // SB
         intInst.type = "S";
         setLatency(10); // 10 sim ticks
         setRead(0);
         setMemAccess(1);
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100100"){  // ADDI
         intInst.type = "I";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100110" && intInst.funct3.to_string() == "111"){  // ADD
         intInst.type = "R";
         setLatency(10); // 10 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1100110"){  // MUL
         intInst.type = "R";
         setLatency(20); // 20 sim ticks
         setFloat(0);
    } else if(intInst.opcode.to_string() == "1110000"){  // FLW
         fInst.type = "I";
         setLatency(50); // 50 sim ticks
         setRead(1);
         setMemAccess(1);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1110010"){  // FSW
         fInst.type = "S";
         setLatency(50); // 50 sim ticks
         setMemAccess(1);
         setRead(0);
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1100001"){  // FMADD.S
         fInst.type = "R4";
         setLatency(50); // 50 sim ticks
         setFloat(1);
    } else if(intInst.opcode.to_string() == "1100101"){  // FADD.S
         fInst.type = "R";
         setLatency(50); // 50 sim ticks
         setFloat(1);
    }
}

void CPU::recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;

        // Temp: Execute for load data should NOT be passing through this if statement
        if((pkt->getName() == "fetch" && pkt->isRead())){         // only true for fetch stage
            // Reading from memory in binary
            bitset<32> instruction = *(uint32_t *)(pkt->getBuffer());
            cout << getName() << " read in binary: " << instruction << endl << endl;    // example: cpu0 read in binary: 01010101010

            if(f->isFlushed()){
                f->intInst.currentInstruction.reset(); // Flushing the pipleine
                f->setFlushed(0);
            }
            else
                f->intInst.currentInstruction = instruction;

            f->setMemAccessFinished(1);
            f->release->releaseEvent();
        } else if(pkt->getName() == "execute" && pkt->isRead()){        // only true for execute
            // Reading int from data memory
            if(!ex->getIsFloat()){
                int val = (*(int *)(pkt->getBuffer())); // Loading into rd. The store stage will get the data and store it in the proper location
                cout << "Loaded value: " << val << endl << endl;

                if(ex->intInst.funct3.to_string() == "101")        // LBH Zero Extend
                    val = val << 16;
                else if(ex->intInst.funct3.to_string() == "100")   // LBU Zero extend
                    val = val << 24;

                ex->intInst.rd.setData(val);
            }
            // Reading float from memory
            else{
                cout << "Loaded fp value: " << *(float *)(pkt->getBuffer()) << endl << endl;
                ex->fInst.rd.setData(*(float *)(pkt->getBuffer()));
            }

            ex->setMemAccessFinished(1); // Setting execute stage to not busy
        } else {
            if(!ex->getIsFloat())
                cout << "Successfully Stored " << *(int *)(pkt->getBuffer()) << " to Memory" << endl << endl;
            else{
                cout << "Successfully Stored " << *(float *)(pkt->getBuffer()) << " to Memory" << endl << endl;
                output.push_back(*(float *)(pkt->getBuffer()));
            }

            ex->setMemAccessFinished(1); // Setting execute stage to not busy
        }
        delete pkt;
}

template<size_t N>
bitset<N> reverse(const bitset<N> &bit_set) {
  std::bitset<N> reversed;
  // reverses bitset
  for (int i = 0, j = N - 1; i < N; i++, j--) {
    reversed[j] = bit_set[i];
  }
  return reversed;
}
