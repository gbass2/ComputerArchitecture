#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <bitset>
#include <math.h>
#include <algorithm>
using namespace std;

void simulationOne() { // 256 byte cache, 32 byte block, direct mapped
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 32;                   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 3
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 1; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;

    // Cache data structure initialization
    string cache[2][numOfSets];
    // Index bits
    cache[0][0] = "000";
    cache[0][1] = "001";
    cache[0][2] = "010";
    cache[0][3] = "011";
    cache[0][4] = "100";
    cache[0][5] = "101";
    cache[0][6] = "110";
    cache[0][7] = "111";
    // Simulation One
    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0,4);
        string index = addy.substr(4,3);
        string offset = addy.substr(7,5);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // find the correct column for the index
                break;
            }
        }

        // *********************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the input
        // *********************************************************************

        if (cache[1][indexVal] == tag) {    // cache hit
            currTick += 5;
            hitRate += 1;
        }
        else {                              // cache miss
            cache[1][indexVal] = tag;
            currTick += costOfCacheMiss;
            missRate += 1;
        }
        ss.clear();     // clear string stream
    }

    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 1" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

void simulationTwo() { // 512 byte cache, 32 byte block, direct mapped
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 512 / 32;                   // 16
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 4
    int tagBits = 12 - offsetBits - indexBits;  // 3
    int lookupTime = ceil(log2(numOfSets)) * 1; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;

    // Cache data structure initialization
    string cache[2][numOfSets];
    // Index bits
    cache[0][0] = "0000";
    cache[0][1] = "0001";
    cache[0][2] = "0010";
    cache[0][3] = "0011";
    cache[0][4] = "0100";
    cache[0][5] = "0101";
    cache[0][6] = "0110";
    cache[0][7] = "0111";
    cache[0][8] = "1000";
    cache[0][9] = "1001";
    cache[0][10] = "1010";
    cache[0][11] = "1011";
    cache[0][12] = "1100";
    cache[0][13] = "1101";
    cache[0][14] = "1110";
    cache[0][15] = "1111";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0,3);
        string index = addy.substr(3,4);
        string offset = addy.substr(7,5);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // find the correct column for the index
                break;
            }
        }

        // *********************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the input
        // *********************************************************************

        if (cache[1][indexVal] == tag) {    // cache hit
            currTick += 5;
            hitRate += 1;
        }
        else {                              // cache miss
            cache[1][indexVal] = tag;
            currTick += costOfCacheMiss;
            missRate += 1;
        }
        ss.clear();     // clear string stream
    }

    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 2" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

void simulationThree() { // 256 byte cache, 64 byte block, direct mapped
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 64;                   // 4
    int offsetBits = log2(64);                  // 6
    int indexBits = log2(numOfSets);            // 2
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 1; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;

    // Cache data structure initialization
    string cache[2][numOfSets];
    // Index bits
    cache[0][0] = "00";
    cache[0][1] = "01";
    cache[0][2] = "10";
    cache[0][3] = "11";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0,4);
        string index = addy.substr(4,2);
        string offset = addy.substr(6,6);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // find the correct column for the index
                break;
            }
        }

        // *********************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the input
        // *********************************************************************

        if (cache[1][indexVal] == tag) {    // cache hit
            currTick += 5;
            hitRate += 1;
        }
        else {                              // cache miss
            cache[1][indexVal] = tag;
            currTick += costOfCacheMiss;
            missRate += 1;
        }
        ss.clear();     // clear string stream
    }

    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 3" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

void simulationFour() { // 256 byte cache, 32 byte block, 4-way
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 32;   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 3
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;
    int block3Tick = 3;
    int block4Tick = 4;

    // Cache data structure intialization
    string cache[5][numOfSets];    // 5x8 array
    // Index bits
    cache[0][0] = "000";
    cache[0][1] = "001";
    cache[0][2] = "010";
    cache[0][3] = "011";
    cache[0][4] = "100";
    cache[0][5] = "101";
    cache[0][6] = "110";
    cache[0][7] = "111";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 4);
        string index = addy.substr(4,3);
        string offset = addy.substr(7,5);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // finds the correct column for the set
                break;
            }
        }

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[1][indexVal] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[2][indexVal] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[3][indexVal] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[4][indexVal] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[1][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[2][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block3Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[3][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block3Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[4][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: 4-WAY TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 4" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

void simulationFive() { // 256 byte cache, 64 byte block, 4-way
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 64;                   // 4
    int offsetBits = log2(64);                  // 6
    int indexBits = log2(numOfSets);            // 2
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;
    int block3Tick = 3;
    int block4Tick = 4;

    // Cache data structure intialization
    string cache[5][numOfSets];    // 5x4 array
    // Index bits
    cache[0][0] = "00";
    cache[0][1] = "01";
    cache[0][2] = "10";
    cache[0][3] = "11";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 4);
        string index = addy.substr(4,2);
        string offset = addy.substr(6,6);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // finds the correct column for the set
                break;
            }
        }

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[1][indexVal] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[2][indexVal] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[3][indexVal] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[4][indexVal] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[1][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[2][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block3Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[3][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block3Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[4][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: 4-WAY TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation (256B Overall Size | 64B Block Size | 4-Way)" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

// NOT OUTPUTTED
void simulationSix() { // 256 byte cache, 64 byte block, Full
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 64;                   // 4
    int offsetBits = log2(64);                  // 6
    int indexBits = 0;            // 0
    int tagBits = 12 - offsetBits - indexBits;  // 6
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;
    int block3Tick = 3;
    int block4Tick = 4;

    // Cache data structure intialization
    string cache[1][numOfSets];    // 1x4 array

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 6);
        string offset = addy.substr(6,6);

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[0][0] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][1] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][2] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][3] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][0] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][1] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block3Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][2] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block3Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][3] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: FULL ASSOCIATIVE TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 6 (256B 64B Fully)" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

