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
    using super_type::size_type;

public: //构造与析构
        // 默认构造
    using super_type::super_type; //使用父类的构造

    // 友元
    // 输出矩阵内容
    friend std::ostream &operator<<(std::ostream &, const Matrix &);

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
    size_t GetColumn() const;

public: // 重载运算符 定义矩阵的某些运算
    // 使用父类的运算符=
    using super_type::operator=;

    /**
     * \brief 转换函数 判断当前矩阵是否符合数学定义
     * \returns 当前矩阵非空且有效时为true,否则false
     * 所谓有效是指满足矩阵的数学定义
     * \date by yingquelou at 2022-10-04 17:18:21
     */
    explicit operator bool() const;
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

    /**
     * \brief 矩阵的加法运算
     * 当前对象会被改变,加法结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的任一矩阵为空、无效或两矩阵不是同型矩阵时,抛出异常
     * \date by yingquelou at 2022-10-05 15:14:29
     */
    Matrix &operator+=(const Matrix &);
    // 矩阵的加法运算
    Matrix operator+(const Matrix &mat) const { return Matrix(*this) += mat; }

    /**
     * \brief 矩阵的乘法运算
     * 当前对象会被改变,乘法结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的任一矩阵为空、无效或两矩阵不可相乘时,抛出异常
     * \date by yingquelou at 2022-10-05 14:21:46
     */
    Matrix &operator*=(const Matrix &);
    // 矩阵的乘法运算
    Matrix operator*(const Matrix &mat) const { return Matrix(*this) *= mat; }

    /**
     * \brief 矩阵的数乘运算
     * 当前对象会被改变,运算结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的矩阵为空、无效时,抛出异常
     * \date by yingquelou at 2022-10-05 16:24:54
     */
    Matrix &operator*=(const element_type &);
    // 矩阵的数乘运算
    Matrix operator*(const element_type &k) const { return Matrix(*this) *= k; }
    // 矩阵的数乘运算
    friend Matrix operator*(const element_type &k, const Matrix &mat) { return mat * k; }

    // 矩阵的减法运算
    Matrix &operator-=(const Matrix &mat) { return (*this) += mat * -1; }
    // 矩阵的减法运算
    Matrix operator-(const Matrix &mat) const { return (mat * -1) += *this; }

    //在矩阵右侧拼接另一个行数相同的矩阵
    Matrix &operator&=(const Matrix &);
    //在矩阵右侧拼接另一个行数相同的矩阵
    Matrix operator&(const Matrix &mat) { return Matrix(*this) &= mat; }

    // 使用父类的运算符
    using super_type::operator[];

    // 幂运算
    // Matrix operator^(const element_type &) const;

public: //
    // 将矩阵的内容储存到一个串里
    std::string toString() const;

    /**
     * \brief 行交换
     * 当前对象会被改变,运算结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的矩阵为空、无效或索引越界时,抛出异常
     * \date by yingquelou at 2022-10-05 20:07:20
     */
    Matrix &lineExchange(const size_type &, const size_type &);
    // 行交换
    Matrix lineExchange(const size_type &i, const size_type &j) const { return Matrix(*this).lineExchange(i, j); }

    /**
     * \brief 初等变换——某行倍增
     * 当前对象会被改变,运算结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的矩阵为空、无效或索引越界时,抛出异常
     * \date by yingquelou at 2022-10-05 20:30:52
     */
    Matrix &lineMul(const size_type &, const element_type &);
    // 初等变换——某行倍增
    Matrix lineMul(const size_type &i, const element_type &k) const { return Matrix(*this).lineMul(i, k); }

    /**
     * \brief 初等变换——某行倍增所得加到某行上去
     * 当前对象会被改变,运算结果被保存在当前对象中
     * \return 当前对象的引用
     * \throws 当参与运算的矩阵为空、无效或索引越界时,抛出异常
     * \date by yingquelou at 2022-10-05 21:21:06
     */
    Matrix &lineMulToLine(const size_type &, const element_type &, const size_type &);
    /* 初等变换——某行倍增所得加到某行上去 注意矩阵的合法性*/
    Matrix lineMulToLine(const size_type &i, const element_type &k, const size_type &j) const { return Matrix(*this).lineMulToLine(i, k, j); }
    // 求可左乘当前矩阵的单位矩阵 注意矩阵的合法性
    Matrix LeftMulUnitMatrix(void) const { return UnitMatrix(size()); }
    // 求可右乘当前矩阵的单位矩阵 注意矩阵的合法性
    Matrix RightMulUnitMatrix(void) const { return UnitMatrix(GetColumn()); }

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