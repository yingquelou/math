#include "Matrix.h"
bool Matrix::IsLegitimate() const
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
ostream &operator<<(ostream &cout, const Matrix &mat)
{
    if (mat.IsLegitimate())
    {
        size_t Rows = mat.size();
        for (size_t i = 0; i < Rows; ++i)
        {
            size_t L = mat[i].size(), m = 0;
            for (auto j : mat[i])
            {
                ++m;
                cout << j;
                if (m < L)
                    cout << ",";
            }
            if (i < Rows - 1)
                cout << endl;
        }
    }
    else
    {
        cout << "It isn't a legitimate matrix!" << endl;
    }
    return cout;
}
Matrix Matrix::operator+(const Matrix &mat) const
{
    if (!(IsLegitimate() && mat.IsLegitimate()))
        exit(EXIT_FAILURE);
    size_t p = size(), q = begin()[0].size();
    if (p == mat.size() && q == mat[0].size())
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
        MyDouble tmp;
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