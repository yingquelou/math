#include "Matrix.h"
//#include "Matrix.cpp"
#include <ctime>
#define X 20
bool AssignValuesRandomly(Matrix &);
int main(void)
{
    Matrix A;
    AssignValuesRandomly(A);
    Matrix B;
    B = A;
    cout << A[0].size() << string(2, ' ') << A[0].capacity() << endl;
    // system("pause");
    return 0;
}
bool AssignValuesRandomly(Matrix &mat)
{
    if (mat.empty())
    {
        srand((unsigned)time(NULL));
        auto p = rand() % X + 2;
        auto q = rand() % X + 2;
        for (int i = 0; i < p; ++i)
        {
            Mat1 Rows;
            for (int j = 0; j < q; ++j)
            {
                Rows.push_back(rand() % X);
            }
            mat.push_back(Rows);
        }
        return true;
    }
    else
        return false;
}