#include "Line.hpp"
#include "Point.hpp"
#include <iostream>

int main() {
  using planeGeometry::Line;
  using planeGeometry::Point;
  Line<double> l(1.0, 0.0, 0.0);
  Point<double> p(0.0, 2.0);
  Point<double> foot = l.foot_of_perpendicular(p);
  std::cout << "foot.x() = " << foot.x() << std::endl;
  std::cout << "foot.y() = " << foot.y() << std::endl;
  return 0;
}
