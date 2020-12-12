#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

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

int main(){
    bitset<20> test = 16;

    cout << "shift before: " << test.to_ulong() << endl;
    test = test << 1;

    cout << "shift after: " << test.to_ulong() << endl;

    return 0;
}
