#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Parabola.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

  // Test 1: default unit parabola (y^2 = 4x) — focus (1,0), directrix x=-1.
  {
    Parabola<double> p;
    const auto f = p.focus();
    const auto d = p.directrix();
    CHECK_NEAR(p.vertex().x(), 0.0, 1e-12);
    CHECK_NEAR(p.vertex().y(), 0.0, 1e-12);
    CHECK_NEAR(p.focal_parameter(), 1.0, 1e-12);
    CHECK_NEAR(f.x(), 1.0, 1e-12);
    CHECK_NEAR(f.y(), 0.0, 1e-12);
    CHECK_NEAR(d.A(), 1.0, 1e-12); // directrix normal x-component
    CHECK_NEAR(d.B(), 0.0, 1e-12); // directrix normal y-component
    CHECK_NEAR(d.C(), 1.0, 1e-12); // line: 1*x + 0*y + 1 = 0, i.e. x=-1
    CHECK_NEAR(p.eccentricity(), 1.0, 1e-12);
  }

  // Test 2: Parabola(Point(0,0), 2.0) — focus (2,0), directrix x=-2.
  {
    Parabola<double> p(Point<double>(0.0, 0.0), 2.0);
    const auto f = p.focus();
    const auto d = p.directrix();
    CHECK_NEAR(p.vertex().x(), 0.0, 1e-12);
    CHECK_NEAR(p.vertex().y(), 0.0, 1e-12);
    CHECK_NEAR(p.focal_parameter(), 2.0, 1e-12);
    CHECK_NEAR(f.x(), 2.0, 1e-12);
    CHECK_NEAR(f.y(), 0.0, 1e-12);
    CHECK_NEAR(d.A(), 1.0, 1e-12);
    CHECK_NEAR(d.B(), 0.0, 1e-12);
    CHECK_NEAR(d.C(), 2.0, 1e-12);
    CHECK_NEAR(p.latus_rectum_length(), 8.0, 1e-12);
  }

  // Test 3: eccentricity is always 1.
  {
    Parabola<double> p1;
    Parabola<double> p2(Point<double>(3.0, -1.0), 2.5);
    Parabola<double> p3(Point<double>(0.0, 0.0), 1.0, M_PI / 3.0);
    CHECK_NEAR(p1.eccentricity(), 1.0, 1e-12);
    CHECK_NEAR(p2.eccentricity(), 1.0, 1e-12);
    CHECK_NEAR(p3.eccentricity(), 1.0, 1e-12);
  }

  // Test 4: latus_rectum_length == 4 * p.
  {
    Parabola<double> p(Point<double>(-5.0, 7.0), 1.5);
    CHECK_NEAR(p.latus_rectum_length(), 4.0 * 1.5, 1e-12);
  }

  // Test 5: axis() is horizontal (parallel to x-axis) for zero rotation.
  {
    Parabola<double> p;
    const auto ax = p.axis();
    // Axis direction is (-sin(0), cos(0)) = (0, 1) — vertical line through
    // the vertex, i.e. x = 0.
    CHECK_NEAR(ax.A(), 0.0, 1e-12);
    CHECK_NEAR(ax.B(), 1.0, 1e-12);
    CHECK_NEAR(ax.C(), 0.0, 1e-12);
  }

  // Test 6: vertex form round-trip via from_general.
  {
    Parabola<double> p1(Point<double>(3.0, -1.0), 2.5);
    Parabola<double> p2 =
        Parabola<double>::from_general(p1.coef_x2(), p1.coef_y2(), p1.coef_xy(),
                                       p1.coef_x1(), p1.coef_y1(), p1.coef_c());
    CHECK_NEAR(p2.vertex().x(), p1.vertex().x(), 1e-9);
    CHECK_NEAR(p2.vertex().y(), p1.vertex().y(), 1e-9);
    CHECK_NEAR(p2.focal_parameter(), p1.focal_parameter(), 1e-9);
    const auto f1 = p1.focus();
    const auto f2 = p2.focus();
    CHECK_NEAR(f1.x(), f2.x(), 1e-9);
    CHECK_NEAR(f1.y(), f2.y(), 1e-9);
  }

  // Test 7: from_general on a true parabola works; non-parabola throws.
  {
    // Unit parabola y^2 = 4x: coefficients a=0, b=-1, c=0, d=4, e=0, f=0.
    bool threw = false;
    try {
      Parabola<double>::from_general(0.0, -1.0, 0.0, 4.0, 0.0, 0.0);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(!threw);

    // Ellipse (delta < 0) must throw.
    threw = false;
    try {
      Parabola<double>::from_general(1.0, 1.0, 0.0, 0.0, 0.0, -1.0);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);

    // Degenerate: parallel lines (x - y + 1)^2 = x^2 - 2xy + y^2 + 2x - 2y + 1.
    threw = false;
    try {
      Parabola<double>::from_general(1.0, 1.0, -2.0, 2.0, -2.0, 1.0);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  // Test 8: relative_position / contains semantics — point on focal side
  // returns +1 / true, point on other side returns -1 / false, point on
  // boundary returns 0 / false.
  {
    Parabola<double> p;                              // y^2 = 4x, focus at (1,0)
    CHECK_TRUE(p.contains(Point<double>(0.5, 0.0))); // inside
    CHECK_TRUE(!p.contains(Point<double>(-1.0, 0.0)));   // outside
    CHECK_TRUE(!p.contains(Point<double>(0.0, 0.0)));    // on boundary
    CHECK_TRUE(p.on_boundary(Point<double>(1.0, 2.0)));  // y^2=4x, x=1 -> y=2
    CHECK_TRUE(p.on_boundary(Point<double>(0.25, 1.0))); // y^2=4x -> x=0.25
    // y^2=4(2)=8 -> y=2√2 for x=2
    CHECK_TRUE(p.on_boundary(Point<double>(2.0, std::sqrt(8.0))));
  }

  // Test 9: 45° rotated parabola has correct vertex/focus/directrix.
  {
    const double half_pi_4 = M_PI / 4.0;
    Parabola<double> p(Point<double>(1.0, 2.0), 3.0,
                       static_cast<double>(half_pi_4));
    CHECK_NEAR(p.vertex().x(), 1.0, 1e-9);
    CHECK_NEAR(p.vertex().y(), 2.0, 1e-9);
    CHECK_NEAR(p.focal_parameter(), 3.0, 1e-9);
    const auto f = p.focus();
    CHECK_NEAR(f.x(), 1.0 + 3.0 * std::cos(half_pi_4), 1e-9);
    CHECK_NEAR(f.y(), 2.0 + 3.0 * std::sin(half_pi_4), 1e-9);
    const auto d = p.directrix();
    const double nx = std::cos(half_pi_4);
    const double ny = std::sin(half_pi_4);
    // Directrix line: nx x + ny y + C = 0 with C = p - (nx cx + ny cy).
    const double C = 3.0 - (nx * 1.0 + ny * 2.0);
    CHECK_NEAR(d.A(), nx, 1e-9);
    CHECK_NEAR(d.B(), ny, 1e-9);
    CHECK_NEAR(d.C(), C, 1e-9);
  }

  // Test 10: Rotated parabola — vertex(1,2), p=2, rotation=PI/2
  // (opens along +y). Focus at (1, 4), directrix y = 0.
  {
    Parabola<double> p(Point<double>(1.0, 2.0), 2.0, M_PI / 2.0);
    const auto f = p.focus();
    CHECK_NEAR(f.x(), 1.0, 1e-12);
    CHECK_NEAR(f.y(), 4.0, 1e-12);
    const auto d = p.directrix();
    CHECK_NEAR(d.A(), 0.0, 1e-12);
    CHECK_NEAR(d.B(), 1.0, 1e-12);
    CHECK_NEAR(d.C(), 0.0, 1e-12); // line 0*x + 1*y + 0 = 0, i.e. y = 0
  }

  // Test 11: from_general round-trip stability — vertex form -> general
  // coefficients -> from_general -> geometric parameters are preserved.
  {
    const double pi = std::acos(-1.0);
    Parabola<double> src(Point<double>(-2.5, 3.75), 1.25, pi / 6.0);
    Parabola<double> rt = Parabola<double>::from_general(
        src.coef_x2(), src.coef_y2(), src.coef_xy(), src.coef_x1(),
        src.coef_y1(), src.coef_c());
    CHECK_NEAR(rt.vertex().x(), src.vertex().x(), 1e-9);
    CHECK_NEAR(rt.vertex().y(), src.vertex().y(), 1e-9);
    CHECK_NEAR(rt.focal_parameter(), src.focal_parameter(), 1e-9);
    // Allow for 2*pi periodicity on the rotation angle.
    double dtheta = rt.rotation_angle() - src.rotation_angle();
    while (dtheta > pi)
      dtheta -= 2.0 * pi;
    while (dtheta < -pi)
      dtheta += 2.0 * pi;
    CHECK_NEAR(dtheta, 0.0, 1e-9);
    const auto f1 = src.focus();
    const auto f2 = rt.focus();
    CHECK_NEAR(f1.x(), f2.x(), 1e-9);
    CHECK_NEAR(f1.y(), f2.y(), 1e-9);
    const auto d1 = src.directrix();
    const auto d2 = rt.directrix();
    CHECK_NEAR(d1.A(), d2.A(), 1e-9);
    CHECK_NEAR(d1.B(), d2.B(), 1e-9);
    CHECK_NEAR(d1.C(), d2.C(), 1e-9);
    // Implicit equation preserved (same F(x,y) for any point).
    const double Fsrc = src.evaluate(Point<double>(7.3, -4.1));
    const double Frt = rt.evaluate(Point<double>(7.3, -4.1));
    CHECK_NEAR(Fsrc, Frt, 1e-9);
  }

  if (g_failures == 0) {
    std::cout << "All Parabola tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Parabola test(s) failed." << std::endl;
  return 1;
}
