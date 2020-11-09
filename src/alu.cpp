#include "simobject.hh"

// Determines what alu operation needs to be done
void CPU::ALU::aluOperation() {
    if(cpu->ex->opcode == "1100100") {      // addi/mv + slli
        if(cpu->ex->funct3 == "000") {       // addi/mv

        }
        else if(cpu->ex->funct3 == "001") { // slli

        }
    }
    else if(cpu->ex->opcode == "1100010") { // sw

    }
    else if(cpu->ex->opcode == "1100101") {
        if(cpu->ex->funct3 == "010") {       // fsw

        }
        else if(cpu->ex->funct3 == "111") {  // fadd.s

        }
    }
    else if(cpu->ex->opcode == "1111011") { // j

    }
    else if(cpu->ex->opcode == "1100000") { // lw

    }
    else if(cpu->ex->opcode == "1100011") { // blt

    }
    else if(cpu->ex->opcode == "1110110") { // lui

    }
    else if(cpu->ex->opcode == "1110000") { // flw

    }
    else if(cpu->ex->opcode == "1100110") { // add

    }
    else if(cpu->ex->opcode == "1100101") { // fsw

    }
    else if(cpu->ex->opcode == "1110011") { // ret/jalr

    }
}
