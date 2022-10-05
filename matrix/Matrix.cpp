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
Matrix Matrix::GetInverseMatrix(void) const
{
    const auto c = GetColumn();
    auto UM = UnitMatrix(c);
    if (UM == RowSimplestFormOfMatrix())
    {
        auto ret = *this;
        ret &= UM;
        ret.RefRowSimplestFormOfMatrix();
        for (size_t i = 0; i < c; ++i)
            ret[i].erase(ret[i].begin(), ret[i].begin() + c);
        return ret;
    }
    else
        return Matrix();
}
inline size_t Matrix::GetColumn(void) const
{
    if (size() > 0)
        return begin()->size();
    else
        return 0;
}
Matrix &Matrix::RSFM_REM(bool TF)
{
    const auto r = GetRow(), c = GetColumn();
    auto result = *this;
    size_t SwapPos = r - 1;
    auto NotZeroLog = new int[r];
    size_t ZeroLine = 0;
    for (size_t i = 0; i < r;)
    {
        size_t j = 0;
        while (j < c && !result[i][j])
            ++j;
        if (j < c)
        // 找到当前行的第一个非零元素时:
        { // NotZeroLog数组记录每一行第一个非零元素的位置
            NotZeroLog[i] = j;
            auto times = result[i][j];
            result[i][j] = 1; //该非零元素置为1
            for (size_t k = j + 1; k < c; ++k)
            // 处理该非零元同行的元素 初等行变换——行倍增(减)
            {
                result[i][k] /= times;
            }
            // 处理该非零元同列的元素(置为0) 初等行变换——行倍增(减)并加到另一行
            if (TF)
                for (size_t u = 0; u < i; ++u)
                // 1.处理该非零元上方的同列元素
                {
                    if (times = result[u][j])
                        for (size_t x = j; x < c; ++x)
                        {
                            result[u][x] -= times * result[i][x];
                        }
                }
            for (size_t d = i + 1; d < r; ++d)
            // 2.处理该非零元下方的同列元素
            {
                if (times = result[d][j])
                    for (size_t x = j; x < c; ++x)
                    {
                        result[d][x] -= times * result[i][x];
                    }
            }
            ++i;
        }
        else if (i < SwapPos)
        // 找到一个全零行且满足行交换条件(可能未找出所有全零行的情况下)
        // 行交换条件:SwapPos标记了下一次用于交换的行(如果有全零行,从尾行开始交换)
        // 当浮标i与SwapPos未相遇(即i<SwapPos),
        {
            result = result.lineExchange(i, SwapPos);
            NotZeroLog[SwapPos] = -1; // NotZeroLog数组中值为-1的元素的下标表明对应行为全零行
            ++ZeroLine;               // ZeroLine最终记录了全零行的数目
            --SwapPos;                //已经完成了某次交换,SwapPos改变,将标记下一次用于交换的行
        }
        else
        {
            ++ZeroLine;
            NotZeroLog[i] = -1;
            // 已经找到最后一个全零行,将退出外循环
            break;
        }
        // 当外循环条件为假时，亦将退出外循环
    }
    // 利用NotZeroLog数组生成result的特征map
    std::map<int, size_t> ma;
    for (size_t i = 0; i < r; ++i)
        ma[NotZeroLog[i]] = i;
    Matrix &ret = *this;
    ret.clear();
    /* for (auto &i : ma)
        ret.push_back(result[i.second]); */
    // for_each利用特征map放置好非零行向量
    for_each(ma.cbegin(), ma.cend(), [&ret, &result](decltype(ma)::value_type it)
             {if(it.first>=0)ret.push_back(result[it.second]); });
    //  再放置好ZeroLine个全零行
    Matrix::row_type tmp(c, 0);
    for (size_t i = 0; i < ZeroLine; ++i)
        ret.push_back(tmp);
    // 动态内存释放
    delete[] NotZeroLog;
    NotZeroLog = nullptr;
    // ret.shrink_to_fit();
    return ret;
}
Matrix &Matrix::RefRowSimplestFormOfMatrix(void) { return RSFM_REM(true); }
Matrix Matrix::RowSimplestFormOfMatrix(void) const
{
    auto ret = *this;
    return ret.RSFM_REM(true);
}
Matrix &Matrix::RefRowEchelonMatrix(void) { return RSFM_REM(false); }
Matrix Matrix::RowEchelonMatrix(void) const
{
    auto ret = *this;
    return ret.RSFM_REM(false);
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
        for (size_t i = 0; i < sz; i++)
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
        auto &&m = mat.GetRow();
        auto &&q = mat.GetColumn();
        if (m == GetColumn())
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
                    for (size_t k = 0; k < m; k++)
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
        for (size_t i = 0; i < size(); ++i)
            for (auto &j : mat[i])
                ret[i].push_back(j);
        return ret;
    }
    else
        throw std::invalid_argument(reportException("invalid_argument"));
}
bool Matrix::operator==(const Matrix &mat) const
{
    if (*this && mat && mat.GetRow() == GetRow() && mat.GetColumn() == GetColumn())
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
Matrix &Matrix::lineExchange(const size_t &i, const size_t &j)
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
Matrix &Matrix::lineMul(const size_t &i, const element_type &k)
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
Matrix &Matrix::lineMulToLine(const size_t &i, const element_type &k, const size_t &j)
{

    if (*this)
    {
        if (i < size() && j < size())
        {
            Matrix &ret = *this;
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
Matrix Matrix::TransposeMatrix(void) const
{
    Matrix result;
    auto R = GetRow(), C = GetColumn();
    for (decltype(R) i = 0; i < C; ++i)
    {
        row_type Rows;
        for (decltype(R) j = 0; j < R; j++)
        {
            Rows.push_back((*this)[j][i]);
        }
        result.push_back(Rows);
    }
    return result;
}
size_t Matrix::RankOfMatrix(void) const
{
    if (empty())
        return 0;
    auto mat = *this;
    mat.RSFM_REM(false);
    size_t ret = 0, r = size(), c = GetColumn();
    auto tmp = Matrix::value_type(c, 0.0);
    for (const auto &val : mat)
    {
        if (val == tmp)
            ++ret;
    }
    return r - ret;
}
Matrix Matrix ::UnitMatrix(const size_t &n)
{
    Matrix::row_type Rows(n, 0);
    Matrix result;
    for (size_t i = 0; i < n; ++i)
    {
        result.push_back(Rows);
        result[i][i] = 1;
    }
    result.shrink_to_fit();
    return result;
}
Matrix Matrix::AssignValuesRandomly(const size_t &r = 3, const size_t &c = 3, const element_type &inf = 0, const element_type &sup = 10)
{
    static std::default_random_engine rd(static_cast<unsigned>(time(nullptr))); // 将用于获得随机数引擎的种子
    Matrix ret;
    if (!r || !c || inf >= sup)
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