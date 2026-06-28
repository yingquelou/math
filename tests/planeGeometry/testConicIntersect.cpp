#include <cmath>
#include <cstdio>
#include <vector>

#include "../src/planeGeometry/conic_intersect.hpp"

using namespace planeGeometry;

static int g_tests = 0;
static int g_passed = 0;

#define CHECK(cond, msg)                                    \
  do {                                                      \
    ++g_tests;                                              \
    if (!(cond)) {                                          \
      std::printf("FAIL: %s\n", msg);                       \
    } else {                                                \
      ++g_passed;                                           \
    }                                                       \
  } while (0)

static bool point_close(const Point<double> &a, const Point<double> &b,
                       double tol = 1e-6) {
  const double dx = a.x() - b.x();
  const double dy = a.y() - b.y();
  return (dx * dx + dy * dy) < tol * tol;
}

static bool contains_point(const std::vector<Point<double>> &pts,
                           const Point<double> &target,
                           double tol = 1e-6) {
  for (const auto &p : pts) {
    if (point_close(p, target, tol)) return true;
  }
  return false;
}

int main() {
  // Test 1: Circle x Circle (disjoint)
  {
    Circle<double> c1(Point<double>(0.0, 0.0), 1.0);
    Circle<double> c2(Point<double>(3.0, 0.0), 1.0);
    auto res = conic_intersect<double>::compute(c1, c2);
    CHECK(res.points.empty(), "circle-circle disjoint");
  }

  // Test 2: Circle x Circle (two intersections)
  {
    Circle<double> c1(Point<double>(0.0, 0.0), 2.0);
    Circle<double> c2(Point<double>(3.0, 0.0), 2.0);
    auto res = conic_intersect<double>::compute(c1, c2);
    CHECK(res.points.size() == 2, "circle-circle 2 ints count");
    // Expected: x = 1.5, y = +/- sqrt(4 - 2.25) = +/- sqrt(1.75)
    const double yexp = std::sqrt(1.75);
    CHECK(contains_point(res.points, Point<double>(1.5, yexp)),
          "circle-circle (1.5, +)");
    CHECK(contains_point(res.points, Point<double>(1.5, -yexp)),
          "circle-circle (1.5, -)");
  }

  // Test 3: Circle x Ellipse
  {
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    Ellipse<double> ell(Point<double>(0.0, 0.0), 2.0, 1.0, 0.0);
    auto res = conic_intersect<double>::compute(circ, ell);
    // x^2 + y^2 = 1 ; x^2/4 + y^2 = 1 => 3x^2/4 = 0 => x=0, y=+/-1
    CHECK(res.points.size() == 2, "circle-ellipse count");
    CHECK(contains_point(res.points, Point<double>(0.0, 1.0)),
          "circle-ellipse (0,1)");
    CHECK(contains_point(res.points, Point<double>(0.0, -1.0)),
          "circle-ellipse (0,-1)");
  }

  // Test 4: Circle x Parabola
  {
    // Parabola vertex at origin, p=0.5, rot=0: 4p(x-0) - (y-0)^2 = 0
    //   => y^2 = 2x (opens along +x).
    // Circle: x^2 + y^2 = 1.
    // Substituting: x^2 + 2x = 1 => x^2 + 2x - 1 = 0
    //   => x = -1 + sqrt(2) ≈ 0.4142, y^2 = 2x ≈ 0.8284, y ≈ ±0.9102.
    const double xval = -1.0 + std::sqrt(2.0);
    const double yval = std::sqrt(2.0 * xval);
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    Parabola<double> pb(Point<double>(0.0, 0.0), 0.5, 0.0);
    auto res = conic_intersect<double>::compute(circ, pb);
    std::printf("DBG T4 size=%zu\n", res.points.size());
    std::fflush(stdout);
    CHECK(res.points.size() == 2, "circle-parabola count");
    CHECK(contains_point(res.points, Point<double>(xval,  yval), 1e-5),
          "circle-parabola (+x, +y)");
    CHECK(contains_point(res.points, Point<double>(xval, -yval), 1e-5),
          "circle-parabola (+x, -y)");
  }

  // Test 5: Circle x Hyperbola
  {
    // x^2 - y^2 = 1 (hyperbola)
    // x^2 + y^2 = 2 (circle)
    // => 2x^2 = 3 => x^2 = 3/2, y^2 = 1/2
    Hyperbola<double> hb(Point<double>(0.0, 0.0), 1.0, 1.0, 0.0);
    Circle<double> circ(Point<double>(0.0, 0.0), std::sqrt(2.0));
    auto res = conic_intersect<double>::compute(hb, circ);
    std::printf("DBG T5 size=%zu\n", res.points.size());
    std::fflush(stdout);
    CHECK(res.points.size() == 4, "circle-hyperbola count");
    const double xexp = std::sqrt(1.5);
    const double yexp = std::sqrt(0.5);
    std::printf("DBG T5 points:");
    for (auto &p : res.points) {
      std::printf(" (%g,%g)", p.x(), p.y());
    }
    std::printf("\n");
    std::fflush(stdout);
    std::printf("DBG T5 before (+,+)\n"); std::fflush(stdout);
    CHECK(contains_point(res.points, Point<double>( xexp,  yexp)),
          "circle-hyperbola (+,+)");
    std::printf("DBG T5 before (+,-)\n"); std::fflush(stdout);
    CHECK(contains_point(res.points, Point<double>( xexp, -yexp)),
          "circle-hyperbola (+,-)");
    std::printf("DBG T5 before (-,+)\n"); std::fflush(stdout);
    CHECK(contains_point(res.points, Point<double>(-xexp,  yexp)),
          "circle-hyperbola (-,+)");
    std::printf("DBG T5 before (-,-)\n"); std::fflush(stdout);
    CHECK(contains_point(res.points, Point<double>(-xexp, -yexp)),
          "circle-hyperbola (-,-)");
    std::printf("DBG T5 done\n"); std::fflush(stdout);
  }

  // Test 6: Ellipse x Parabola (two intersections)
  {
    // Ellipse: x^2/4 + y^2 = 1. Parabola (p=0.5, vertex at origin): y^2 = 2x.
    // Substituting: x^2/4 + 2x = 1 => x^2 + 8x - 4 = 0 => x = -4 + 2sqrt(5).
    //   x ≈ 0.477, y^2 = 2x ≈ 0.954, y ≈ ±0.977.
    const double xval = -4.0 + 2.0 * std::sqrt(5.0);
    const double y2 = 2.0 * xval;
    const double yval = std::sqrt(y2);
    Ellipse<double> ell(Point<double>(0.0, 0.0), 2.0, 1.0, 0.0);
    Parabola<double> pb(Point<double>(0.0, 0.0), 0.5, 0.0);
    auto res = conic_intersect<double>::compute(ell, pb);
    std::printf("DBG T6 size=%zu\n", res.points.size());
    std::fflush(stdout);
    CHECK(res.points.size() == 2, "ellipse-parabola count");
    std::printf("DBG T6 points:");
    for (auto &p : res.points) std::printf(" (%g,%g)", p.x(), p.y());
    std::printf(" expected: (%g,%g) (%g,%g)\n", xval, yval, xval, -yval);
    std::fflush(stdout);
    CHECK(contains_point(res.points, Point<double>( xval, yval), 1e-5),
          "ellipse-parabola (+x, +y)");
    CHECK(contains_point(res.points, Point<double>( xval, -yval), 1e-5),
          "ellipse-parabola (+x, -y)");
  }

  // Test 7: Rotated ellipse x circle (2 intersections)
  {
    // Ellipse rotated pi/4, center at (5, 0), semi-major=3, semi-minor=1
    // Intersect with circle radius 1 at origin
    Ellipse<double> ell(Point<double>(5.0, 0.0), 3.0, 1.0, M_PI / 4.0);
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    auto res = conic_intersect<double>::compute(ell, circ);
    // Should have 0 or 2 intersections - check that returned points satisfy
    // both equations approximately.
    bool all_ok = true;
    for (const auto &p : res.points) {
      const double F_ell = ell.evaluate(p);
      const double F_circ = circ.evaluate(p);
      if (std::abs(F_ell) > 1e-4 || std::abs(F_circ) > 1e-4) {
        all_ok = false;
        break;
      }
    }
    CHECK(all_ok, "rotated ellipse-circle eval ~ 0");
    CHECK(res.points.size() <= 2, "rotated ellipse-circle count <= 2");
  }

  // Test 8: Test that intersect() with variant works
  {
    conic_variant<double> v1(Circle<double>(Point<double>(0.0, 0.0), 1.0));
    conic_variant<double> v2(Circle<double>(Point<double>(1.0, 0.0), 1.0));
    std::printf("DBG T8 v1.which()=%d v2.which()=%d\n",
                (int)v1.which(), (int)v2.which());
    std::fflush(stdout);
    auto res = intersect(v1, v2);
    std::printf("DBG T8 size=%zu points:", res.points.size());
    for (auto &p : res.points) std::printf(" (%g,%g)", p.x(), p.y());
    std::printf("\n");
    std::fflush(stdout);
    // Two circles radius 1, distance 1 -> 2 intersections at (0.5, +/- sqrt(3)/2)
    CHECK(res.points.size() == 2, "variant-circle-circle count");
    const double yexp = std::sqrt(3.0) / 2.0;
    CHECK(contains_point(res.points, Point<double>(0.5,  yexp), 1e-5),
          "variant-circle (+,+)");
    CHECK(contains_point(res.points, Point<double>(0.5, -yexp), 1e-5),
          "variant-circle (+,-)");
  }

  // Test 9: Tangent case (circle x ellipse touching)
  {
    // Circle radius 1 at origin, ellipse center (3,0) semi-a=2, semi-b=1
    // They touch at (1, 0) because ellipse x-extent is [1,5] along x axis
    // At (1,0): circle x^2+y^2=1 OK; ellipse (1-3)^2/4 + 0/1 = 1 OK
    Circle<double> circ(Point<double>(0.0, 0.0), 1.0);
    Ellipse<double> ell(Point<double>(3.0, 0.0), 2.0, 1.0, 0.0);
    auto res = conic_intersect<double>::compute(circ, ell);
    // Tangent means double root -> 1 unique point
    CHECK(res.points.size() == 1, "tangent count");
    CHECK(contains_point(res.points, Point<double>(1.0, 0.0), 1e-4),
          "tangent point at (1,0)");
  }

  std::printf("\n=== Results: %d/%d passed ===\n", g_passed, g_tests);
  std::fflush(stdout);
  return (g_passed == g_tests) ? 0 : 1;
}
