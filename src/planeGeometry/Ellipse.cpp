#include "Ellipse.h"
Ellipse::Ellipse(const Ellipse &E)
{
    xy = E.xy;
    x2 = E.x2;
    x1 = E.x1;
    y2 = E.y2;
    y1 = E.y1;
    c = E.c;
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