#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Hyperbola.hpp"
#include "Line.hpp"
#include "Point.hpp"

static int g_failures = 0;

#define CHECK_TRUE(expr)                                                   \
  do {                                                                    \
    if (!(expr)) {                                                        \
      std::cerr << "FAIL: " #expr << " at " << __FILE__ << ":" << __LINE__ \
                << std::endl;                                             \
      ++g_failures;                                                       \
    }                                                                     \
  } while (0)

#define CHECK_NEAR(a, b, tol)                                             \
  do {                                                                    \
    if (std::abs((a) - (b)) > (tol)) {                                    \
      std::cerr << "FAIL: " #a " near " #b " at " << __FILE__ << ":"      \
                << __LINE__ << " (" << (a) << " vs " << (b) << ")"         \
                << std::endl;                                             \
      ++g_failures;                                                       \
    }                                                                     \
  } while (0)

int main() {
  using planeGeometry::Hyperbola;
  using planeGeometry::Line;
  using planeGeometry::Point;

  // 1) Default construction: unit hyperbola x^2 - y^2 = 1
  {
    Hyperbola<double> h;
    CHECK_NEAR(h.coef_x2(), 1.0, 1e-12);
    CHECK_NEAR(h.coef_y2(), -1.0, 1e-12);
    CHECK_NEAR(h.coef_xy(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_x1(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_y1(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_c(), -1.0, 1e-12);
    CHECK_NEAR(h.real_semi_axis(), 1.0, 1e-12);
    CHECK_NEAR(h.imag_semi_axis(), 1.0, 1e-12);
  }

  // 2) Hyperbola(Point(0,0), 3.0, 2.0): x^2/9 - y^2/4 = 1
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    CHECK_NEAR(h.center().x(), 0.0, 1e-9);
    CHECK_NEAR(h.center().y(), 0.0, 1e-9);
    CHECK_NEAR(h.real_semi_axis(), 3.0, 1e-9);
    CHECK_NEAR(h.imag_semi_axis(), 2.0, 1e-9);

    CHECK_NEAR(h.coef_x2(), 1.0 / 9.0, 1e-12);
    CHECK_NEAR(h.coef_y2(), -1.0 / 4.0, 1e-12);
    CHECK_NEAR(h.coef_xy(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_x1(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_y1(), 0.0, 1e-12);
    CHECK_NEAR(h.coef_c(), -1.0, 1e-12);

    const double expected_e = std::sqrt(13.0) / 3.0;
    CHECK_NEAR(h.eccentricity(), expected_e, 1e-9);
    CHECK_NEAR(h.focal_distance(), std::sqrt(13.0), 1e-9);
  }

  // 3) Focal points: (+-sqrt(13), 0)
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    const auto f = h.focal_points();
    const double c = std::sqrt(13.0);
    CHECK_NEAR(f.first.x(), c, 1e-9);
    CHECK_NEAR(f.first.y(), 0.0, 1e-9);
    CHECK_NEAR(f.second.x(), -c, 1e-9);
    CHECK_NEAR(f.second.y(), 0.0, 1e-9);
  }

  // 4) Asymptotes direction for non-rotated case
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    const auto asym = h.asymptotes();
    CHECK_NEAR(asym.first.A(), -2.0 / 3.0, 1e-12);
    CHECK_NEAR(asym.first.B(), 1.0, 1e-12);
    CHECK_NEAR(asym.first.C(), 0.0, 1e-12);
    CHECK_NEAR(asym.second.A(), 2.0 / 3.0, 1e-12);
    CHECK_NEAR(asym.second.B(), 1.0, 1e-12);
    CHECK_NEAR(asym.second.C(), 0.0, 1e-12);
  }

  // 5) contains
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    const int rp_40 = h.relative_position(Point<double>(4.0, 0.0));
    const int rp_00 = h.relative_position(Point<double>(0.0, 0.0));
    CHECK_TRUE(rp_40 == 1);
    CHECK_TRUE(rp_00 == -1);
    CHECK_TRUE(h.contains(Point<double>(4.0, 0.0)));
    CHECK_TRUE(!h.contains(Point<double>(0.0, 0.0)));
  }

  // 6) Rotation by 45 degrees
  {
    const double pi = std::acos(-1.0);
    Hyperbola<double> h(Point<double>(1.0, 2.0), 3.0, 2.0, pi / 4.0);
    std::cout << "  Rotated hyperbola coefficients: "
              << "a=" << h.coef_x2() << " b=" << h.coef_y2()
              << " c=" << h.coef_xy() << " d=" << h.coef_x1()
              << " e=" << h.coef_y1() << " f=" << h.coef_c() << std::endl;
    std::cout << "  evaluate(center)=" << h.evaluate(h.center()) << std::endl;
    std::cout << "  real_a_=" << h.real_semi_axis()
              << " imag_b_=" << h.imag_semi_axis()
              << " rotation=" << h.rotation_angle() << std::endl;

    CHECK_NEAR(h.center().x(), 1.0, 1e-9);
    CHECK_NEAR(h.center().y(), 2.0, 1e-9);
    CHECK_NEAR(h.real_semi_axis(), 3.0, 1e-9);
    CHECK_NEAR(h.imag_semi_axis(), 2.0, 1e-9);
    CHECK_NEAR(h.rotation_angle(), pi / 4.0, 1e-9);

    CHECK_NEAR(h.evaluate(h.center()), -1.0, 1e-9);

    const auto f = h.focal_points();
    const double c = std::sqrt(13.0);
    const double cc = std::cos(pi / 4.0);
    const double ss = std::sin(pi / 4.0);
    CHECK_NEAR(f.first.x(), 1.0 + c * cc, 1e-9);
    CHECK_NEAR(f.first.y(), 2.0 + c * ss, 1e-9);
    CHECK_NEAR(f.second.x(), 1.0 - c * cc, 1e-9);
    CHECK_NEAR(f.second.y(), 2.0 - c * ss, 1e-9);
  }

  // 7) from_general round-trip
  {
    Hyperbola<double> src(Point<double>(0.0, 0.0), 3.0, 2.0);
    Hyperbola<double> rt = Hyperbola<double>::from_general(
        src.coef_x2(), src.coef_y2(), src.coef_xy(), src.coef_x1(),
        src.coef_y1(), src.coef_c());
    CHECK_NEAR(rt.center().x(), 0.0, 1e-9);
    CHECK_NEAR(rt.center().y(), 0.0, 1e-9);
    CHECK_NEAR(rt.real_semi_axis(), 3.0, 1e-9);
    CHECK_NEAR(rt.imag_semi_axis(), 2.0, 1e-9);
    CHECK_NEAR(rt.eccentricity(), std::sqrt(13.0) / 3.0, 1e-9);
  }

  // 8) from_general validation
  {
    bool threw = false;
    try {
      Hyperbola<double>::from_general(1.0 / 9.0, 1.0 / 4.0, 0.0, 0.0, 0.0,
                                      -1.0);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  // 9) Axis swap invariant
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 2.0, 3.0);
    CHECK_TRUE(h.real_semi_axis() >= h.imag_semi_axis());
    CHECK_NEAR(h.real_semi_axis(), 3.0, 1e-9);
    CHECK_NEAR(h.imag_semi_axis(), 2.0, 1e-9);
  }

  // 10) Directrices for non-rotated hyperbola at origin
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    const auto d = h.directrices();
    const double c = std::sqrt(13.0);
    const double dval = 9.0 / c;
    CHECK_NEAR(d.first.A(), 1.0, 1e-12);
    CHECK_NEAR(d.first.B(), 0.0, 1e-12);
    CHECK_NEAR(d.first.C(), dval, 1e-9);
    CHECK_NEAR(d.second.A(), 1.0, 1e-12);
    CHECK_NEAR(d.second.B(), 0.0, 1e-12);
    CHECK_NEAR(d.second.C(), -dval, 1e-9);
  }

  // 11) Stream operator
  {
    Hyperbola<double> h(Point<double>(0.0, 0.0), 3.0, 2.0);
    std::ostringstream oss;
    oss << h;
    const std::string s = oss.str();
    CHECK_TRUE(s.rfind("Hyperbola", 0) == 0);
  }

  if (g_failures == 0) {
    std::cout << "All Hyperbola tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Hyperbola test(s) failed." << std::endl;
  return 1;
}
