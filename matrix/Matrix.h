#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <string>
#define Mod 10;
// 矩阵
class Matrix : private std::vector<std::vector<long double>>
{
public:                                         //类型别名
    using element_type = long double;           // 矩阵元素类型
    using row_type = std::vector<element_type>; // 行/列矩阵
    using super_type = std::vector<row_type>;
    using super_type::const_iterator;
    using super_type::const_pointer;
    using super_type::const_reference;
    using super_type::const_reverse_iterator;
    using super_type::iterator;
    using super_type::pointer;
    using super_type::reference;
    using super_type::reverse_iterator;
    // 友元
    // 显示矩阵到标准输出
    friend std::ostream &operator<<(std::ostream &, const Matrix &);
    // 数乘(数前置)
    friend inline Matrix operator*(const element_type &, const Matrix &);
    friend Matrix UnitMatrix(const size_t &n);
    // 可以使用指定范围内的数以指定的行高、列宽随机生成一个矩阵
    friend Matrix AssignValuesRandomly(const size_t &r, const size_t &c, const element_type &inf, const element_type &sup);

public: //构造与析构
        // 默认构造
    Matrix(void);
    // 拷贝构造
    // Matrix(const Matrix &);
    Matrix(const std::initializer_list<Matrix::row_type> &);
    Matrix(Matrix::const_iterator &, Matrix::const_iterator &);
    /*~Matrix(); */

private:
    // 检验矩阵的合法性/存在性
    bool IsLegitimate(void) const;

public: // 获取矩阵的行数 注意在使用之前请检查矩阵的合法性
    inline size_t GetRows(void) const;
    // 获取矩阵的列数 注意在使用之前请检查矩阵的合法性
    inline size_t GetColumn(void) const;
    using super_type::operator[];

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
    Matrix operator*(const element_type &) const;
    // 数乘(复合运算) 注意在使用之前请检查矩阵的合法性、参数的非零性
    Matrix &operator*=(const element_type &);
    // 减法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    inline Matrix operator-(const Matrix &) const;
    // 减法(复合运算) 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix &operator-=(const Matrix &);
    //在矩阵右侧拼接另一个行数相同的矩阵
    Matrix &operator&=(const Matrix &);
    // 比较两个矩阵是否相同
    bool operator==(const Matrix &) const;
    // 幂运算
    // Matrix operator^(const element_type &) const;

    // 向布尔类型转换(显示地) 效果等同于IsLegitimate
    // 当*this为非空矩阵时返回true,否则返回false
    explicit operator bool() const { return IsLegitimate(); };

public: //
    // 将矩阵的内容储存到一个串里
    std::string toString() const;
    /* 行交换 注意矩阵的合法性*/
    Matrix LineExchange(const size_t &i, const size_t &j) const;
    // 行交换 引用版本
    Matrix &RefLineExchange(const size_t &i, const size_t &j);
    /* 初等变换——某行倍增 注意矩阵的合法性*/
    Matrix LineMul(const size_t &i, const element_type &k) const;
    // 初等变换——某行倍增 引用版本
    Matrix &RefLineMul(const size_t &i, const element_type &k);
    /* 初等变换——某行倍增所得加到某行上去 注意矩阵的合法性*/
    Matrix LineMulToLine(const size_t &i, const element_type &k, const size_t &j) const;
    // 初等变换——某行倍增所得加到某行上去 引用版本
    Matrix &RefLineMulToLine(const size_t &i, const element_type &k, const size_t &j);
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

public:
    // 创建n阶单位矩阵
    static Matrix UnitMatrix(const size_t &n);
    static Matrix AssignValuesRandomly(const size_t &r, const size_t &c, const element_type &inf, const element_type &sup);
};
#endif