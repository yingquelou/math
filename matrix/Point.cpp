#pragma once
#include "Point.h"
Point::Point() : x(0), y(0) {}
Point::Point(const double &a, const double &b) : x(a), y(b) {}
Point::Point(const Point &P) : x(P.x), y(P.y) {}
Point::Point(const Point &P, const double &a, const double &b)
{
    x = a * P.x;
    y = b * P.y;
}
Point Point::operator-() const
{
    return Point(-x, -y);
}
Point &Point::operator*=(const Point &P)
{
    x *= P.x;
    y *= P.y;
    return *this;
}
Point Point::operator*(const double &b) const
{
    return Point(b * x, b * y);
}
Point operator*(const double &b, const Point &P)
{
    return Point(b * P.x, b * P.y);
}
double Point::AngleOfVector(const Point &P) const
{
    double tmp = *this * P / (this->VectorMold() * P.VectorMold());
    return acos(tmp);
}
Point Point::CoordinateInBase(const Point &ex, const Point &ey) const
{
    Point P1_Component = this->VectorComponent(ex);
    Point P2_Component = this->VectorComponent(ey);
    double tx = P1_Component.VectorMold() / ex.VectorMold();
    double ty = P2_Component.VectorMold() / ey.VectorMold();
    if (*this * ex < 0)
        tx = -tx;
    if (*this * ey < 0)
        ty = -ty;
    return Point(tx, ty);
}
Point Point::VectorComponent(const Point &P) const
{
    double P_Mold = P.VectorMold();
    if (!P_Mold)
    //此时P为零向量，参数非法
    {
        cerr << "errer:zero Vector" << endl;
        exit(EXIT_FAILURE);
    }
    double proportion = ((*this * P) / P_Mold) / P_Mold;
    return P * proportion;
}
double Point::VectorMold() const
{
    return sqrt(x * x + y * y);
}
// #undef allowableError
bool Point::operator==(const Point &P) const
{
#ifdef allowableError
    if (abs(x - P.x) < allowableError && abs(y - P.y) < allowableError)
#else
    if (x == P.x && y == P.y)
#endif
        return true;
    return false;
}
bool Point::operator||(const Point &P) const
{
#ifdef allowableError
    if (x * P.y - y * P.x < allowableError)
#else
    if (x * P.y == y * P.x)
#endif
        return true;
    return false;
}
Point Point::operator+(const Point &P) const
{
    return Point(x + P.x, y + P.y);
}
Point Point::operator-(const Point &P) const
{
    return Point(x - P.x, y - P.y);
}
Point &Point::operator++()
{
    ++x;
    return *this;
}
Point &Point::operator++(int)
{
    ++y;
    return *this;
}
Point &Point::operator--()
{
    --x;
    return *this;
}
Point &Point::operator--(int)
{
    --y;
    return *this;
}
double Point::operator*(const Point &P) const
{
    return x * P.x + y * P.y;
}
ostream &operator<<(ostream &cout, const Point &P)
{
    cout << '(' << P.x << ',' << P.y << ')';
    return cout;
}
Point &Point::operator=(const Point &b)
{
    x = b.x;
    y = b.y;
    return *this;
}