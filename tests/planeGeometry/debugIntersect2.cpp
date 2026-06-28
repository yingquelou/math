#include <cmath>
#include <cstdio>
#include <vector>

#include "../src/planeGeometry/conic_intersect.hpp"

using namespace planeGeometry;

int main() {
  {
    Circle<double> c1(Point<double>(0.0, 0.0), 2.0);
    Circle<double> c2(Point<double>(3.0, 0.0), 2.0);
    auto res = conic_intersect<double>::compute(c1, c2);
    std::printf("Circle-Circle points size = %zu\n", res.points.size());
    for (const auto &p : res.points) {
      std::printf("  (%g, %g)\n", p.x(), p.y());
    }
  }
  {
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    Ellipse<double> ell(Point<double>(0.0, 0.0), 2.0, 1.0, 0.0);
    auto res = conic_intersect<double>::compute(circ, ell);
    std::printf("Circle-Ellipse points size = %zu\n", res.points.size());
    for (const auto &p : res.points) {
      std::printf("  (%g, %g)\n", p.x(), p.y());
    }
  }
  {
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    Parabola<double> pb(Point<double>(0.0, 0.0), 0.5, 0.0);
    auto res = conic_intersect<double>::compute(circ, pb);
    std::printf("Circle-Parabola points size = %zu\n", res.points.size());
    for (const auto &p : res.points) {
      std::printf("  (%g, %g)\n", p.x(), p.y());
    }
  }
  return 0;
}
