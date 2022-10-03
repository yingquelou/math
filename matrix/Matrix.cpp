#include "Matrix.h"
#include <algorithm>
#include <random>
#include <map>
#include <sstream>
// 类成员函数

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
Matrix::Matrix(void)
{
    clear();
}
Matrix::Matrix(Matrix::const_iterator &be, Matrix::const_iterator &en)
{
    if (be < en)
        for (auto it = be; it != en; ++it)
            push_back(*it);
}
Matrix::Matrix(const std::initializer_list<Matrix::row_type> &il)
{
    for (auto &&e : il)
        push_back(e);
}
bool Matrix::IsLegitimate(void) const
{
    auto ret = true;
    if (empty())
        ret = false;
    else
    {
        auto c = cbegin()->size();
        if (!c)
            return false;
        for (auto i = cbegin() + 1; i != cend(); ++i)
            if (i->size() != c)
            {
                ret = false;
                break;
            }
    }
    return ret;
}
inline size_t Matrix::GetRows(void) const
{
    return size();
}
inline size_t Matrix::GetColumn(void) const
{
    return begin()->size();
}
Matrix &Matrix::RSFM_REM(bool TF)
{
    const auto r = GetRows(), c = GetColumn();
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
            result = result.LineExchange(i, SwapPos);
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
Matrix &Matrix::operator=(const Matrix mat)
{
    clear();
    for (auto &&i : mat)
    {
        push_back(i);
        // cout << size() << string(2, ' ') << capacity() << endl;
    }
    shrink_to_fit();
    return *this;
}
Matrix Matrix::operator+(const Matrix &mat) const
{
    Matrix ret(*this);
    return ret += mat;
}
Matrix &Matrix::operator+=(const Matrix &mat)
{
    auto p = GetRows(), q = GetColumn();
    for (decltype(p) i = 0; i < p; ++i)
        for (decltype(p) j = 0; j < q; ++j)
            (*this)[i][j] += mat[i][j];
    return *this;
}
Matrix Matrix::operator*(const element_type &k) const
{
    Matrix result;
    for (auto &i : *this)
    {
        row_type Rows;
        for (auto &j : i)
        {
            Rows.push_back(k * j);
        }
        result.push_back(Rows);
    }
    return result;
}
Matrix &Matrix::operator*=(const element_type &k)
{
    for (auto &i : *this)
        for (auto &j : i)
            j *= k;
    return *this;
}
inline Matrix operator*(const Matrix::element_type &k, const Matrix &mat)
{
    return mat * k;
}
Matrix Matrix::operator*(const Matrix &mat) const
{
    // A(m*p)*B(p*n)
    auto m = size(), p = mat.size(), n = mat.GetColumn();
    Matrix result;
    if (p != GetColumn())
        return result;
    element_type tmp;
    size_t i, j, k;
    // 开始计算
    for (i = 0; i < m; ++i)
    {
        row_type Rows;
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
    element_type tmp;
    decltype(m) i, j, k;
    // 开始计算
    for (i = 0; i < m; ++i)
    {
        row_type Rows;
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
    {
        std::cerr << "Splicing failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}
bool Matrix::operator==(const Matrix &mat) const
{
    auto ret = true;
    if (*this && mat)
    {
        auto r = mat.size(), c = mat.GetColumn();
        if (r != size() || c != GetColumn())
            return false;
        auto &ma = *this;
        for (size_t i = 0; i < r; ++i)
        {
            /* for (size_t j = 0; j < c; ++j)
                if (ma[i][j] != mat[i][j]) */
            if (ma[i] != mat[i])
                ret = false;
        }
    }
    else
        ret = false;
    return ret;
}
Matrix &Matrix::RefLineExchange(const size_t &i, const size_t &j)
{
    if (*this && i != j && i < size() && j < size())
    {
        Matrix &result = *this;
        auto c = GetColumn();
        for (size_t k = 0; k < c; ++k)
            if (result[j][k] || result[i][k])
            {
                auto tmp = result[i][k];
                result[i][k] = result[j][k];
                result[j][k] = tmp;
            }
        return result;
    }
    else
    {
        std::cerr << "It is not a matrix,or Not exchangeable!" << std::endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineExchange(const size_t &i, const size_t &j) const
{
    auto ret = *this;
    return ret.RefLineExchange(i, j);
}
Matrix &Matrix::RefLineMul(const size_t &i, const element_type &k)
{
    if (*this && i < size() && k)
    {
        Matrix &result = *this;
        for_each(result[i].begin(), result[i].end(), [&k](element_type &ba)
                 { ba *= k; });
        return result;
    }
    else
    {
        std::cerr << "It is not a matrix,or the \"k\" is zero,or the \"i\" is too large!" << std::endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineMul(const size_t &i, const element_type &k) const
{
    auto ret = *this;
    return ret.RefLineMul(i, k);
}
Matrix &Matrix::RefLineMulToLine(const size_t &i, const element_type &k, const size_t &j)
{
    if (*this && i < size() && k && j < size())
    {
        Matrix &ret = *this;
        Matrix::row_type ik;
        for_each(ret[i].cbegin(), ret[i].cend(), [&ik, &k](const element_type &ba)
                 { ik.push_back(ba * k); });
        auto c = GetColumn();
        for (size_t t = 0; t < c; ++t)
            ret[j][t] += ik[t];
        return ret;
    }
    else
    {
        std::cerr << "It is not a matrix,or the \"i\" is too large,or the \"j\" is too large,or the \"k\" is zero!" << std::endl;
        exit(EXIT_FAILURE);
    }
}
Matrix Matrix::LineMulToLine(const size_t &i, const element_type &k, const size_t &j) const
{
    auto ret = *this;
    return ret.RefLineMulToLine(i, k, j);
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
static std::default_random_engine rd; // 将用于获得随机数引擎的种子
Matrix Matrix::AssignValuesRandomly(const size_t &r = 3, const size_t &c = 3, const element_type &inf = 0, const element_type &sup = 10)
{
    Matrix ret;
    if (!r || !c || inf >= sup)
        return ret;
    std::uniform_real_distribution<element_type> dis(inf, sup); // 以 rd() 播种的标准 mersenne_twister_engine
    /*     auto range = sup - inf;
        decltype(range) base;
        if (range > 0)
            base = inf;
        else if (range < 0)
        {
            range *= -1;
            base = sup;
        }
        else
            return ret;
        ++range; */
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