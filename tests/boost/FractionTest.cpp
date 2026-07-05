#include "Fraction.hpp"
auto &os = std::cout;

int main(int argc, char *argv[]) {
  Fraction<int> f1(1, 2), f2(-6, -4);
  os << f1 + f2 << '\n';
  os << 1 + f1 + 2 << '\n';
  os << f1 - f2 << '\n';
  os << 1 - f1 - 2 << '\n';
  os << f1 * f2 << '\n';
  os << 1 * f1 * 2 << '\n';
  os << f1 / f2 << '\n';
  os << 1 / f1 / 2 << '\n';
  return 0;
}