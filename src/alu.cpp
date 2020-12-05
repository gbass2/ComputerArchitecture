#include "cpu.hh"
#include <sstream>
#include <limits.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include <bitset>
//
int Binary2Decimal(std::string binary, int significantBits);
std::string Decimal2Binary(std::string input);
float GetFloat32( std::string Binary );
std::string GetBinary32( float value );
//
// // Determines what alu operation needs to be done
void CPU::ALU::aluOperations() {

    if(!cpu->ex->getIsFloat()){ // Calling the functions to execute the integer instructions
        if(cpu->ex->intInst.opcode.to_string() == "1100100") {      // addi/mv + slli
            if(cpu->ex->intInst.funct3.to_string() == "000") {      // addi/mv
                ADDI();
            }
            else if(cpu->ex->intInst.funct3.to_string() == "001") { // slli
                SLLI();
            }
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100010") { // sw
            SW();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1111011") { // j
            J();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100000") { // lw
            LW();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100011") { // blt
            BLT();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110110") { // lui
            LUI();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110100"){
            AUIPC();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1100110") { // add
            ADD();
        }
        else if(cpu->ex->intInst.opcode.to_string() == "1110011") { // ret/jalr
            JALR();
        }
    } else { // Calling the functions to execute the fp instructions
        if(cpu->ex->fInst.opcode.to_string() == "1100101") {
            FADDS();
        }
        else if(cpu->ex->fInst.funct3.to_string() == "1110010") { // fadd.s
            FSW();
        }
        else if(cpu->ex->fInst.opcode.to_string() == "1110000") { // flw
                        std::cout << "FLW" << std::endl;
                FLW();
        }
    }
    if(!cpu->ex->isMemAccess()){
        std::cout << "# Set Busy to False" << std::endl;
        cpu->ex->setBusy(0); // Setting execute stage to not busy
    }
}

void CPU::ALU::ADDI() {
    // rd = rs1 + imm
    int val, val2;

    val = cpu->ex->intInst.rs1.getData();    // rs1
    val2 =  Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12); // immediate

    std::cout << "imm: " << val2<< std::endl;

    cpu->ex->intInst.rd.setData(val + val2); // Adding an int to an immediate value
}

void CPU::ALU::SLLI() {   // Logical left shift (zeros are shifted into the lower bits) PAGE 14

}

void CPU::ALU::LW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12);
    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->processData(); // Loading word from memory
}

void CPU::ALU::SW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->intInst.immISB.to_string(), 12);
    size_t addrs =  +  cpu->ex->intInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    if(cpu->ex->intInst.rs2.getName().to_string() == "00001"){
        int tmp = cpu->currAddrI;
        cpu->ex->intInst.rs2.setData(tmp);
    }

    cpu->ex->intInst.data = cpu->ex->intInst.rs2.getData();

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rs1.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->processData(); // Storing word to memory
}

void CPU::ALU::LUI() {
    std::string imm = cpu->ex->intInst.immJU.to_string() + "000000000000";
    int val = Binary2Decimal(imm, 32);

    std::cout << "imm: " << imm << std::endl;

    cpu->ex->intInst.rd.setData(val); // Loading upper 20 bits to rd with lower 12 bits 0
}

void CPU::ALU::AUIPC() {
    std::string imm = cpu->ex->intInst.immJU.to_string() + "000000000000";
    int offset = Binary2Decimal(imm, 32);
    std::cout << "offset: " << offset << std::endl;

    std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
    cpu->currAddrI += offset; // Adding offset to pc
    int val2 = cpu->currAddrI;
    std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;

    cpu->ex->intInst.rd.setData(val2); // Saving the current address in rd
}

void CPU::ALU::ADD() {
    int val1 = cpu->ex->intInst.rs1.getData();    // rs1
    int val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->intInst.rd.setData(val1 + val2); // Adding 2 int values
}

void CPU::ALU::BLT() {  // Branch Less Than
    int val1, val2;

    val1 = cpu->ex->intInst.rs1.getData();    // rs1
    val2 = cpu->ex->intInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->intInst.rd.getName() << std::endl;

    if(val1 < val2){
        int val3 = Binary2Decimal(cpu->ex->intInst.immJU.to_string(), 12);
        cpu->currAddrI += val3;
    }
}

void CPU::ALU::J() {
     // Jumping back to an address
     int imm = Binary2Decimal(cpu->ex->intInst.immJU.to_string(), 12);
     std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
     cpu->currAddrI += imm;
     std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;
     int val = cpu->currAddrI;

     std::cout << "imm: " << imm << std::endl;
     // Storing address in rd
     cpu->ex->intInst.rd.setData(val);
}

