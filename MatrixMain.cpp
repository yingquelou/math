#include "Matrix.h"
#include "Matrix.cpp"
#include <algorithm>
#include <ctime>
#define X 10
#define Y 2
bool AssignValuesRandomly(Matrix &);
int main(void)
{
    Matrix A;
    AssignValuesRandomly(A);
    /* Mat1 r1{1, 1, -2};
    Mat1 r2{2, -1, -1};
    Mat1 r3{3, 6, -9};
    A.push_back(r1);
    A.push_back(r2);
    A.push_back(r3); */
    fill(A[0].begin(), A[0].end(), 0.0);
    // fill(A[1].begin(), A[1].end(), 0.0);
    Matrix::value_type;
    Matrix::const_iterator;
    Matrix::iterator;
    cout << A << endl;
    cout << endl;
    cout << A.RowSimplestFormOfMatrix() << endl;
    return 0;
}
bool AssignValuesRandomly(Matrix &mat)
{
    if (mat.empty())
    {
        srand((unsigned)time(NULL));
        auto p = rand() % X + Y;
        // auto q = rand() % X + Y;
        auto q = p + 4;
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