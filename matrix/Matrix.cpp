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
inline Matrix::size_type Matrix::getColumn() const
{
    if (size() > 0)
        return begin()->size();
    else
        return 0;
}
Matrix &Matrix::rowSimplestForm()
{
    auto &m = *this;
    if (m)
    {
        const auto &&r = size();
        const auto &&c = getColumn();
        size_type i, j;
        std::vector<size_type> vs;
        for (i = 0; j = 0, i < r; ++i)
        {
            auto &curLine = m[i];
            while (j < c && !curLine[j])
                ++j;
            // 保存当前行首非零元的位置
            // 如果当前行是全零行,保存的是列宽 c
            vs.push_back(j);
            if (j != c)
            // 如果不是全零行,进入
            {
                auto &&it = curLine.begin() + j;
                auto k = *it;
                if (k != 1) // 当前行首非零元化为1
                    std::for_each_n(it, c - j, [&k](element_type &e)
                                    { e /= k; });
                // 利用"初等变换——某行倍增所得加到某行上去"将当前首非零元的同列非零元化为零
                for (size_type n = 0; n < r; ++n)
                    if (n != i)
                    { // 倍增的比例
                        auto &&proportion = -1 * m[n][j];
                        if (proportion)
                            lineMulToLine(i, proportion, n);
                    }
            }
        }
        // 行之间要排序,multimap以每行首非零元的位置为键保存相应的行
        std::multimap<size_type, row_type> maps;
        for (i = 0; i < r; ++i)
            maps.emplace(std::move(vs[i]), std::move(m[i]));
        clear();
        for (auto &&i : maps)
            push_back(std::move(i.second));
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
    if (this == &mat)
        return false;
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
Matrix::size_type Matrix::rankOfMatrix()
{
    auto &m = rowSimplestForm();
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
Matrix Matrix::getInverseMatrix() const
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
std::string Matrix::toString() const
{
    std::stringstream ret;
    ret << '[';
    for (auto &r : *this)
    {
        ret << '[';
        std::stringstream tmp;
        for (auto &v : r)
            tmp << v << ',';
        std::string &&s = tmp.str();
        auto pos = s.rfind(',');
        if (pos != std::string::npos)
            s.erase(pos, 1);
        ret << s << "],";
    }
    ret << ']';
    std::string s(ret.str());
    auto &&pos = s.rfind(',');
    return std::move(s.erase(pos, 1));
}
// 类的静态成员
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
    return std::move(ret);
}
