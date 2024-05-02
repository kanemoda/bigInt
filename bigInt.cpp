#include "bigInt.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fftw3.h>
#include <complex>

using namespace::std;

//Perform FFT
vector<complex<double>> bigInt::fft(const vector<complex<double>>& a){
    int n = a.size();
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_plan p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    
    for (int i = 0; i < n; ++i) {
        in[i][0] = a[i].real();
        in[i][1] = a[i].imag();
    }
    
    fftw_execute(p);
    
    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = complex<double>(out[i][0], out[i][1]);
    }
    
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    
    return result;
}   

vector<complex<double>> bigInt::ifft(const vector<complex<double>>& a){
    int n = a.size();
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_plan p = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    
    for (int i = 0; i < n; ++i) {
        in[i][0] = a[i].real();
        in[i][1] = a[i].imag();
    }
    
    fftw_execute(p);
    
    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = complex<double>(out[i][0] / n, out[i][1] / n);
    }
    
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    
    return result;
}







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

void bigInt::multiply(bigInt num) {
    int n = 1;
    while (n < 2 * max(number.size(), num.number.size())) {
        n <<= 1;
    }

    // Zero-pad input arrays
    vector<complex<double>> fa(n), fb(n);
    for (int i = 0; i < number.size(); ++i) {
        fa[i] = complex<double>(number[i], 0);
    }
    for (int i = 0; i < num.number.size(); ++i) {
        fb[i] = complex<double>(num.number[i], 0);
    }

    // Perform FFT
    fa = fft(fa);
    fb = fft(fb);

    // Multiply FFT results element-wise
    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = fa[i] * fb[i];
    }

    // Perform inverse FFT
    result = ifft(result);

    // Extract real parts (rounding to integers)
    vector<int> res(n);
    for (int i = 0; i < n; ++i) {
        res[i] = round(result[i].real());
    }

    // Handle carries
    int carry = 0;
    for (int i = n - 1; i >= 0; --i) {
        res[i] += carry;
        carry = res[i] / 10;
        res[i] %= 10;
    }

    // Remove leading zeros
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }

    number = res;
}
