#include "bigInt.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fftw3.h>

using namespace::std;

bigInt::bigInt(string num){
    {
        for(int i = 0 ; i < num.size() ; i++){

            number.push_back(num[i] - '0');
        }
    }
}

void bigInt::print(){
    for(int i = 0 ; i < number.size() ; i++){
            cout << number[i];
        }
        cout << '\n';
}

void bigInt::add(bigInt num){
    vector<int> result;
    int carry = 0;
    int i = number.size() -1;
    int j = num.number.size() -1;

    while(i >= 0 || j >= 0 || carry){
        int sum = carry;
        if(i >= 0) sum += number[i--];
        if(j >= 0) sum += num.number[j--];
        
        carry = sum / 10;
        result.push_back(sum % 10);
    }
    reverse(result.begin() , result.end());
    number = result;
}

void bigInt::subtract(bigInt num){
    vector<int> result;
    int borrow = 0;


    int i = number.size() - 1;
    int j = num.number.size() - 1;
    
    while (i >= 0 || j >= 0)
    {
        int diff = (i >= 0 ? number[i] : 0) - borrow - (j >= 0 ? num.number[j] : 0);
        borrow = (diff < 0) ? 1 : 0;
        if (diff < 0)
        {
            diff += 10;
        }

        result.push_back(diff);

        if(i >= 0) i--;
        if(j >= 0) j--;
        
    }
    while (result.size() > 1 && result.back() == 0){
        result.pop_back();
    }
    reverse(result.begin() , result.end());
    number = result;
}

void bigInt::multiply(bigInt num){
    
}