#ifndef _Matrix_
#define _Matrix_ 1
#include <iostream>
#include <iomanip>
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
// 矩阵类
class Matrix : public Mat2
{
    // 友元
    // 显示矩阵
    friend ostream &operator<<(ostream &, const Matrix &);
    // 数乘(数前置)
    friend inline Matrix operator*(const LDouble &, const Matrix &);

public: //构造与析构
        // 默认构造
    // Matrix() = default;
    // 拷贝构造
    // Matrix(const Matrix &);
    // Matrix(...);
    /*~Matrix(); */

public:
    // 检验矩阵的合法性/存在性
    bool IsLegitimate(void) const;
    // 获取矩阵的行数 注意在使用之前请检查矩阵的合法性
    inline size_t GetRows(void) const;
    // 获取矩阵的列数 注意在使用之前请检查矩阵的合法性
    inline size_t GetColumn(void) const;

public: //定义矩阵的某些运算/重载运算符
    // 拷贝赋值
    Matrix &operator=(const Matrix &);
    // 加法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix operator+(const Matrix &) const;
    // 加法(复合运算) 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix &operator+=(const Matrix &);
    // 乘法 注意在使用之前请检查矩阵的合法性,以及两矩阵的可乘性
    Matrix operator*(const Matrix &) const;
    // 乘法 注意在使用之前请检查矩阵的合法性,以及两矩阵的可乘性
    Matrix &operator*=(const Matrix &);
    // 数乘(复合运算) 注意在使用之前请检查矩阵的合法性、参数的非零性
    Matrix operator*(const LDouble &) const;
    // 数乘(复合运算) 注意在使用之前请检查矩阵的合法性、参数的非零性
    Matrix &operator*=(const LDouble &);
    // 减法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    inline Matrix operator-(const Matrix &) const;
    // 减法(复合运算) 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix &operator-=(const Matrix &);
    // 幂运算
    // Matrix operator^(const MyType &) const;

public: // 初等变换
    /* 行交换 注意矩阵的合法性*/
    Matrix LineExchange(const size_t &i, const size_t &j) const;
    /* 某行倍增 注意矩阵的合法性*/
    Matrix LineMul(const size_t &i, const LDouble &k) const;
    /* 某行倍增所得加到某行上去 注意矩阵的合法性*/
    Matrix LineMulToLine(const size_t &i, const LDouble &k, const size_t &j) const;
    // 求可左乘当前矩阵的单位矩阵 注意矩阵的合法性
    inline Matrix LeftMulUnitMatrix(void) const;
    // 求可右乘当前矩阵的单位矩阵 注意矩阵的合法性
    inline Matrix RightMulUnitMatrix(void) const;

public:
    //求行阶梯形矩阵
    // Matrix RowEchelonMatrix(void) const;
    // 求逆

    // 转置 注意矩阵的合法性
    Matrix TransposeMatrix(void) const;
    //
};
// 创建n阶单位矩阵
Matrix UnitMatrix(const size_t &);
#endif