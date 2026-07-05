#define _USE_MATH_DEFINES
#include <cmath>
#include <iomanip>
#include <iostream>
int main(void) {
  // int
  std::cout << std::oct << std::showbase << 15 << std::endl;
  std::cout << std::dec << 15 << std::endl;
  std::cout << std::hex << std::uppercase << 15 << std::endl
            << std::noshowbase << std::nouppercase << std::dec;
  std::cout << M_PI << std::endl;
  std::cout << std::setprecision(7) << M_PI << std::endl;
  std::cout << std::setprecision(8) << M_PI << std::endl;
  std::cout << std::setw(15) << M_PI << std::endl;
  std::cout << std::left << M_PI << std::endl;

  return 0;
}