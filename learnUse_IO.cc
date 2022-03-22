#include <iostream>
#include <iomanip>
#include <cmath>
int main(void)
{
    // int
    std::cout << std::oct << std::showbase << 15 << std::endl;
    std::cout << std::dec << 15 << std::endl;
    std::cout << std::hex << std::uppercase << 15 << std::endl
              << std::noshowbase << std::nouppercase << std::dec;
    double PI = 3.1415926;
    std::cout << PI << std::endl;
    std::cout << std::setprecision(7) << PI << std::endl;
    std::cout << std::setprecision(8) << PI << std::endl;
    std::cout << std::setw(15) << PI << std::endl;
    std::cout << std::left << PI << std::endl;

    return 0;
}