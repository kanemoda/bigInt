#include "bigInt.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fftw3.h>
#include <complex>

using namespace ::std;

// Perform FFT
vector<complex<double>> bigInt::fft(const vector<complex<double>> &a)
{
    int n = a.size();
    fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_plan p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (int i = 0; i < n; ++i)
    {
        in[i][0] = a[i].real();
        in[i][1] = a[i].imag();
    }

    fftw_execute(p);

    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i)
    {
        result[i] = complex<double>(out[i][0], out[i][1]);
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return result;
}
// Perform IFFT
vector<complex<double>> bigInt::ifft(const vector<complex<double>> &a)
{
    int n = a.size();
    fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
    fftw_plan p = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    for (int i = 0; i < n; ++i)
    {
        in[i][0] = a[i].real();
        in[i][1] = a[i].imag();
    }

    fftw_execute(p);

    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i)
    {
        result[i] = complex<double>(out[i][0] / n, out[i][1] / n);
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return result;
}

bigInt::bigInt(string num)
{
    {
        for (int i = 0; i < num.size(); i++)
        {

            number.push_back(num[i] - '0');
        }
    }
}

void bigInt::print()
{
    for (int i = 0; i < number.size(); i++)
    {
        cout << number[i];
    }
    cout << '\n';
}

void bigInt::add(bigInt num)
{
    vector<int> result;
    int carry = 0;
    int i = number.size() - 1;
    int j = num.number.size() - 1;

    while (i >= 0 || j >= 0 || carry)
    {
        int sum = carry;
        if (i >= 0)
            sum += number[i--];
        if (j >= 0)
            sum += num.number[j--];

        carry = sum / 10;
        result.push_back(sum % 10);
    }
    reverse(result.begin(), result.end());
    number = result;
}

void bigInt::subtract(bigInt num)
{
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

        if (i >= 0)
            i--;
        if (j >= 0)
            j--;
    }
    while (result.size() > 1 && result.back() == 0)
    {
        result.pop_back();
    }
    reverse(result.begin(), result.end());
    number = result;
}

void bigInt::multiply(bigInt num)
{
    int n = 1;
    while (n < 2 * max(number.size(), num.number.size()))
    {
        n <<= 1;
    }

    // Zero-pad input arrays
    vector<complex<double>> fa(n), fb(n);
    for (int i = 0; i < number.size(); ++i)
    {
        fa[i] = complex<double>(number[i], 0);
    }
    for (int i = 0; i < num.number.size(); ++i)
    {
        fb[i] = complex<double>(num.number[i], 0);
    }

    // Perform FFT
    fa = fft(fa);
    fb = fft(fb);

    // Multiply FFT results element-wise
    vector<complex<double>> result(n);
    for (int i = 0; i < n; ++i)
    {
        result[i] = fa[i] * fb[i];
    }

    // Perform inverse FFT
    result = ifft(result);

    // Extract real parts (rounding to integers)
    vector<int> res(n);
    for (int i = 0; i < n; ++i)
    {
        res[i] = round(result[i].real());
    }

    // Handle carries
    int carry = 0;
    for (int i = n - 1; i >= 0; --i)
    {
        res[i] += carry;
        carry = res[i] / 10;
        res[i] %= 10;
    }

    // Remove leading zeros
    while (res.size() > 1 && res.back() == 0)
    {
        res.pop_back();
    }

    number = res;
}

void bigInt::divide(bigInt num)
{
    // Find the sizes of dividend and divisor
    int dividendSize = number.size();
    int divisorSize = num.number.size();

    // Calculate the size needed for FFT (next power of 2)
    int fftSize = 1;
    while (fftSize < 2 * max(dividendSize, divisorSize))
    {
        fftSize <<= 1;
    }

    // Create arrays for complex numbers
    fftw_complex *dividendFFT = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fftSize);
    fftw_complex *divisorFFT = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fftSize);
    fftw_complex *resultFFT = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fftSize);

    // Initialize arrays with zeros
    for (int i = 0; i < fftSize; ++i)
    {
        dividendFFT[i][0] = 0.0;
        dividendFFT[i][1] = 0.0;
        divisorFFT[i][0] = 0.0;
        divisorFFT[i][1] = 0.0;
    }

    // Fill the arrays with data from dividend and divisor
    for (int i = 0; i < dividendSize; ++i)
    {
        dividendFFT[i][0] = number[dividendSize - 1 - i];
    }
    for (int i = 0; i < divisorSize; ++i)
    {
        divisorFFT[i][0] = num.number[divisorSize - 1 - i];
    }

    // Create FFT plans
    fftw_plan dividendPlan = fftw_plan_dft_1d(fftSize, dividendFFT, resultFFT, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan divisorPlan = fftw_plan_dft_1d(fftSize, divisorFFT, resultFFT, FFTW_FORWARD, FFTW_ESTIMATE);

    // Perform FFT on dividend and divisor
    fftw_execute(dividendPlan);
    fftw_execute(divisorPlan);

    // Perform element-wise division
    for (int i = 0; i < fftSize; ++i)
    {
        double realPart = dividendFFT[i][0] * divisorFFT[i][0] - dividendFFT[i][1] * divisorFFT[i][1];
        double imagPart = dividendFFT[i][0] * divisorFFT[i][1] + dividendFFT[i][1] * divisorFFT[i][0];
        resultFFT[i][0] = realPart;
        resultFFT[i][1] = imagPart;
    }

    // Create inverse FFT plan
    fftw_plan inversePlan = fftw_plan_dft_1d(fftSize, resultFFT, dividendFFT, FFTW_BACKWARD, FFTW_ESTIMATE);

    // Perform inverse FFT
    fftw_execute(inversePlan);

    // Extract the real parts and scale the result
    vector<int> quotient;
    for (int i = 0; i < dividendSize; ++i)
    {
        quotient.push_back(round(dividendFFT[i][0] / fftSize));
    }

    // Free memory and destroy plans
    fftw_destroy_plan(dividendPlan);
    fftw_destroy_plan(divisorPlan);
    fftw_destroy_plan(inversePlan);
    fftw_free(dividendFFT);
    fftw_free(divisorFFT);
    fftw_free(resultFFT);

    number = quotient;
}

// Greater than operator
bool bigInt::operator>(const bigInt &num) const
{
    if (number.size() != num.number.size())
        return number.size() > num.number.size();

    for (int i = 0; i < number.size(); ++i)
    {
        if (number[i] != num.number[i])
            return number[i] > num.number[i];
    }

    return false; // Numbers are equal
}

// Less than operator
bool bigInt::operator<(const bigInt &num) const
{
    return num > *this;
}

// Equal to operator
bool bigInt::operator==(const bigInt &num) const
{
    return !(*this < num) && !(*this > num);
}

// Exponentiation operation
void bigInt::exponentiation(int exp)
{
    if (exp == 0)
    {
        number = {1};
        return;
    }

    bigInt result("1");
    for (int i = 0; i < exp; ++i)
    {
        result.multiply(*this);
    }

    number = result.number;
}
