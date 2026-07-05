#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "Line.hpp"
#include "Point.hpp"

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
  using planeGeometry::Line;
  using planeGeometry::Point;

  {
    Line<double> l;
    CHECK_TRUE(!l.is_valid());
  }

  {
    Line<double> l(1.0, 2.0, 3.0);
    CHECK_TRUE(l.is_valid());
    CHECK_NEAR(l.A(), 1.0, 1e-12);
    CHECK_NEAR(l.B(), 2.0, 1e-12);
    CHECK_NEAR(l.C(), 3.0, 1e-12);
  }

  {
    Point<double> p1(1.0, 2.0);
    Point<double> p2(3.0, 4.0);
    Line<double> l = Line<double>::from_two_points(p1, p2);
    CHECK_TRUE(l.is_valid());
    Point<double> out;
    CHECK_TRUE(l.point_at_x(1.0, out));
    CHECK_NEAR(out.y(), 2.0, 1e-9);
    CHECK_TRUE(l.point_at_x(3.0, out));
    CHECK_NEAR(out.y(), 4.0, 1e-9);
  }

  {
    Point<double> p1(2.0, 1.0);
    Point<double> p2(2.0, 5.0);
    Line<double> l = Line<double>::from_two_points(p1, p2);
    CHECK_TRUE(l.is_valid());
    CHECK_TRUE(!l.point_at_x(2.0, p1));
    Point<double> out;
    CHECK_TRUE(l.point_at_y(1.0, out));
    CHECK_NEAR(out.x(), 2.0, 1e-9);
  }

  {
    Point<double> p(3.0, 4.0);
    Line<double> l = Line<double>::from_two_points(p, p);
    CHECK_TRUE(!l.is_valid());
  }

  {
    Point<double> p(0.0, 0.0);
    Line<double> l = Line<double>::from_point_slope(p, 1.0);
    CHECK_TRUE(l.is_valid());
    Point<double> out;
    CHECK_TRUE(l.point_at_x(5.0, out));
    CHECK_NEAR(out.y(), 5.0, 1e-9);
  }

  {
    Line<double> l = Line<double>::from_normal_polar(0.0, 5.0);
    CHECK_TRUE(l.is_valid());
    CHECK_NEAR(l.A(), 1.0, 1e-9);
    CHECK_NEAR(l.B(), 0.0, 1e-9);
    CHECK_NEAR(l.C(), -5.0, 1e-9);
  }

  {
    Point<double> p(1.0, 2.0);
    Point<double> dir(2.0, 3.0);
    Line<double> l = Line<double>::from_point_direction(p, dir);
    CHECK_TRUE(l.is_valid());
    Point<double> out;
    CHECK_TRUE(l.point_at_x(1.0, out));
    CHECK_NEAR(out.y(), 2.0, 1e-9);
  }

  {
    Point<double> p(1.0, 2.0);
    Point<double> z;
    Line<double> l = Line<double>::from_point_direction(p, z);
    CHECK_TRUE(!l.is_valid());
  }

  {
    Point<double> p(1.0, 2.0);
    Point<double> normal(1.0, 1.0);
    Line<double> l = Line<double>::from_point_normal(p, normal);
    CHECK_TRUE(l.is_valid());
    Point<double> out;
    CHECK_TRUE(l.point_at_x(1.0, out));
    CHECK_NEAR(out.y(), 2.0, 1e-9);
  }

  {
    Point<double> p(1.0, 2.0);
    Point<double> z;
    Line<double> l = Line<double>::from_point_normal(p, z);
    CHECK_TRUE(!l.is_valid());
  }

  {
    Line<double> l1(1.0, 2.0, 3.0);
    Line<double> l2(2.0, 4.0, 7.0);
    CHECK_TRUE(l1.is_parallel_to(l2));

    Line<double> l3(1.0, 2.0, 3.0);
    Line<double> l4(2.0, 3.0, 7.0);
    CHECK_TRUE(!l3.is_parallel_to(l4));
  }

  {
    Line<double> xaxis(0.0, 1.0, 0.0);
    Line<double> yaxis(1.0, 0.0, 0.0);
    Point<double> out;
    CHECK_TRUE(xaxis.intersection_with(yaxis, out));
    CHECK_NEAR(out.x(), 0.0, 1e-9);
    CHECK_NEAR(out.y(), 0.0, 1e-9);
  }

  {
    Line<double> l1(1.0, 0.0, -2.0);
    Line<double> l2(0.0, 1.0, -3.0);
    Point<double> out;
    CHECK_TRUE(l1.intersection_with(l2, out));
    CHECK_NEAR(out.x(), 2.0, 1e-9);
    CHECK_NEAR(out.y(), 3.0, 1e-9);
  }

  {
    Line<double> l1(1.0, 1.0, 0.0);
    Line<double> l2(2.0, 2.0, 5.0);
    Point<double> out;
    CHECK_TRUE(!l1.intersection_with(l2, out));
  }

  {
    Line<double> l(1.0, 2.0, 3.0);
    Point<double> out;
    CHECK_TRUE(l.point_at_x(1.0, out));
    CHECK_NEAR(out.y(), -2.0, 1e-9);
  }

  {
    Line<double> l(1.0, 2.0, 3.0);
    Point<double> out;
    CHECK_TRUE(l.point_at_y(-1.5, out));
    CHECK_NEAR(out.x(), 0.0, 1e-9);
  }

  {
    Line<double> l(1.0, 0.0, -2.0);
    Point<double> out;
    CHECK_TRUE(!l.point_at_x(0.0, out));
  }

  {
    Line<double> y_eq_x(1.0, -1.0, 0.0);
    Point<double> dir = y_eq_x.direction_vector();
    CHECK_NEAR(dir.x(), 1.0, 1e-9);
    CHECK_NEAR(dir.y(), 1.0, 1e-9);
  }

  {
    Line<double> l(2.0, 3.0, 4.0);
    Point<double> n = l.normal_vector();
    CHECK_NEAR(n.x(), 2.0, 1e-9);
    CHECK_NEAR(n.y(), 3.0, 1e-9);
  }

  {
    Line<double> xaxis(0.0, 1.0, 0.0);
    Line<double> yaxis(1.0, 0.0, 0.0);
    CHECK_NEAR(xaxis.angle_to(yaxis), M_PI / 2.0, 1e-9);
  }

  {
    Line<double> l1(1.0, -1.0, 0.0);
    Line<double> l2(1.0, 1.0, 0.0);
    CHECK_NEAR(l1.angle_to(l2), M_PI / 2.0, 1e-9);
  }

  {
    Line<double> xaxis(0.0, 1.0, 0.0);
    Point<double> p(3.0, 4.0);
    CHECK_NEAR(xaxis.distance_to(p), 4.0, 1e-9);
  }

  {
    Line<double> y_eq_x(1.0, -1.0, 0.0);
    Point<double> p(1.0, 2.0);
    CHECK_NEAR(y_eq_x.distance_to(p), 1.0 / std::sqrt(2.0), 1e-9);
  }

  {
    Line<double> l1(0.0, 1.0, 0.0);
    Line<double> l2(0.0, 1.0, -2.0);
    CHECK_NEAR(l1.distance_to(l2), 2.0, 1e-9);
  }

  {
    Line<double> l1(1.0, -1.0, 0.0);
    Line<double> l2(1.0, 1.0, 0.0);
    CHECK_NEAR(l1.distance_to(l2), 0.0, 1e-9);
  }

  {
    Line<double> l1(2.0, 0.0, -4.0);
    Line<double> l2(2.0, 0.0, -10.0);
    CHECK_NEAR(l1.distance_to(l2), 3.0, 1e-9);
  }

  {
    Line<double> l(1.5, -2.25, 3.0);
    std::ostringstream oss;
    oss << l;
    CHECK_TRUE(oss.str() == "{1.5,-2.25,3}");
  }

  {
    Line<double> l(1.0, 0.0, 0.0);
    Point<double> p(2.0, 3.0);
    Point<double> foot = l.foot_of_perpendicular(p);
    CHECK_NEAR(foot.x(), 0.0, 1e-12);
    CHECK_NEAR(foot.y(), 3.0, 1e-12);
    Point<double> ref = l.reflected_point(p);
    CHECK_NEAR(ref.x(), -2.0, 1e-12);
    CHECK_NEAR(ref.y(), 3.0, 1e-12);
  }

  {
    Line<double> l1(1.0, 0.0, 0.0);
    Line<double> l2(0.0, 1.0, 0.0);
    Line<double> b1, b2;
    CHECK_TRUE(l1.angle_bisector(l2, b1, b2));
    Point<double> p1(1.0, 1.0);
    Point<double> p2(1.0, -1.0);
    double d1 = b1.distance_to(p1);
    double d2 = b2.distance_to(p1);
    double d3 = b1.distance_to(p2);
    double d4 = b2.distance_to(p2);
    CHECK_TRUE((d1 < 1e-9 && d4 < 1e-9) || (d2 < 1e-9 && d3 < 1e-9));
  }

  if (g_failures == 0) {
    std::cout << "All Line tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Line test(s) failed." << std::endl;
  return 1;
}
