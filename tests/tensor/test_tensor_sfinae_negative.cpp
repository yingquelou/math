// Negative test for SFINAE: this file MUST NOT compile.
// It verifies that Tensor<NoNum,2> with operator+ fails, confirming SFINAE
// removes it.
#include "tensor.hpp"
struct NoNum {};
int main() {
  tensor::Tensor<NoNum, 2> a(2, 2), b(2, 2);
  auto c = a + b; // should NOT compile
  (void)c;
  return 0;
}
