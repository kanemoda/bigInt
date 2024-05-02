#pragma once

#include <vector>
#include <string>
using namespace::std;

class bigInt
{
private:
    vector<int> number;
public:
    void print();
    bigInt(string num);
    void add(bigInt num);
    void subtract(bigInt num);
    void multiply(bigInt num);
};
