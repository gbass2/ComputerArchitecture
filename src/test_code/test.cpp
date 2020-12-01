#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <string>

using namespace std;

// Working
int main(){
    // // So we place the 8 bit portion of the instruction in memory
    // uint8_t num =  0b11001000;
    //
    // // Convert it back to binary using bitset
    // bitset<8> numbit;
    // cout << "num in decimal: " << int(num) << endl;
    // numbit = num;
    // cout << "num in binary: " << numbit << endl;
    //
    // cout << "num in decimal: " << numbit.to_ulong() << endl;
    //
    // // Then we can spilt it up as normal for the deocde/execute stages
    int val = 100000000;
    int e;
    uint8_t* p;
    p = (uint8_t *)(&val);
    e = *(int *)(p);
    cout << "e: " << e << endl;

    // float val = .5;
    // int *j = (int *)(&val); // storage element
    // cout << "j: " << *j << endl;
    // float p = *(float *)(j);
    // cout << "p: " << p << endl;
    return 0;
}
