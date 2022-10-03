#include "Matrix.cpp"
int main(void)
{
     Matrix B{{0, 1, 1},
              {1, 0, -1},
              {1, 1, 0}};
     auto &os = std::cout;
     os << B.RowSimplestFormOfMatrix();
     return 0;
}