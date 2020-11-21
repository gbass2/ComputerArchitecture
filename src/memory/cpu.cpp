#include "cpu.hh"

// Fetches the instruction from memory
void CPU::Fetch::fetchInstruction() {

    if(isMemAccess){
        cout << "Fetch Stage" << endl << endl;
        // cpu->Iport->p1->process(); // The code does not like how mem is accessing the simobject
        // cpu->schedule(cpu->Iport->p1, cpu->currTick() + 1);
        // cpu->stall->setIsStalled(1);
        return;
    }

    // The 32bits of data from the 4 memory locations will be concatinated into one 32 bit long instruction and then sent to the decode stage
    // string binary = (currentInstruction1.getBinary()).to_string() + (currentInstruction2.getBinary()).to_string() + (currentInstruction3.getBinary()).to_string() + (currentInstruction4.getBinary()).to_string();
    //
    // cout << "instruction fetched: "  << binary << endl;
    // cout << "instruction Type: "  << currentInstruction1.getInstType() << endl;

    cpu->PC += 4;
}

// Deodes the instructions into registers, immediates, etc.
void CPU::Decode::decodeInstruction() {
    cout << "Decode Stage" << endl;
    string instruction = currentInstruction1.getBinary().to_string() + currentInstruction2.getBinary().to_string() + currentInstruction3.getBinary().to_string() + currentInstruction4.getBinary().to_string();
    if(cpu->reg->getRegisterAccess() == false){
        if(currentInstruction1.getInstType() == "R"){
            opcode = stoi((instruction.substr(0,6)));
            rd =  stoi((instruction.substr(7,5)));
            funct3 =  stoi((instruction.substr(12,3)));
            rs1 =  stoi((instruction.substr(15,5)));
            rs2 =  stoi((instruction.substr(20,5)));
            funct7 =  stoi((instruction.substr(25,7)));

        } else if(currentInstruction1.getInstType() == "I"){
            opcode =  stoi((instruction.substr(0,6)));
            rd =  stoi((instruction.substr(7,5)));
            funct3 =  stoi((instruction.substr(12,3)));
            rs1 =  stoi((instruction.substr(15,5)));
            imm = (instruction.substr(20,12));

        } else if(currentInstruction1.getInstType() == "S"){
            opcode =  stoi((instruction.substr(0,6)));
            imm = (instruction.substr(7,5) + instruction.substr(25,7));
            funct3 =  stoi((instruction.substr(12,3)));
            rs1 =  stoi((instruction.substr(15,5)));
            rs2 =  stoi((instruction.substr(20,5)));

        } else if(currentInstruction1.getInstType() == "U"){
            opcode =  stoi((instruction.substr(0,6)));
            rd =  stoi((instruction.substr(7,5)));
            imm =  stoi((instruction.substr(12,19)));

        } else if(currentInstruction1.getInstType() == "B"){
            opcode =  stoi((instruction.substr(0,6)));
            imm = (instruction.substr(8,4) + instruction.substr(25,7) + instruction.substr(7,1) + instruction.substr(31,1));
            funct3 =  stoi((instruction.substr(12,3)));
            rs1 =  stoi((instruction.substr(15,5)));
            rs2 =  stoi((instruction.substr(20,5)));

        } else if(currentInstruction1.getInstType() == "J"){
            opcode =  stoi((instruction.substr(0,6)));
            rd =  stoi((instruction.substr(15,5)));
            imm = (instruction.substr(21,10) + instruction.substr(20,1) + instruction.substr(12,8) + instruction.substr(31,1));
        }

        // hazards check goes here
        // If so then stall, come back and access registers values
        // See hazard table for how long to stall
        // ---------------------------

        cpu->reg->process();
        cpu->stall->isStalled = true;
        return;
    }
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {
    cpu->a->process();
}

// Prints the store instruction
void CPU::Store::storeInstruction() {
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
    // Passing fetch to decode
    // cpu->d->currentInstruction1 = cpu->f->currentInstruction1;
    // cpu->d->currentInstruction2 = cpu->f->currentInstruction2;
    // cpu->d->currentInstruction3 = cpu->f->currentInstruction3;
    // cpu->d->currentInstruction4 = cpu->f->currentInstruction3;

    // Passing decode to execute
    cpu->ex->rs1 = cpu->d->rs1;
    cpu->ex->rs2 = cpu->d->rs2;
    cpu->ex->rs3 = cpu->d->rs3;
    cpu->ex->rd = cpu->d->rd;
    cpu->ex->funct2 = cpu->d->funct2;
    cpu->ex->funct3 = cpu->d->funct3;
    cpu->ex->funct5 = cpu->d->funct5;
    cpu->ex->funct7= cpu->d->funct7;
    cpu->ex->opcode = cpu->d->opcode;
    cpu->ex->imm = cpu->d->imm;
    // cpu->ex->setInstruction(cpu->d->currentInstruction1);

    // Passing execute to store
    cpu->s->rd = cpu->ex->rd;
    cpu->s->immDestination = cpu->ex->immDestination;

}
