#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// Working
int main(){
    string test = "0101";
    reverse(test.begin(), test.end());
    cout << "test: " << test << endl;
    return 0;
}
