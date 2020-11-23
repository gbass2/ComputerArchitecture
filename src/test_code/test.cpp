#include <iostream>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <string>

using namespace std;

string CONVERT_TYPE(long double input){
    std::ostringstream ss;

    ss << input;
    std::string numberIn = ss.str();

    return numberIn;
}

string Decimal2Binary(double input){
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


// Working
int main(){
    uint8_t num =  0b11001000;
    bitset<8> numbit;
    cout << "num in decimal: " << num/2 << endl;

    numbit = num;
    cout << "num in binary: " << numbit << endl;
    return 0;
}