// NOT OUTPUTTED
void simulationSeven() { // 256 byte cache, 32 byte block, Full
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 32;                   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = 0;            // 0
    int tagBits = 12 - offsetBits - indexBits;  // 7
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;
    int block3Tick = 3;
    int block4Tick = 4;
    int block5Tick = 5;
    int block6Tick = 6;
    int block7Tick = 7;
    int block8Tick = 8;

    // Cache data structure intialization
    string cache[1][numOfSets];    // 1x4 array

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 7);
        string offset = addy.substr(7,5);

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[0][0] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][1] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][2] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][3] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][4] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][5] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][6] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[0][7] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][0] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][1] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block3Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][2] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block3Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][3] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][4] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][5] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][6] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[0][7] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: FULL ASSOCIATIVE TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 7 (256B 32B Fully)" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

// NOT OUTPUTTED
void simulationEight() { // 256 byte cache, 32 byte block, 4-way
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 32;                   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 3
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;
    int block3Tick = 3;
    int block4Tick = 4;

    // Cache data structure intialization
    string cache[5][numOfSets];    // 5x8 array
    // Index bits
    cache[0][0] = "000";
    cache[0][1] = "001";
    cache[0][2] = "010";
    cache[0][3] = "011";
    cache[0][4] = "100";
    cache[0][5] = "101";
    cache[0][6] = "110";
    cache[0][7] = "111";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 4);
        string index = addy.substr(4,3);
        string offset = addy.substr(7,5);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // finds the correct column for the set
                break;
            }
        }

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[1][indexVal] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[2][indexVal] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[3][indexVal] == tag) {   // cache hit on block 3
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[4][indexVal] == tag) {   // cache hit on block 4
            currTick += 5;
            hitRate += 1;
        }
        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[1][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[2][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block3Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[3][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block3Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block4Tick == min({block1Tick, block2Tick, block3Tick, block4Tick})){
                cache[4][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block4Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: 4-WAY TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 8 (256B 32B 4-Way)" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

// NOT OUTPUTTED
void simulationNine() { // 256 byte cache, 32 byte block, 2-way
    // Text File Initializations
    stringstream ss;
    ifstream inFile;
    inFile.open("addresses.txt");
    string line, addy;
    // Cache parameter declarations
    int numOfSets = 256 / 32;   // 8
    int offsetBits = log2(32);                  // 5
    int indexBits = log2(numOfSets);            // 3
    int tagBits = 12 - offsetBits - indexBits;  // 4
    int lookupTime = ceil(log2(numOfSets)) * 4; // 1 way associativity
    int costOfCacheMiss = lookupTime + 100;
    int i, indexVal;
    int currTick = 0;
    unsigned x;
    double hitRate, missRate = 0;
    // For LRU eviction
    // Values are for ordered block eviction
    int block1Tick = 1;
    int block2Tick = 2;

    // Cache data structure intialization
    string cache[3][numOfSets];    // 5x8 array
    // Index bits
    cache[0][0] = "000";
    cache[0][1] = "001";
    cache[0][2] = "010";
    cache[0][3] = "011";
    cache[0][4] = "100";
    cache[0][5] = "101";
    cache[0][6] = "110";
    cache[0][7] = "111";

    // Tag Bits
    while(getline(inFile, line)) {
        ss << hex << line;              // stores the hex value of line in ss
        ss >> x;                        // converts to decimal
        bitset<12> b(x);                // converts to binary
        addy = b.to_string();           // converts to string

        string tag = addy.substr(0, 4);
        string index = addy.substr(4,3);
        string offset = addy.substr(7,5);

        // Find index of input
        for (i = 0; i < numOfSets; i++){
            if (cache[0][i] == index) {
                indexVal = i;       // finds the correct column for the set
                break;
            }
        }

        // *************************************************************************
        // Checks to see if cache tag is empty
        // Determines cache hit/miss
        // If miss, it fills the tag with the new value
        // *************************************************************************

        if (cache[1][indexVal] == tag) {        // cache hit on block 1
            currTick += 5;
            hitRate += 1;
        }
        else if (cache[2][indexVal] == tag) {   // cache hit on block 2
            currTick += 5;
            hitRate += 1;
        }

        else {                                  // cache miss
            // Find the oldest cache block
            if (block1Tick == min({block1Tick, block2Tick})){
                cache[1][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block1Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else if (block2Tick == min({block1Tick, block2Tick})){
                cache[2][indexVal] = tag;
                // update tick values
                currTick += costOfCacheMiss;
                block2Tick = currTick;
                // calculate missRate
                missRate += 1;
            }
            else {
                cout << "ERROR: 2-WAY TAG MISMATCH" << endl;
            }

        }
        ss.clear();     // clear string stream
    }
    hitRate = hitRate / (hitRate + missRate);
    missRate = 1 - hitRate;
    // converts to percentage
    hitRate *= 100;
    missRate *= 100;
    // output
    cout << "# Cache Simulation 9" << endl;
    cout << "Hit rate: " << hitRate << "%" << endl;
    cout << "Miss rate: " << missRate << "%" << endl;
    cout << "Total Trace Simulation Time: " << currTick << " Ticks" << endl << endl;
}

int main() {
    // Deliverable 1
    simulationOne();
    simulationTwo();
    simulationThree();
    simulationFour();

    // Deliverable 2
    // See write-up

    // Deliverable 3
    simulationFive();

    return 0;
}
