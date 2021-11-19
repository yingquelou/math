#pragma once
#include "Ellipse.h"
Ellipse::Ellipse(void)
{
    xy = 0.0;
    x2 = 0.0;
    x1 = 0.0;
    y2 = 0.0;
    y1 = 0.0;
    c = 0.0;
}
Ellipse::Ellipse(const Ellipse &E)
{
    xy = E.xy;
    x2 = E.x2;
    x1 = E.x1;
    y2 = E.y2;
    y1 = E.y1;
    c = E.c;
}
Ellipse::Ellipse(const double &, const double &)
{
}
Ellipse::Ellipse(const Point &, const double &)
{
}
Ellipse::Ellipse(const Point &, const Point &, const double)
{
}
Ellipse &Ellipse::operator=(const Ellipse &E)
{
    xy = E.xy;
    x2 = E.x2;
    x1 = E.x1;
    y2 = E.y2;
    y1 = E.y1;
    c = E.c;
    return *this;
}