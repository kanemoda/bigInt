#include <iostream>
#include "bigInt.h"
using namespace::std;

int main(int argc, char const *argv[])
{
    bigInt num = bigInt("99");
    bigInt num2 = bigInt("99");
    num.multiply(num2);
    num.print();
    return 0;
}
