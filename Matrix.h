#include <iostream>
#include <vector>
using namespace std;
#define Mod 10;
using std::vector;
using MyType = double;
using LDouble = long double;
// 行/列矩阵
using Mat1 = vector<MyType>;
// 二维矩阵
using Mat2 = vector<Mat1>;
class Matrix : public Mat2
{
    // 友元
    // 显示矩阵
    friend ostream &operator<<(ostream &, const Matrix &);
    // 数乘(数前置)
    friend Matrix operator*(const LDouble &, const Matrix &);

public: //构造与析构
        // 默认构造
    // Matrix() = default;
    // 拷贝构造
    // Matrix(const Matrix &);
    // Matrix(...);
    /*~Matrix(); */

public:
    // 矩阵的合法性检验
    bool IsLegitimate(void) const;
    // 获取矩阵的行数
    size_t GetRows(void) const;
    // 获取矩阵的列数
    size_t GetColumn(void) const;

public: //定义矩阵的某些运算
    // 加法(同型矩阵)
    Matrix operator+(const Matrix &) const;
    // 乘法
    Matrix operator*(const Matrix &) const;
    // 数乘(数后置)
    Matrix operator*(const LDouble &) const;
    // 幂运算
    // Matrix operator^(const MyType &) const;

public: // 初等变换
    /* 行交换 */
    Matrix LineExchange(const size_t i, const size_t j) const;
    /* 某行倍增 */
    Matrix LineMul(const size_t i, const LDouble k) const;
    /* 某行倍增所得加到某行上去 */
    Matrix LineMulToLine(const size_t i, const LDouble k, const size_t j) const;
    // 求可左乘当前矩阵的单位矩阵
    Matrix LeftMulUnitMatrix(void) const;
    // 求可右乘当前矩阵的单位矩阵
    Matrix RightMulUnitMatrix(void) const;

public:
    //求行阶梯形矩阵
    // Matrix RowEchelonMatrix(void) const;
    // 求逆
    // 转置
    //
};
using pMatrix = Matrix *;