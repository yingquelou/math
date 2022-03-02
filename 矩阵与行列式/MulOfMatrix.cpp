/* 矩阵的乘法 */
#include <iostream>
#include <vector>
#include <time.h>
/* #define M 3
#define P 4
#define N 4 */
#define Mod 50
using namespace std;
using MyDouble = long double;
// 行/列矩阵
using Value_Type = vector<MyDouble>;
// 二维矩阵
using Mat2 = vector<Value_Type>;
ostream &operator<<(ostream &, const Mat2 &);
Mat2 MulOfMatrix(const Mat2 &, const Mat2 &);
int main(void)
{
    /* Mat2 m1 = {{-1, 1}, {2, -2}}, m2 = {{2, 1}, {-6, -3}};
    // Mat m1 = {{3, -1, 1}, {2, 2, 0}}, m2 = {{1, -1, 0}, {1, 1, 1}, {2, 1, -1}};
    // Mat2 tmp1 = MulOfMatrix(m1, m2);
    Mat2 tmp2 = MulOfMatrix(m2, m1);
    cout << m1 << endl;
    cout << m2 << endl;
    cout << tmp2 << endl; */
    srand((unsigned)time(NULL));
    int M = 1 + rand() % Mod, P = 1 + rand() % Mod, N = 1 + rand() % Mod;
    Mat2 m1, m2;
    for (size_t i = 0; i < M; ++i)
    {
        Value_Type rows;
        for (size_t j = 0; j < P; ++j)
        {
            rows.push_back(rand() % Mod);
        }
        m1.push_back(rows);
    }
    for (size_t i = 0; i < P; ++i)
    {
        Value_Type rows;
        for (size_t j = 0; j < N; ++j)
        {
            rows.push_back(rand() % Mod);
        }
        m2.push_back(rows);
    }
    cout << m1 << endl
         << m2 << endl;
    Mat2 tmp = MulOfMatrix(m1, m2);
    cout << tmp << endl;
    return 0;
}
ostream &operator<<(ostream &cout, const Mat2 &mat)
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
        cout << endl;
    }
    return cout;
}
Mat2 MulOfMatrix(const Mat2 &Matrix1, const Mat2 &Matrix2)
// A(m×p)  B(p×n)
{
    size_t m = Matrix1.size(), p = Matrix1[0].size(), n = Matrix2[0].size();
    // 检查两个矩阵能否相乘
    if (!m || !p || !n || (p != Matrix2.size()))
    {
        cerr << "The two cannot be multiplied." << endl;
        exit(EXIT_FAILURE);
    }
    unsigned i, j, k;
    // 检查第一个参数是否是合法矩阵
    for (i = 1; i < m; ++i)
    {
        if (Matrix1[i].size() != p)
        {
            cerr << "The first is not a matrix." << endl;
            exit(EXIT_FAILURE);
        }
    }
    // 检查第二个参数是否是合法矩阵
    for (i = 1; i < p; ++i)
    {
        if (Matrix2[i].size() != n)
        {
            cerr << "The second is not a matrix." << endl;
            exit(EXIT_FAILURE);
        }
    }
    Mat2 result;
    MyDouble tmp;
    // 开始计算
    for (i = 0; i < m; ++i)
    {
        Value_Type Rows;
        for (j = 0; j < n; ++j)
        {
            for (k = 0, tmp = 0; k < p; ++k)
            {
                tmp += Matrix1[i][k] * Matrix2[k][j];
            }
            Rows.push_back(tmp);
        }
        result.push_back(Rows);
    }
    return result;
}