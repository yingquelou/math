#pragma once
#include <memory>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <algorithm>
#include <initializer_list>
class M_Base
{
    // 显示到标准输出
    friend std::ostream &operator<<(std::ostream &, const M_Base &);

public:
    using value_type =long double;
    using rows_type = std::vector<value_type>;
    using iterator = std::vector<rows_type>::iterator;

private:
    std::unique_ptr<std::vector<rows_type>> pB;
    bool status;

public:
    M_Base();
    // if TF is true,it will create a identity matrix
    // if TF is false,it will create a zero matrix
    M_Base(const size_t &, const bool &TF = false);
    M_Base(const M_Base &);
    M_Base(const std::initializer_list<rows_type> &);
    ~M_Base();

public:
    M_Base &operator=(const M_Base);
    rows_type& operator[](const size_t&);
    const rows_type &operator[](const size_t &) const;

public:
    explicit operator bool() const;

public:
    size_t GetRows() const;
    size_t GetColumn() const;
    iterator begin();
    iterator end();
    M_Base transpose()const;
};
// 矩阵类
class Matrix : public M_Base
{
    friend Matrix operator*(const Matrix::value_type &, const Matrix &);

public:
    using M_Base::M_Base;

public:
    Matrix &operator+=(const Matrix mat);
    Matrix operator+(const Matrix &) const;
    // Matrix operator-()const;
    Matrix &operator-=(const Matrix &);
    Matrix& operator*=(const Matrix);
    Matrix operator*(const Matrix &) const;
    Matrix operator*(const Matrix::value_type &) const;
    Matrix &operator&=(const Matrix &);

private:
    Matrix &RSFM_REM(const bool &);

public:
    Matrix &LineExchange(const size_t &, const size_t &);
    Matrix LineExchange(const size_t &, const size_t &) const;
    Matrix &LineMul(const size_t &, const Matrix::value_type &);
    Matrix LineMul(const size_t &, const Matrix::value_type &) const;
    Matrix &RowSimplestFormOfMatrix(void);
    Matrix &RowEchelonMatrix(void);
    size_t RankOfMatrix(void) const;
    Matrix &LineMulToLine(const size_t &, const Matrix::value_type &, const size_t &);
    Matrix LineMulToLine(const size_t &, const Matrix::value_type &, const size_t &) const;
    Matrix GetInverseMatrix(void) const;
};
class Determinant : public M_Base
{
public:
    using M_Base::M_Base;
private:
    bool RSFM_REM();
    //bool RSFM_REM()const;
public:
    Determinant& lineSwap(const size_t&, const size_t&);
public:
    Determinant::value_type operator+(const Determinant&)const;
    Determinant::value_type operator*(const Determinant&)const;
    Determinant::value_type operator()()const;
};