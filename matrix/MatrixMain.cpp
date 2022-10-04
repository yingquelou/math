#include "Matrix.h"
int main(int argc, char *argv[])
{
     auto &os = std::cout;
     Matrix A{{1, 1}, {1, 1}}, B{{0, 1, 1}, {1, 0, -1}, {1, 1, 0}},
         C(Matrix::AssignValuesRandomly(2, 2, -10, 10)), D(C);

     os << (C != D) << '\n';
     return 0;
}