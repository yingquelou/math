#include <iostream>
#include <vector>
#include <time.h>
using Dtype = double;
using namespace std;
#define N 10
#define y 0
static Dtype n_Determinant(vector<vector<Dtype>>);
int main(void)
{
    srand((unsigned)time(NULL));
    vector<vector<Dtype>> a;
    for (size_t i = 0; i < N; ++i)
    {
        vector<Dtype> tmp;
        for (size_t j = 0; j < N; ++j)
        {
            /*  if (j == i)
                a[i][i] = 0;
            else */
            tmp.push_back(rand() % 100);
        }
        a.push_back(tmp);
    }
    a[1][1] = 0;
    a[3][3] = 0;
    puts("");
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            if (j != N - 1)
                printf("%.*lf,", y, a[i][j]);
            else
                printf("%.*lf", y, a[i][j]);
        }
            puts("");
    }
    puts("\n");
    Dtype tmp = n_Determinant(a);
    for (size_t i = 0; i < N; ++i)
    {
        printf("[");
        for (size_t j = 0; j < N; ++j)
        {
            if (j != N - 1)
                printf("%.*lf,", y, a[i][j]);
            else
                printf("%.*lf", y, a[i][j]);
        }
        printf("]");
        if (i != N - 1)
            puts(",");
    }
    puts("");
    printf("%e\n", tmp);
    return 0;
}
static Dtype n_Determinant(vector<vector<Dtype>> a)
// 行列式转换为上三角,然后再求值
{
    size_t i = 0, j = 0, l = 0, n = a.size();
    Dtype k = 0;
    for (; j < n - 1; ++j)
    {
        if (a[j][j])
        {
            for (i = j + 1; i < n; ++i)
            {
                k = -a[i][j] / a[j][j];
                if (k)
                {
                    a[i][j] = 0;
                    for (l = j + 1; l < n; ++l)
                    {
                        a[i][l] += k * a[j][l];
                    }
                }
            }
        }
        else
        // 对角线有零的情况
        {
            vector<vector<Dtype>> tmp1 = a;
            a[j][j] = -1.0;
            vector<vector<Dtype>> tmp2 = a;
            a[j][j] = 1.0;
            return n_Determinant(tmp1) +
                   n_Determinant(tmp2);
        }
    }
    Dtype Product = 1;
    for (l = 0; l < n; ++l)
    {
        Product *= a[l][l];
    }
    return Product;
}