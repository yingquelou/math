#ifndef _Matrix_
#define _Matrix_ 1
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <string>
#include <map>
using namespace std;
#define Mod 10;
using std::vector;
using MyType = double;
// 行/列矩阵
using Value_Type = vector<MyType>;
// 二维矩阵
using Mat2 = vector<Value_Type>;
// 矩阵类
class Matrix : private Mat2
{
    // 友元
    // 显示矩阵到标准输出
    friend ostream &operator<<(ostream &, const Matrix &);
    // 数乘(数前置)
    friend inline Matrix operator*(const MyType &, const Matrix &);
    friend Matrix UnitMatrix(const size_t &n);
    // 将矩阵的内容储存到一个串里
    friend string MatrixToString(const Matrix &mat);
    // 可以使用指定范围内的数以指定的行高、列宽随机生成一个矩阵
    friend Matrix AssignValuesRandomly(const size_t &r = 3, const size_t &c = 3, const MyType &inf = 0, const MyType &sup = 10);

public: //类型别名
    using iterator = Mat2::iterator;
    using const_iterator = Mat2::const_iterator;
    using value_type = vector<MyType>;
    using basic_type = MyType;

public: //构造与析构
        // 默认构造
    Matrix(void);
    // 拷贝构造
    // Matrix(const Matrix &);
    Matrix(const initializer_list<Matrix::value_type> &);
    Matrix(Matrix::const_iterator &, Matrix::const_iterator &);
    /*~Matrix(); */

private:
    // 检验矩阵的合法性/存在性
    bool IsLegitimate(void) const;

public: // 获取矩阵的行数 注意在使用之前请检查矩阵的合法性
    inline size_t GetRows(void) const;
    // 获取矩阵的列数 注意在使用之前请检查矩阵的合法性
    inline size_t GetColumn(void) const;

public: //定义矩阵的某些运算/重载运算符
    // 拷贝赋值 (可自赋值)
    Matrix &operator=(const Matrix);
    // 加法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix operator+(const Matrix &) const;
    // 加法(复合运算) 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix &operator+=(const Matrix &);
    // 乘法 注意在使用之前请检查矩阵的合法性,以及两矩阵的可乘性
    Matrix operator*(const Matrix &) const;
    // 乘法 注意在使用之前请检查矩阵的合法性,以及两矩阵的可乘性
    Matrix &operator*=(const Matrix &);
    // 数乘(复合运算) 注意在使用之前请检查矩阵的合法性、参数的非零性
    Matrix operator*(const MyType &) const;
    // 数乘(复合运算) 注意在使用之前请检查矩阵的合法性、参数的非零性
    Matrix &operator*=(const MyType &);
    // 减法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    inline Matrix operator-(const Matrix &) const;
    // 减法(复合运算) 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix &operator-=(const Matrix &);
    //在矩阵右侧拼接另一个行数相同的矩阵
    Matrix &operator&=(const Matrix &);
    // 比较两个矩阵是否相同
    bool operator==(const Matrix &) const;
    // 幂运算
    // Matrix operator^(const MyType &) const;

    // 向布尔类型转换(显示地) 效果等同于IsLegitimate
    // 当*this为非空矩阵时返回true,否则返回false
    explicit operator bool() const { return IsLegitimate(); };

public: //
    /* 行交换 注意矩阵的合法性*/
    Matrix LineExchange(const size_t &i, const size_t &j) const;
    // 行交换 引用版本
    Matrix &RefLineExchange(const size_t &i, const size_t &j);
    /* 初等变换——某行倍增 注意矩阵的合法性*/
    Matrix LineMul(const size_t &i, const MyType &k) const;
    // 初等变换——某行倍增 引用版本
    Matrix &RefLineMul(const size_t &i, const MyType &k);
    /* 初等变换——某行倍增所得加到某行上去 注意矩阵的合法性*/
    Matrix LineMulToLine(const size_t &i, const MyType &k, const size_t &j) const;
    // 初等变换——某行倍增所得加到某行上去 引用版本
    Matrix &RefLineMulToLine(const size_t &i, const MyType &k, const size_t &j);
    // 求可左乘当前矩阵的单位矩阵 注意矩阵的合法性
    inline Matrix LeftMulUnitMatrix(void) const;
    // 求可右乘当前矩阵的单位矩阵 注意矩阵的合法性
    inline Matrix RightMulUnitMatrix(void) const;

private:
    // true:求行最简形矩阵 false:求行阶梯形矩阵
    // RSFM:RowSimplestFormOfMatrix REM:RowEchelonMatrix
    Matrix &RSFM_REM(bool);

public:
    // 求行最简形矩阵
    Matrix RowSimplestFormOfMatrix(void) const;
    // 求行最简形矩阵 引用版本
    Matrix &RefRowSimplestFormOfMatrix(void);
    // 求行阶梯形矩阵
    Matrix RowEchelonMatrix(void) const;
    // 求行阶梯形矩阵 引用版本
    Matrix &RefRowEchelonMatrix(void);
    // 求逆
    Matrix GetInverseMatrix(void) const;
    // 转置 注意矩阵的合法性
    Matrix TransposeMatrix(void) const;
    // 求矩阵的秩
    size_t RankOfMatrix(void) const;
};
// 浮点型转换为字符串
string LdoubleToString(long double);
// 创建n阶单位矩阵
Matrix UnitMatrix(const size_t &n);
#endif