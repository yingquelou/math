#include "Matrix.h"
#include <algorithm>
#include <map>
bool Matrix::IsLegitimate(void) const
{
    // 检查是否是合法矩阵
    if (empty())
        return false;
    auto Column = begin()->size();
    for (auto &i : *this)
        if (Column != i.size())
            return false;
    return true;
}
inline size_t Matrix::GetRows(void) const
{
    return size();
}
inline size_t Matrix::GetColumn(void) const
{
    return begin()->size();
}
Matrix Matrix::RowSimplestFormOfMatrix(void) const
{
    auto r = GetRows(), c = GetColumn();
    auto result = *this;
    size_t SwapPos = r - 1;
    auto setlog = new int[r];
    size_t ZeroLine = 0;
    for (size_t i = 0; i < r;)
    {
        size_t j = 0;
        while (j < c && !result[i][j])
            ++j;
        if (j < c)
        // 找到当前行的第一个非零项时:
        {
            setlog[i] = j;
            auto times = result[i][j];
            result[i][j] = 1;
            for (size_t k = j + 1; k < c; ++k)
            {
                result[i][k] /= times;
            }
            for (size_t l = 0; l < r; ++l)
            {
                if (l != i)
                {
                    times = result[l][j];
                    for (size_t x = 0; x < c; ++x)
                    {
                        result[l][x] -= times * result[i][x];
                    }
                }
            }
            ++i;
        }
        else if (i < SwapPos)
        // 找到一个全零行且满足行交换条件
        {
            result = result.LineExchange(i, SwapPos);
            setlog[SwapPos] = -1;
            ++ZeroLine;
            --SwapPos;
        }
        else
            break;
    }
    map<int, decltype(r)> ma;
    for (size_t i = 0; i < r; ++i)
        ma[setlog[i]] = i;
    Matrix ret;
    /* for (auto &i : ma)
        ret.push_back(result[i.second]); */
    for_each(ma.cbegin(), ma.cend(), [&](decltype(ma)::value_type it)
             {if(it.first>=0)ret.push_back(result[it.second]); });
    Matrix::value_type tmp(c, 0);
    for (size_t i = 0; i < ZeroLine; ++i)
        ret.push_back(tmp);
    delete setlog;
    setlog = nullptr;
    // ret.shrink_to_fit();
    return ret;
}
/* Matrix Matrix::RowSimplestFormOfMatrix(void) const
{
    size_t p = GetRows(), q = GetColumn(), r = p;
    if (r > q)
        r = q;
    size_t i, j, l;
    LDouble k;
    Matrix result = *this;
    for (j = 0; j < r - 1; ++j)
    {
        if (result[j][j])
        {
            for (i = j + 1; i < r; ++i)
            {
                k = -result[i][j] / result[j][j];
                if (k)
                {
                    result[i][j] = 0;
                    for (l = j + 1; l < q; ++l)
                    {
                        result[i][l] += k * result[j][l];
                    }
                }
            }
        }
        else
        {
            l = j;
            do
            {
                ++l;
                result = result.LineExchange(j, l);
            } while (!result[j][j] && l < p - 1);
            // 当某列对角线下均为零且此列对角线亦为零时，如何处理？
        }
    }
    for (i = 0; i < r; ++i)
    {
        Matrix::value_type::value_type tmp = result[i][i];
        if (tmp)
        {
            result[i][i] = 1.0;
            for (j = i + 1; j < q; ++j)
            {
                result[i][j] /= tmp;
            }
        }
    }
    for (i = 1; i < r; ++i)
    {
        for (j = 0; j < i; ++j)
        {
            k = result[j][i];
            result[j][i] = 0;
            for (l = i + 1; l < q; ++l)
            {
                result[j][l] -= k * result[i][l];
            }
        }
    }
    return result;
} */
ostream &operator<<(ostream &cout, const Matrix &mat)
{
    size_t Rows = mat.GetRows();
    for (size_t i = 0; i < Rows; ++i)
    {
        size_t L = mat[i].size(), m = 0;
        for (auto j : mat[i])
        {
            ++m;
            // printf("%+.3llf", j);
            cout << setfill(' ') << setprecision(3) << j;
            if (m < L)
                cout << ",";
        }
        if (i < Rows - 1)
            cout << endl;
    }
    return cout;
}
Matrix &Matrix::operator=(const Matrix &mat)
{
    const auto be = begin(), en = end();
    erase(be, en);
    for (auto &i : mat)
    {
        push_back(i);
        // cout << size() << string(2, ' ') << capacity() << endl;
    }
    shrink_to_fit();
    return *this;
}
Matrix Matrix::operator+(const Matrix &mat) const
{
    size_t p = GetRows(), q = GetColumn();
    Matrix tmp;
    for (size_t i = 0; i < p; ++i)
    {
        Mat1 line;
        for (size_t j = 0; j < q; ++j)
        {
            line.push_back((*this)[i][j] + mat[i][j]);
        }
        tmp.push_back(line);
    }
    return tmp;
}
Matrix &Matrix::operator+=(const Matrix &mat)
{
    auto p = GetRows(), q = GetColumn();
    for (decltype(p) i = 0; i < p; ++i)
        for (decltype(p) j = 0; j < q; ++j)
            (*this)[i][j] += mat[i][j];
    return *this;
}
Matrix Matrix::operator*(const LDouble &k) const
{
    Matrix result;
    for (auto &i : *this)
    {
        Mat1 Rows;
        for (auto &j : i)
        {
            Rows.push_back(k * j);
        }
        result.push_back(Rows);
    }
    return result;
}
Matrix &Matrix::operator*=(const LDouble &k)
{
    for (auto &i : *this)
        for (auto &j : i)
            j *= k;
    return *this;
}
inline Matrix operator*(const LDouble &k, const Matrix &mat)
{
    return mat * k;
}
Matrix Matrix::operator*(const Matrix &mat) const
{
    // A(m*p)*B(p*n)
    auto m = size(), p = GetColumn(), n = mat.GetColumn();
    Matrix result;
    MyType tmp;
    size_t i, j, k;
    // 开始计算
    for (i = 0; i < m; ++i)
    {
        Mat1 Rows;
        for (j = 0; j < n; ++j)
        {
            for (k = 0, tmp = 0; k < p; ++k)
            {
                tmp += (*this)[i][k] * mat[k][j];
            }
            Rows.push_back(tmp);
        }
        result.push_back(Rows);
    }
    return result;
}
Matrix &Matrix::operator*=(const Matrix &mat)
{
    auto m = size(), p = GetColumn(), n = mat.GetColumn();
    Matrix result;
    MyType tmp;
    decltype(m) i, j, k;
    // 开始计算
    for (i = 0; i < m; ++i)
    {
        Mat1 Rows;
        for (j = 0; j < n; ++j)
        {
            for (k = 0, tmp = 0; k < p; ++k)
            {
                tmp += (*this)[i][k] * mat[k][j];
            }
            Rows.push_back(tmp);
        }
        result.push_back(Rows);
    }
    *this = result;
    return *this;
}
inline Matrix Matrix::operator-(const Matrix &mat) const { return -1.0 * mat + (*this); }
Matrix &Matrix::operator-=(const Matrix &mat)
{
    auto p = GetRows(), q = GetColumn();
    for (decltype(p) i = 0; i < p; ++i)
        for (decltype(p) j = 0; j < q; ++j)
            (*this)[i][j] -= mat[i][j];
    return *this;
}
Matrix Matrix::LineExchange(const size_t &i, const size_t &j) const
{
    if (IsLegitimate() && i != j && i < size() && j < size())
    {
        Matrix result = *this;
        auto c = GetColumn();
        for (size_t k = 0; k < c; ++k)
        {
            auto tmp = result[i][k];
            result[i][k] = result[j][k];
            result[j][k] = tmp;
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix,or Not exchangeable!" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineMul(const size_t &i, const LDouble &k) const
{
    if (IsLegitimate() && i <= size() && k)
    {
        Matrix result = *this;
        auto q = GetColumn();
        for (size_t j = 0; j < q; ++j)
        {
            result[i][j] *= k;
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix,or the \"k\" is zero,or the \"i\" is too large!" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineMulToLine(const size_t &i, const LDouble &k, const size_t &j) const
{
    if (IsLegitimate() && i <= size() && j <= size())
    {
        Matrix result = this->LineMul(i, k);
        for (size_t t = 0; t < size(); ++t)
        {
            result[j][t] = (*this)[j][t] + result[i][t];
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix,or the \"i\" is too large,or the \"j\" is too large!" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix UnitMatrix(const size_t &n)
{
    Mat1 Rows(n, 0);
    Matrix result;
    for (size_t i = 0; i < n; ++i)
    {
        result.push_back(Rows);
        result[i][i] = 1;
    }
    result.shrink_to_fit();
    return result;
}
inline Matrix Matrix::LeftMulUnitMatrix(void) const
{
    auto n = GetRows();
    return UnitMatrix(n);
}
inline Matrix Matrix::RightMulUnitMatrix(void) const
{
    auto n = GetColumn();
    return UnitMatrix(n);
}
Matrix Matrix::TransposeMatrix(void) const
{
    Matrix result;
    auto R = GetRows(), C = GetColumn();
    for (decltype(R) i = 0; i < C; ++i)
    {
        Mat1 Rows;
        for (decltype(R) j = 0; j < R; j++)
        {
            Rows.push_back((*this)[j][i]);
        }
        result.push_back(Rows);
    }
    return result;
}