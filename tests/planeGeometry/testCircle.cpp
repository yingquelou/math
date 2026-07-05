#include "Circle.hpp"
#include <cmath>
#include <iostream>

static int g_failures = 0;
#define CHECK_TRUE(expr)                                                       \
  do {                                                                         \
    if (!(expr)) {                                                             \
      std::cerr << "FAIL: " #expr << " at " << __FILE__ << ":" << __LINE__     \
                << std::endl;                                                  \
      ++g_failures;                                                            \
    }                                                                          \
  } while (0)
#define CHECK_NEAR(a, b, tol)                                                  \
  do {                                                                         \
    if (std::abs((a) - (b)) > (tol)) {                                         \
      std::cerr << "FAIL: " #a " near " #b " at " << __FILE__ << ":"           \
                << __LINE__ << " (" << (a) << " vs " << (b) << ")"             \
                << std::endl;                                                  \
      ++g_failures;                                                            \
    }                                                                          \
  } while (0)

int main() {
  using namespace planeGeometry;

  {
    Circle<double> c(Point<double>(1.0, 2.0), 3.0);
    CHECK_NEAR(c.center().x(), 1.0, 1e-12);
    CHECK_NEAR(c.center().y(), 2.0, 1e-12);
    CHECK_NEAR(c.radius(), 3.0, 1e-12);
    CHECK_NEAR(c.area(), M_PI * 9.0, 1e-9);
    CHECK_NEAR(c.circumference(), 6.0 * M_PI, 1e-9);
    CHECK_NEAR(c.eccentricity(), 0.0, 1e-12);
  }

  {
    Circle<double> c(0.0, 0.0, 5.0);
    CHECK_NEAR(c.center().x(), 0.0, 1e-12);
    CHECK_NEAR(c.center().y(), 0.0, 1e-12);
  }

  {
    Circle<double> c(Point<double>(0.0, 0.0), 2.0);
    CHECK_TRUE(c.on_boundary(Point<double>(2.0, 0.0)));
    CHECK_TRUE(!c.contains(Point<double>(3.0, 0.0)));
    CHECK_TRUE(c.contains(Point<double>(0.0, 0.0)));
  }

  {
    bool threw = false;
    try {
      Circle<double>::from_general(1.0, 2.0, 0.0, 0.0, 0.0, -1.0);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  {
    Circle<double> c(Point<double>(1.0, 1.0), 2.0);
    auto fps = c.focal_points();
    CHECK_NEAR(fps.first.x(), 1.0, 1e-12);
    CHECK_NEAR(fps.first.y(), 1.0, 1e-12);
    CHECK_NEAR(fps.second.x(), 1.0, 1e-12);
    CHECK_NEAR(fps.second.y(), 1.0, 1e-12);
  }

  if (g_failures == 0) {
    std::cout << "All Circle tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Circle test(s) failed." << std::endl;
  return 1;
}
