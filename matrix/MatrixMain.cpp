#include "Matrix.cpp"
int main(int argc, char *argv[])
{
    auto &os = std::cout;
    os << Matrix::AssignValuesRandomly(3, 6).rowSimplestForm();
    return 0;
}