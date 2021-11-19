#include "Line.h"
#include "Line.cpp"
using namespace std;
int main(void)
{
    Line L1{{1, 2}, VectorOfMath(1, 2), 1},
        L2{{0, 0}, {2, 4}, 1},
        L3{1.0, -1.0, 0.0},
        L4{0.0, 1.0, 0.0},
        L5({0, 1.0}, 1.0),
        L6(M_PI / 3, 1);
    Point P;
    // L1.KnowX(-1.0, P);
    cout << L1.CrossoverPoint(L2, P) << endl
         << P << endl
         << L1 << endl
         << L2 << endl
         << L3 << endl
         << L4 << endl
         << L5 << endl
         << L6 << endl
         << (Line(0, 0, 0) || Line(0, 0, 0)) << endl;
    printf("%.20lf\n", L3.DistanceOfParallelLines(L5));
    printf("%.20lf\n", L3.AngleOfLine(L4));
    printf("%.100lf\n", pow(1 / sqrt(2),2));
    return 0;
}