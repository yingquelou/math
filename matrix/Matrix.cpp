#include "Matrix.h"
#include <algorithm>
#include <random>
#include <map>
#include <sstream>
#include <stdexcept>
#define $ << '|' <<
#define reportException(msg)                   \
    ((std::stringstream()                      \
      << __DATE__                              \
             $ __TIME__                        \
                 $ __FILE__                    \
                     $ __FUNCTION__ $ __LINE__ \
                         $ msg)                \
         .str())
Matrix Matrix::GetInverseMatrix() const
{
    const auto c = getColumn();
    auto UM = UnitMatrix(c);
    if (UM == standardShape())
    {
        auto ret = *this;
        ret &= UM;
        ret.standardShape();
        for (size_type i = 0; i < c; ++i)
            ret[i].erase(ret[i].begin(), ret[i].begin() + c);
        return ret;
    }
    else
        return Matrix();
}
inline Matrix::size_type Matrix::getColumn() const
{
    if (size() > 0)
        return begin()->size();
    else
        return 0;
}
Matrix &Matrix::rowEchelonMatrix()
{
    if (*this)
    {
        const auto &&r = size();
        const auto &&c = getColumn();
        auto &m = *this;
        std::map<size_type, row_type> maps;
        size_type j;
        for (size_type i = 0; j = 0, i < r; ++i)
        {
            while (j < c && !m[i][j])
                ++j;
            if (j != c)
            // 找到当前行首非零元的位置 j
            {
                auto &curLine = m[i];
                auto &&it = curLine.begin() + j;
                auto k = *it;
                if (k != 1) // 当前行首非零元化为1
                    std::for_each_n(it, c - j, [&k](element_type &e)
                                    { e /= k; });
                // 行之间要排序,map以当前行首非零元的位置为键保存当前行
                // 但全零行不会被保存
                maps[j] = curLine;
                // 利用"初等变换——某行倍增所得加到某行上去"将当前首非零元下方的同列非零元化为零
                for (size_type n = i + 1; n < r; ++n)
                {
                    // 倍增的比例
                    auto &&proportion = -1 * m[n][j];
                    if (proportion)
                        lineMulToLine(i, proportion, n);
                }
            }
        }
        j = 0;
        // 处理非全零行
        for (auto &i : maps)
        {
            m[j] = std::move(i.second);
            ++j;
        }
        // 处理全零行
        while (r > j)
        {
            m[j] = std::move(row_type(c, 0));
            ++j;
        }
        return m;
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
std::ostream &operator<<(std::ostream &cout, const Matrix &mat)
{
    return cout << mat.toString();
}
Matrix::operator bool() const
{
    if (empty())
        return false;
    auto &&sz = begin()->size();
    if (sz > 0 && std::all_of(cbegin() + 1, cend(),
                              [&sz](const row_type &r)
                              { return r.size() == sz; }))
        return true;
    return false;
}
Matrix &Matrix::operator+=(const Matrix &mat)
{
    if (*this == mat)
    {
        auto &m = const_cast<Matrix &>(mat);
        auto &&sz = size();
        for (size_type i = 0; i < sz; i++)
        {
            auto &&be = at(i).begin();
            std::transform(be, at(i).end(), m[i].begin(), be,
                           [](element_type &x, element_type &y)
                           { return x + y; });
        }
        return *this;
    }
    else
        throw std::invalid_argument(reportException("Both must be Homotypic matrix"));
}
Matrix &Matrix::operator*=(const element_type &k)
{
    if (*this)
    {
        for (auto &i : *this)
            for (auto &j : i)
                j *= k;
        return *this;
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
Matrix &Matrix::operator*=(const Matrix &mat)
{
    // A(pm) * B(mq)
    if (mat && *this)
    {
        auto &&p = size();
        auto &&m = mat.getRow();
        auto &&q = mat.getColumn();
        if (m == getColumn())
        {
            Matrix left(*this);
            Matrix right(mat); // 预防自赋值问题
            for (size_type i = 0; i < p; ++i)
            {
                auto &r = at(i);
                r.clear();
                for (size_type j = 0; j < q; j++)
                {
                    element_type sum = 0;
                    for (size_type k = 0; k < m; k++)
                        sum += left[i][k] * right[k][j];
                    r.push_back(sum);
                }
            }
            return *this;
        }
        else
            throw std::invalid_argument(reportException("The matrix can be multiplied?"));
    }
    else
        throw std::invalid_argument(reportException("matrix is null?"));
}
Matrix &Matrix::operator&=(const Matrix &mat)
{
    if (*this && mat && size() == mat.size())
    {
        auto &ret = *this;
        for (size_type i = 0; i < size(); ++i)
            for (auto &j : mat[i])
                ret[i].push_back(j);
        return ret;
    }
    else
        throw std::invalid_argument(reportException("invalid_argument"));
}
bool Matrix::operator==(const Matrix &mat) const
{
    if (*this && mat && mat.getRow() == getRow() && mat.getColumn() == getColumn())
        return true;
    return false;
}
bool Matrix::operator!=(const Matrix &mat) const
{
    if (*this == mat)
    {
        if (!std::equal(cbegin(), cend(), mat.cbegin()))
            return true;
    }
    else
        return true;
    return false;
}
Matrix &Matrix::lineExchange(const size_type &i, const size_type &j)
{
    if (*this)
    {
        if (i < size() && j < size())
        {
            auto &m = *this;
            if (i != j)
                std::swap_ranges(m[i].begin(), m[i].end(), m[j].begin());
            return m;
        }
        else
            throw std::out_of_range(reportException(""));
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
Matrix &Matrix::lineMul(const size_type &i, const element_type &k)
{
    if (*this)
    {
        if (i < size())
        {
            Matrix &result = *this;
            if (k != 1)
            {
                for_each(result[i].begin(), result[i].end(), [&k](element_type &ba)
                         { ba *= k; });
            }
            return result;
        }
        else
            throw std::out_of_range(reportException(""));
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
Matrix &Matrix::lineMulToLine(const size_type &i, const element_type &k, const size_type &j)
{
    Matrix &ret = *this;
    if (ret)
    {
        if (i < size() && j < size())
        {
            if (k)
            {
                auto &&it = ret[j].begin();
                std::transform(ret[i].begin(), ret[i].end(), it, it,
                               [&k](const element_type &x, const element_type &y)
                               { return x * k + y; });
            }
            return ret;
        }
        else
            throw std::out_of_range(reportException(""));
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
Matrix &Matrix::transpose()
{
    auto &m = *this;
    if (m)
    {
        Matrix result;
        auto &&R = size();
        auto &&C = getColumn();
        decltype(R) j = 0;
        for (decltype(R) i = 0; i < C; ++i)
        {
            row_type Rows;
            for (j = 0; j < R; ++j)
            {
                Rows.push_back(m[j][i]);
            }
            result.push_back(Rows);
        }
        return m = std::move(result);
    }
    else
        throw std::invalid_argument(reportException("Is a matrix?"));
}
Matrix::size_type Matrix::RankOfMatrix() const
{
    auto &&m = rowEchelonMatrix();
    auto &&r = size() - 1;
    size_type ret = 0;
    row_type row(getColumn(), 0);
    while (m[r] == row)
    {
        ++ret;
        --r;
    }
    return ret;
}
Matrix Matrix ::UnitMatrix(const size_type &n)
{
    Matrix::row_type Rows(n, 0);
    Matrix result;
    for (size_type i = 0; i < n; ++i)
    {
        result.push_back(Rows);
        result[i][i] = 1;
    }
    return result;
}
Matrix Matrix::AssignValuesRandomly(const size_type &r = 3, const size_type &c = 3, const element_type &inf = 0, const element_type &sup = 10)
{
    static std::default_random_engine rd(static_cast<unsigned>(time(nullptr))); // 将用于获得随机数引擎的种子
    Matrix ret;
    if (!r || !c)
        return ret;
    std::uniform_real_distribution<element_type> dis(inf, sup); // 以 rd() 播种的标准 mersenne_twister_engine
    for (int i = 0; i < r; ++i)
    {
        row_type Rows;
        for (int j = 0; j < c; ++j)
        {
            Rows.push_back(dis(rd));
        }
        ret.push_back(Rows);
    }
    return ret;
}
std::string Matrix::toString() const
{
    std::stringstream ret;
    ret << "[\n";
    for (auto &r : *this)
    {
        ret << "[";
        std::stringstream tmp;
        for (auto &v : r)
            tmp << v << ", ";
        std::string &&s = tmp.str();
        auto pos = s.find_last_of(',');
        if (pos != std::string::npos)
            s.erase(pos, 2);
        ret << s << "]\n";
    }
    ret << ']';
    return ret.str();
}