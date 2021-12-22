// 矩阵的乘法
#include <iostream>
#include <vector>
using namespace std;
using MyDouble = double;
vector<vector<MyDouble>> MulOfMatrix(vector<vector<MyDouble>>, vector<vector<MyDouble>>);
int main(void)
{
    cout << MulOfMatrix() << endl;
    cout << "hello world!" << endl;
    return 0;
}
vector<vector<MyDouble>> MulOfMatrix(vector<vector<MyDouble>> Matrix1, vector<vector<MyDouble>> Matrix2)
// A(m×p)  B(p×n)
{
    size_t m = Matrix1.size(), p = Matrix1[0].size(), n = Matrix2[0].size();
    if (p != Matrix2.size())
    {
        cerr << "Operation is not legal." << endl;
        exit(EXIT_FAILURE);
    }
    unsigned i, j, k;
    for (i = 0; i < m; ++i)
    {
    }
}