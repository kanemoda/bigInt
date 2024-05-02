#pragma once

#include <vector>
#include <string>
#include <complex>
using namespace::std;

class bigInt
{
private:
    vector<int> number;
    vector<complex<double>> fft(const vector<complex<double>>& a);
    vector<complex<double>> ifft(const vector<complex<double>>& a);
public:
    void print();
    bigInt(string num);
    void add(bigInt num);
    void subtract(bigInt num);
    void multiply(bigInt num);
};
