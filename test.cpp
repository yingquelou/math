#include "Matrix.h"
// #include "Matrix.cpp"
#include <time.h>
#define X 20
bool AssignValuesRandomly(Matrix &);
int main(void)
{
    /*Matrix A;
    A.push_back({9, 0});
    A.push_back({1, 4});*/
    Matrix B;
    AssignValuesRandomly(B);
    B[0][0] = 0;
    std::cout << B << endl
              << endl
              << B.RowEchelonMatrix() << endl;
    return 0;
}
bool AssignValuesRandomly(Matrix &mat)
{
    if (mat.empty())
    {
        srand((unsigned)time(NULL));
        auto p = rand() % X + 2;
        auto q = rand() % X + 2;
        for (size_t i = 0; i < p; ++i)
        {
            Mat1 Rows;
            for (size_t j = 0; j < q; ++j)
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