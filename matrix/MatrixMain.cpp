#include "Matrix.cpp"
#include <fstream>
int main(int argc, char *argv[])
{
    auto &os = std::cout;
    const Matrix a{{1, 2}, {4, 5}}, b{{100, 10}, {10, 100}, {1, 1000}};
    // os << a * a.transpose() << '\n';
    // os << a.transpose() * a;
    std::ofstream("latex.md") << Matrix::AssignValuesRandomly(50, 5).toLaTeX();
    return 0;
}