void CPU::ALU::JALR() {
    int val = cpu->ex->intInst.rs1.getData();    // rs1
    int imm = Binary2Decimal(cpu->ex->fInst.immJU.to_string(), 12);
    std::cout << "Current Address (Before): " << cpu->currAddrI << std::endl;
    cpu->currAddrI += val + imm;
    std::cout << "Current Address (After): " << cpu->currAddrI << std::endl;
    std::cout << "imm: " << val << std::endl;

    int val2 = cpu->currAddrI;
    cpu->ex->intInst.rd.setData(val2);
}

void CPU::ALU::FADDS() {
    // rd = rs1 + rs2
    float val1, val2;

    val1 = cpu->ex->fInst.rs1.getData();    // rs1
    val2 = cpu->ex->fInst.rs2.getData();    // rs2
    std::cout << "rs1 name: " << cpu->ex->fInst.rd.getName() << std::endl;
    std::cout << "rs2 name: " << cpu->ex->fInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->fInst.rd.getName() << std::endl;

    cpu->ex->fInst.rd.setData(val1 + val2); // Adding 2 float values
}

void CPU::ALU::FLW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->fInst.immISB.to_string(), 12);
    size_t addrs =  +  cpu->ex->fInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "Current address: " << addrs << std::endl;

    cpu->processData(); // Loading word from memory
}

void CPU::ALU::FSW() {
    cpu->byteAmount = 4; // 32 bits for a word
    int imm = Binary2Decimal(cpu->ex->fInst.immISB.to_string(), 12);
    size_t addrs =  +  cpu->ex->fInst.rs1.getData() + imm;
    cpu->currAddrD = addrs;

    std::cout << "imm: " << imm << std::endl;
    std::cout << "Current address: " << addrs << std::endl;
    std::cout << "rs1 name: " << cpu->ex->intInst.rd.getName() << std::endl;
    std::cout << "rd name: " << cpu->ex->intInst.rd.getName() << std::endl;

    cpu->ex->fInst.data = cpu->ex->fInst.rs2.getData();
    cpu->processData(); // Storing word to memory
}


double CONVERT_TYPE(std::string input){
    long double numberIn;

    std::stringstream ss(input);
    ss >> numberIn;

    return numberIn;
}

// Converts a passed in double to a string
std::string CONVERT_TYPE(long double input){
    std::ostringstream ss;

    ss << input;
    std::string numberIn = ss.str();

    return numberIn;
}

std::string Decimal2SignedBinary(double input){
    std::string result;
    // Convert to double
    int numberIn = input;

    int i = 0;
   for(i = 31; i >= 0; i--){
     if((numberIn & (1 << i)) != 0){
       result.push_back('1');
     }else{
       result.push_back('0');
     }
   }

   int n = result.length();

    for (size_t i = n-1 ; i >= 0 ; i--)
        if (result[i] == '1')
            break;

    if (i == -1)
        return '1' + result;

    for (int k = i-1 ; k >= 0; k--)
    {
        if (result[k] == '1')
            result[k] = '0';
        else
            result[k] = '1';
    }

    return result;
}

int Binary2Decimal(std::string binary, int significantBits){
  int power = pow(2,significantBits-1);
  int sum = 0;
  int i;

  for (i=0; i<significantBits; ++i)
  {
      if ( i==0 && binary[i]!='0')
      {
          sum = power * -1;
      }
      else
      {
          sum += (binary[i]-'0')*power;//The -0 is needed
      }
      power /= 2;
  }

  if(binary[0] == '0') {
    if(sum >= 0) {
      return sum;
    } else {
      return -1 * sum;
    }
  } else {
    if(sum >= 0) {
      return -1 * sum;
    } else {
      return sum;
    }
  }
}

// Convert the 32-bit binary encoding into hexadecimal
int Binary2Hex( std::string Binary )
{
    std::bitset<32> set(Binary);
    int hex = set.to_ulong();

    return hex;
}

// Convert the 32-bit binary into the decimal
float GetFloat32( std::string Binary )
{
    int HexNumber = Binary2Hex( Binary );

    bool negative  = !!(HexNumber & 0x80000000);
    int  exponent  =   (HexNumber & 0x7f800000) >> 23;
    int sign = negative ? -1 : 1;

    // Subtract 127 from the exponent
    exponent -= 127;

    // Convert the mantissa into decimal using the
    // last 23 bits
    int power = -1;
    float total = 0.0;
    for ( int i = 0; i < 23; i++ )
    {
        int c = Binary[ i + 9 ] - '0';
        total += (float) c * (float) pow( 2.0, power );
        power--;
    }
    total += 1.0;

    float value = sign * (float) pow( 2.0, exponent ) * total;

    return value;
}

// Get 32-bit IEEE 754 format of the decimal value
std::string GetBinary32( float value )
{
    union
    {
         float input;   // assumes sizeof(float) == sizeof(int)
         int   output;
    }    data;

    data.input = value;

    std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);

    std::string mystring = bits.to_string<char,
                                          std::char_traits<char>,
                                          std::allocator<char> >();

    return mystring;
}
