#include <iostream>
#include "bigInt.h"
using namespace::std;

int main(int argc, char const *argv[])
{
    bigInt num = bigInt("99");
    bigInt num2 = bigInt("99");
    num.print();
    num.add(num2);
    num.print();
    num.subtract(num2);
    num.print();
    return 0;
}
