#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <string>

using namespace std;

// Working
int main(){
    // So we place the 8 bit portion of the instruction in memory
    uint8_t num =  0b11001000;

    // Convert it back to binary using bitset
    bitset<8> numbit;
    cout << "num in decimal: " << int(num) << endl;
    numbit = num;
    cout << "num in binary: " << numbit << endl;

    cout << "num in decimal: " << numbit.to_ulong() << endl;

    // Then we can spilt it up as normal for the deocde/execute stages
    return 0;
}
