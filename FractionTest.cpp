#include <iostream>
#include "Fraction.h"
auto &os = std::cout;
int main(int argc, char *argv[])
{
    Fraction<double> f1(1, 2),f2(-6,-4);
    os<<f2;
    return 0;
}