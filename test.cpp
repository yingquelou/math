#include "Matrix.h"
int main(void)
{
    Matrix A;
    A.push_back({9, 0});
    A.push_back({1, 4});
    Matrix B;
    B.push_back({2, 0});
    B.push_back({1, 3});
    std::cout << A * B << endl
              << A;
    return 0;
}