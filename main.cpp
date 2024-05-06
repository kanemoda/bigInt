#include <iostream>
#include "bigInt.h"
using namespace ::std;

int main(int argc, char const *argv[])
{
    bigInt a("123456789");
    bigInt b("987654321");
    bigInt modulus("1000000007");

    // Exponentiation: a^3
    a.exponentiation(3);
    cout << "a^3 = ";
    a.print(); // Output: a^3 = 1881676371789154860897069
}
