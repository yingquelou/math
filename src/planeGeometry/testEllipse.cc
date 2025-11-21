#include "Ellipse.h"
#include <iostream>
using std::cout;
int main(void)
{
    // Ellipse t;
    Point A(1, -1), B(1, 1);
    VectorOfMath p(A, 6.0, 2.0);
    cout << (B *= Point(2, 3)) << endl;
    // cout << "This point is " << Point(tan(M_PI / 3), 1).VectorMold() << endl;
    /* cout << "This point is " << Point(0, sqrt(2.0)).CoordinateInBase(Point(1 / sqrt(2.0), 1 / sqrt(2.0)), Point(-1 / sqrt(2.0), 1 / sqrt(2.0))) << endl;
    cout << Point(1, 1).CoordinateInBase(Point(0, 0), Point(0, 0)) << endl; */
    /* cout << "This point is " << A << B << endl;
    cout << "A||B? " << (A || B ? "true" : "false") << endl;
    cout << "This point is " << A << endl;
    --A-- --;
    cout << "This point is " << A << endl;
    cout << "A||B? " << (A || B ? "true" : "false") << endl;
    cout << "This point is " << A << endl;
    cout << "This point is " << A - B << endl; */
    // cout << A * B << endl;
    return 0;
}
