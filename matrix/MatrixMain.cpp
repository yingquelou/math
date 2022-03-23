#include "Matrix.cpp"
#include <Windows.h>
int main(void)
{
     Matrix B{{-1, 1, -1},
              {1, -1, -1},
              {-1, -1, 1}};
     Matrix A;
     if (A || B)
          cout << "OK" << endl;
     // Matrix C(B.begin(),B.end());
     /* for (size_t i = 0; i < 10; ++i)
     {
          Matrix D = AssignValuesRandomly(10, 7, -100, 100).RefRowSimplestFormOfMatrix();
          MessageBoxA(nullptr, MatrixToString(D).c_str(), "Matrix", MB_OK);
     } */

     /* cout << D.RankOfMatrix() << endl;
     cout << B * B.GetInverseMatrix(); */
     // cout << (B & D & D) << endl;
     // cout << B << endl;
     /*A.push_back(r1);
     A.push_back(r2);
     A.push_back(r3); */
     // fill(A[0].begin(), A[0].end(), 0.0);
     // fill(A[1].begin(), A[1].end(), 0.0);
     /*  Matrix::value_type;
      Matrix::const_iterator;
      Matrix::iterator; */
     /* cout << B.RefLineMul(0, 0.5) << endl
          << endl;
     cout << A.RefRowEchelonMatrix() << endl
          << endl;
     cout << A.RefRowSimplestFormOfMatrix() << endl
          << endl; */
     // cout << A << endl;
     // cout << endl;
     /* cout << AssignValuesRandomly(6, 6) << endl
          << endl; */
     cout << AssignValuesRandomly(6, 6,-5,0) << endl;
     return 0;
}