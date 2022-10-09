#include "Matrix.cpp"
int main(int argc, char *argv[])
{
    auto &os = std::cout;
    const Matrix a{{1, 2}, {4, 5}}, b{{100, 10}, {10, 100}, {1, 1000}};
    // os << a * a.transpose() << '\n';
    // os << a.transpose() * a;
    os << a.getInverseMatrix() * a;
    return 0;
}