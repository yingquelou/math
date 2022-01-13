#include "Matrix.h"
bool Matrix::IsLegitimate(void) const
{
    auto Column = begin()->size();
    // 检查是否是合法矩阵
    if (empty())
        return false;
    for (auto i : *this)
        if (Column != i.size())
            return false;
    return true;
}
size_t Matrix::GetRows(void) const
{
    if (IsLegitimate())
        return size();
    else
    {
        cout << "It isn't a legitimate matrix!" << endl;
        exit(EXIT_FAILURE);
    }
}
size_t Matrix::GetColumn(void) const
{
    if (IsLegitimate())
        return begin()->size();
    else
    {
        cout << "It isn't a legitimate matrix!" << endl;
        exit(EXIT_FAILURE);
    }
}
/* Matrix Matrix::RowEchelonMatrix(void) const
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
                result = result.LineExchange(j + 1, l + 1);
            } while (!result[j][j] || l == p);
        }
    }
    for (i = 0; i < r; ++i)
        for (j = 0; j < q;)
        {
            auto tmp = result[i][j];
            if (tmp)
            {
                if (tmp != 1)
                {
                    result = result.LineMul(i, 1.0 / tmp);
                    break;
                }
            }
            else
            {
                ++j;
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
            printf("%+6.3llf", j);
            // cout << j;
            if (m < L)
                cout << ",";
        }
        if (i < Rows - 1)
            cout << endl;
    }
    return cout;
}
Matrix Matrix::operator+(const Matrix &mat) const
{
    size_t p = GetRows(), q = GetColumn();
    if (p == mat.GetRows() && q == mat.GetColumn())
    {
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
    else
    {
        cerr << "The both isn't the same type of matrix!" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::operator*(const LDouble &k) const
{
    if (IsLegitimate())
    {
        Matrix result;
        for (auto i : *this)
        {
            Mat1 Rows;
            for (auto j : i)
            {
                Rows.push_back(k * j);
            }
            result.push_back(Rows);
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix operator*(const LDouble &k, const Matrix &mat)
{
    return mat * k;
}
Matrix Matrix::operator*(const Matrix &mat) const
{
    if (!(IsLegitimate() && mat.IsLegitimate()))
        exit(EXIT_FAILURE);
    // A(m*p)*B(p*n)
    auto m = size(), p = begin()[0].size(), n = mat[0].size();
    // 检查两个矩阵能否相乘
    if (p == mat.size())
    {
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
    else
    {
        cerr << "The second is not a matrix." << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineExchange(const size_t i, const size_t j) const
{
    if (IsLegitimate() && i != j && i <= size() && j <= size())
    {
        Matrix result = *this;
        for (size_t k = 0; k < size(); ++k)
        {
            auto tmp = result[i - 1][k];
            result[i - 1][k] = result[j - 1][k];
            result[j - 1][k] = tmp;
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix,or Not exchangeable!" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineMul(const size_t i, const LDouble k) const
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
Matrix Matrix::LineMulToLine(const size_t i, const LDouble k, const size_t j) const
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
Matrix Matrix::LeftMulUnitMatrix(void) const
{
    if (IsLegitimate())
    {
        Matrix result;
        auto n = size();
        for (size_t i = 0; i < n; ++i)
        {
            Mat1 Rows;
            for (size_t j = 0; j < n; ++j)
            {
                if (i == j)
                {
                    Rows.push_back(1);
                }
                else
                    Rows.push_back(0);
            }
            result.push_back(Rows);
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix" << endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::RightMulUnitMatrix(void) const
{
    if (IsLegitimate())
    {
        Matrix result;
        auto n = begin()[0].size();
        for (size_t i = 0; i < n; ++i)
        {
            Mat1 Rows;
            for (size_t j = 0; j < n; ++j)
            {
                if (i == j)
                {
                    Rows.push_back(1);
                }
                else
                    Rows.push_back(0);
            }
            result.push_back(Rows);
        }
        return result;
    }
    else
    {
        cerr << "It is not a matrix" << endl;
        exit(EXIT_FAILURE);
    }
}