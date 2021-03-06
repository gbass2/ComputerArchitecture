#include "cpu.hh"
#include <sstream>
#include <limits.h>
#include <iostream>
#include <bitset>
#include <math.h>
#include <bitset>
//
// int Binary2Decimal(std::string binary, int significantBits);
// std::string Decimal2Binary(std::string input);
// float GetFloat32( std::string Binary );
// std::string GetBinary32( float value );
//
// // Determines what alu operation needs to be done
void CPU::ALU::aluOperations() {
//     if(cpu->ex->opcode == "1100100") {      // addi/mv + slli
//         if(cpu->ex->funct3 == "000") {      // addi/mv
//             ADDI();
//         }
//         else if(cpu->ex->funct3 == "001") { // slli
//             SLLI();
//         }
//     }
//     else if(cpu->ex->opcode == "1100010") { // sw
//         SW();
//     }
//     else if(cpu->ex->opcode == "1100101") {
//         if(cpu->ex->funct3 == "010") {      // fsw
//             FSW();
//         }
//         else if(cpu->ex->funct3 == "111") { // fadd.s
//             FADDS();
//         }
//     }
//     else if(cpu->ex->opcode == "1111011") { // j
//         J();
//     }
//     else if(cpu->ex->opcode == "1100000") { // lw
//         LW();
//     }
//     else if(cpu->ex->opcode == "1100011") { // blt
//         BLT();
//     }
//     else if(cpu->ex->opcode == "1110110") { // lui
//         LUI();
//     }
//     else if(cpu->ex->opcode == "1110000") { // flw
//         FLW();
//     }
//     else if(cpu->ex->opcode == "1100110") { // add
//         ADD();
//     }
//     else if(cpu->ex->opcode == "1100101") { // fsw
//         FSW();
//     }
//     else if(cpu->ex->opcode == "1110011") { // ret/jalr
//         JALR();
//     std::cout << "hello" << std::endl;
//     }
}
//
// void CPU::ALU::ADDI() {
//     // rd = rs1 + imm
//     std::string val1, val2;
//     int val3, val4;
//
//     val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs1)]).getData().to_string();    // rs1
//     val2 = ((cpu->reg->intRegisters)[stoi(cpu->ex->imm)]).getData().to_string();    // imm
//
//     // integer values in base 10
//     val3 = Binary2Decimal(val1, 32);
//     val4 = Binary2Decimal(val2, 12);
//
//     cpu->ex->result =  Decimal2Binary(std::to_string(val3+val4));
//
//
//     // ((cpu->reg->intRegisters)[stoi(cpu->ex->rs1)]).setData(result);
//
// }
//
// void CPU::ALU::SLLI() {   // Logical left shift (zeros are shifted into the lower bits) PAGE 14
// // <<
// }
//
// void CPU::ALU::SW() {
//     // rs2 rs1 imm[4:0]
// }
//
// void CPU::ALU::FSW() {
//
// }
//
// void CPU::ALU::FADDS() {
//     // rd = rs1 + rs2
//     std::string val1, val2;
//     float val3, val4;
//
//     val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs1)]).getData().to_string();
//     val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs2)]).getData().to_string();
//
//     val3 = GetFloat32(val1);
//     val4 = GetFloat32(val2);
//
//     cpu->ex->result = GetBinary32(val3 + val4);
//
// }
//
// void CPU::ALU::J() {
//     cpu->PC = jump.front();
//     jump.pop_front();
//
// }
//
// void CPU::ALU::LW() {
//
// }
//
// void CPU::ALU::BLT() {
//     std::string val1, val2;
//     float val3, val4;
//
//     val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs1)]).getData().to_string();
//     val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs2)]).getData().to_string();
//
//     // integer values in base 10
//     val3 = Binary2Decimal(val1, 32);
//     val4 = Binary2Decimal(val2, 32);
//
//     if(val3 < val4){
//         cpu->PC = jump.front();
//         jump.pop_front();
//     }
// }
//
// void CPU::ALU::LUI() {
//
// }
//
// void CPU::ALU::FLW() {
//
// }
//
// void CPU::ALU::ADD() {
//   std::string val1, val2, val3;
//   val1 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rd)]).getData().to_string();
//   val2 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs1)]).getData().to_string();
//   val3 = ((cpu->reg->intRegisters)[stoi(cpu->ex->rs2)]).getData().to_string();
//
//   int vali1, vali2;
//   vali1 = Binary2Decimal(val1, 32);
//   vali2 = Binary2Decimal(val2, 32);
//
//   std::string result;
//
//   cpu->ex->result = Decimal2Binary(std::to_string(vali1 + vali2));
//
// }
//
// void CPU::ALU::JALR() {
//
// }
//
// void CPU::ALU::RET() {
//
// }
//
// double CONVERT_TYPE(std::string input){
//     long double numberIn;
//
//     std::stringstream ss(input);
//     ss >> numberIn;
//
//     return numberIn;
// }
//
// Converts a passed in double to a string
// std::string CONVERT_TYPE(long double input){
//     std::ostringstream ss;
//
//     ss << input;
//     std::string numberIn = ss.str();
//
//     return numberIn;
// }
//
// std::string Decimal2SignedBinary(double input){
//     std::string result;
//     // Convert to double
//     int numberIn = input;
//
//     int i = 0;
//    for(i = 31; i >= 0; i--){
//      if((numberIn & (1 << i)) != 0){
//        result.push_back('1');
//      }else{
//        result.push_back('0');
//      }
//    }
//
//    int n = result.length();
//
//     for (size_t i = n-1 ; i >= 0 ; i--)
//         if (result[i] == '1')
//             break;
//
//     if (i == -1)
//         return '1' + result;
//
//     for (int k = i-1 ; k >= 0; k--)
//     {
//         if (result[k] == '1')
//             result[k] = '0';
//         else
//             result[k] = '1';
//     }
//
//     return result;
// }
//
// int Binary2Decimal(std::string binary, int significantBits){
//   int power = pow(2,significantBits-1);
//   int sum = 0;
//   int i;
//
//   for (i=0; i<significantBits; ++i)
//   {
//       if ( i==0 && binary[i]!='0')
//       {
//           sum = power * -1;
//       }
//       else
//       {
//           sum += (binary[i]-'0')*power;//The -0 is needed
//       }
//       power /= 2;
//   }
//
//   if(binary[0] == '0') {
//     if(sum >= 0) {
//       return sum;
//     } else {
//       return -1 * sum;
//     }
//   } else {
//     if(sum >= 0) {
//       return -1 * sum;
//     } else {
//       return sum;
//     }
//   }
// }
//
// // Convert the 32-bit binary encoding into hexadecimal
// int Binary2Hex( std::string Binary )
// {
//     std::bitset<32> set(Binary);
//     int hex = set.to_ulong();
//
//     return hex;
// }
//
// // Convert the 32-bit binary into the decimal
// float GetFloat32( std::string Binary )
// {
//     int HexNumber = Binary2Hex( Binary );
//
//     bool negative  = !!(HexNumber & 0x80000000);
//     int  exponent  =   (HexNumber & 0x7f800000) >> 23;
//     int sign = negative ? -1 : 1;
//
//     // Subtract 127 from the exponent
//     exponent -= 127;
//
//     // Convert the mantissa into decimal using the
//     // last 23 bits
//     int power = -1;
//     float total = 0.0;
//     for ( int i = 0; i < 23; i++ )
//     {
//         int c = Binary[ i + 9 ] - '0';
//         total += (float) c * (float) pow( 2.0, power );
//         power--;
//     }
//     total += 1.0;
//
//     float value = sign * (float) pow( 2.0, exponent ) * total;
//
//     return value;
// }
//
// // Get 32-bit IEEE 754 format of the decimal value
// std::string GetBinary32( float value )
// {
//     union
//     {
//          float input;   // assumes sizeof(float) == sizeof(int)
//          int   output;
//     }    data;
//
//     data.input = value;
//
//     std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);
//
//     std::string mystring = bits.to_string<char,
//                                           std::char_traits<char>,
//                                           std::allocator<char> >();
//
//     return mystring;
// }
