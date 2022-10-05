#include "Matrix.cpp"
int main(int argc, char *argv[])
{
    auto &os = std::cout;
    Matrix A{{1, 1}, {1, 0}}, B(A);

    os << A.lineMulToLine(0, 0, 1) << '\n';
    return 0;
}