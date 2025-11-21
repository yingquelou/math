#include "array.hpp"
#include "matrix.hpp"
#include <iostream>
int main(int argc, char const *argv[], char const *env[])
{
    // 测试matrix类
    linear::matrix<double> matA{{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
    linear::matrix<double> matB{{-24, 18, 5}, {20, -15, -4}, {-5, 4, 1}};
    std::cout << "Matrix A:\n";
    std::cout << matA.toString() << std::endl;
    std::cout << "Matrix B:\n"; 
    std::cout << matB.toString() << std::endl;
    auto matC = matA * matB;
    std::cout << "Matrix C = A * B:\n";
    std::cout << matC.toString() << std::endl;
    auto matD = matA.transpose();
    std::cout << "Matrix D = A^T:\n";
    std::cout << matD.toString() << std::endl;
    std::cout << matD.inverse()*matD << std::endl;
    linear::matrix<double> c{ {1, 2}, {2, 5}, {3, 7} };
    std::cout << "Matrix c:\n" << c.rowEchelonForm() << std::endl;
    
    return 0;
}