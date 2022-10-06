#include "Matrix.cpp"
int main(int argc, char *argv[])
{
    auto &os = std::cout;
    // Matrix A={{0, 0}, {1, 1}, {0, 0}, {0.5, 0}}, B(A);

    Matrix m = Matrix::AssignValuesRandomly(3, 5).rowEchelonMatrix();
    os << m << '\n'
       << m.RankOfMatrix();
    return 0;
}