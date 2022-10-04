#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <string>
// 矩阵
class Matrix : private std::vector<std::vector<long double>>
{
public:                                         // 类型别名
    using element_type = long double;           // 矩阵元素类型
    using row_type = std::vector<element_type>; // 行/列矩阵
    using super_type = std::vector<row_type>;   // 超类/父类
    using super_type::const_iterator;
    using super_type::const_pointer;
    using super_type::const_reference;
    using super_type::const_reverse_iterator;
    using super_type::iterator;
    using super_type::pointer;
    using super_type::reference;
    using super_type::reverse_iterator;

public: //构造与析构
        // 默认构造
    using super_type::super_type; //使用父类的构造

    // 友元
    // 输出矩阵内容
    friend std::ostream &operator<<(std::ostream &, const Matrix &);
    // 数乘(数前置)
    friend inline Matrix operator*(const element_type &, const Matrix &);

public:
    /**
     * \brief 获取当前矩阵的行数
     * \return 矩阵的行数
     * \date by yingquelou at 2022-10-04 18:12:02
     */
    size_t GetRow() const { return size(); }
    /**
     * \brief 获取当前矩阵的列数
     * \return 矩阵的列数
     * \date by yingquelou at 2022-10-04 18:35:29
     */
    inline size_t GetColumn() const;

public: // 重载运算符 定义矩阵的某些运算
    // 加法 注意在使用之前请检查矩阵的合法性,以及两矩阵是否是同型矩阵
    Matrix operator+(const Matrix &) const;
    using super_type::operator=; // 使用父类的运算符
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
    /**
     * \brief 判断两个矩阵是否是同型矩阵
     * \b 注意:如果要判断矩阵的内容是否相等,请使用"!="运算符
     * \return 如果是同型矩阵返回true,否则返回false
     * \date by yingquelou at 2022-10-04 19:28:24
     */
    bool operator==(const Matrix &) const;
    /**
     * \brief 判断矩阵的内容是否相等
     * \return  不相等返回true,相等返回false
     * \date by yingquelou at 2022-10-04 19:49:50
     */
    bool operator!=(const Matrix &) const;

    using super_type::operator[]; // 使用父类的运算符

    // 幂运算
    // Matrix operator^(const element_type &) const;
    /**
     * \brief 转换函数 当前矩阵非空且有效时为true,否则false
     * 所谓有效是指满足矩阵的数学定义
     * \date by yingquelou at 2022-10-04 17:18:21
     */
    explicit operator bool() const;

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
    // 可以使用指定范围内的数以指定的行高、列宽随机生成一个矩阵
    static Matrix AssignValuesRandomly(const size_t &r, const size_t &c, const element_type &inf, const element_type &sup);
};
#endif