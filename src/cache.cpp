#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <bitset>
#include <math.h>
using namespace std;

class sim1 {
    public:
        // creating cache set | valid | tag
        unordered_map <int, bitset<3>> cache;
};

void simulationOne(string addy) { // 256 byte cache, 32 byte block, direct mapped

    int numOfSets = 256 / 32;   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 3
    int tagBits = 12 - offsetBits - indexBits;  // 4

    string tag = addy.substr(0, 4);
    cout << "tag: " << tag << endl;
    string index = addy.substr(4,3);
    cout << "index: " << index << endl;
    string offset = addy.substr(7,5);
    cout << "offset: " << offset << endl;

    sim1 data;


    // initialize index bits (3 bits so we go from 0-7)
    /* There is a better way todo this! I'll implement it if we have time */

    data.cache[000] = 0;
    data.cache[001] = 0;
    data.cache[010] = 0;
    data.cache[011] = 0;
    data.cache[100] = 0;
    data.cache[101] = 0;
    data.cache[110] = 0;
    data.cache[111] = 0;

    cout << "numOfSets: " << numOfSets << endl;

    for(int i = 0; i < numOfSets; i++) {
        cout << "i: " << i << endl;
        data.cache[1][i] = "0";
    }



    // match set with tag
    //bool match = false;
    int indexVal;
    for (int i = 0; i < numOfSets; i++){
        if (data.cache[i] == stoi(index)) {
            //match = true;
            indexVal = i;
            cout << "MATCH AT: " << indexVal << endl;
            break;
        }
    }

    //if (data.cache[1][indexVal] == "0")



}

int main() {

    string address = "0x0000049f"; // 10011111
    stringstream ss;
    ss << hex << address;
    unsigned x;
    ss >> x;
    bitset<12> b(x);
    cout << b.to_string() << endl; // 10011111
    ss.clear();

    ifstream inFile;
    inFile.open("addresses.txt");
    string line;
    string binaryAddress;

    while(getline(inFile, line)) {
        cout << "line: " << line << endl;// debug

        ss << hex << line;               // stores the hex value of line in ss
        ss >> x;                         // converts to decimal
        cout << "decimal: " << x << endl;
        bitset<12> b(x);                  // converts to binary
        binaryAddress = b.to_string();
        cout << "binary: " << binaryAddress << endl;   // ouputs binary of line

        simulationOne(binaryAddress);



        cout << endl;
        ss.clear();
    }
    return 0;
}
