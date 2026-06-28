#include <cmath>
#include <cstdio>
#include <vector>

#include "../src/planeGeometry/conic_intersect.hpp"

using namespace planeGeometry;

int main() {
  // Test 1: Circle x Circle
  {
    std::printf("Test 1: circle-circle 2 ints\n");
    Circle<double> c1(Point<double>(0.0, 0.0), 2.0);
    Circle<double> c2(Point<double>(3.0, 0.0), 2.0);
    auto res = conic_intersect<double>::compute(c1, c2);
    std::printf("  points size = %zu\n", res.points.size());
    for (const auto &p : res.points) {
      std::printf("  (%g, %g)\n", p.x(), p.y());
    }
  }
  std::printf("Done.\n");
  return 0;
}
