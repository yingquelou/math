#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

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

#define CHECK_EQ(a, b)                                                         \
  do {                                                                         \
    if (!((a) == (b))) {                                                       \
      std::cerr << "FAIL: " #a " == " #b " at " << __FILE__ << ":" << __LINE__ \
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
  using planeGeometry::Point;
  using planeGeometry::Vector;

  {
    Point<double> p;
    CHECK_NEAR(p.x(), 0.0, 1e-12);
    CHECK_NEAR(p.y(), 0.0, 1e-12);
  }

  {
    Point<double> p(3.0, 4.0);
    CHECK_NEAR(p.x(), 3.0, 1e-12);
    CHECK_NEAR(p.y(), 4.0, 1e-12);
  }

  {
    Point<double> p{5.0, -2.0};
    CHECK_NEAR(p.x(), 5.0, 1e-12);
    CHECK_NEAR(p.y(), -2.0, 1e-12);

    bool threw = false;
    try {
      Point<double> bad{1.0, 2.0, 3.0};
      (void)bad;
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);

    threw = false;
    try {
      Point<double> bad{1.0};
      (void)bad;
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(a);
    CHECK_EQ(a, b);
    CHECK_NEAR(b.x(), 1.0, 1e-12);
    CHECK_NEAR(b.y(), 2.0, 1e-12);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(3.0, 5.0);
    Point<double> c = a + b;
    CHECK_NEAR(c.x(), 4.0, 1e-12);
    CHECK_NEAR(c.y(), 7.0, 1e-12);

    Point<double> d = b - a;
    CHECK_NEAR(d.x(), 2.0, 1e-12);
    CHECK_NEAR(d.y(), 3.0, 1e-12);

    Point<double> e = a * 2.0;
    CHECK_NEAR(e.x(), 2.0, 1e-12);
    CHECK_NEAR(e.y(), 4.0, 1e-12);

    Point<double> f = 3.0 * a;
    CHECK_NEAR(f.x(), 3.0, 1e-12);
    CHECK_NEAR(f.y(), 6.0, 1e-12);

    Point<double> neg = -a;
    CHECK_NEAR(neg.x(), -1.0, 1e-12);
    CHECK_NEAR(neg.y(), -2.0, 1e-12);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(3.0, 5.0);
    Point<double> c(1.0, 2.0);
    c += b;
    CHECK_NEAR(c.x(), 4.0, 1e-12);
    CHECK_NEAR(c.y(), 7.0, 1e-12);

    Point<double> d(3.0, 5.0);
    d -= a;
    CHECK_NEAR(d.x(), 2.0, 1e-12);
    CHECK_NEAR(d.y(), 3.0, 1e-12);

    Point<double> e(1.0, 2.0);
    e *= 3.0;
    CHECK_NEAR(e.x(), 3.0, 1e-12);
    CHECK_NEAR(e.y(), 6.0, 1e-12);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(3.0, 5.0);
    CHECK_NEAR(a.cross_z(b), 1.0 * 5.0 - 2.0 * 3.0, 1e-12);
    CHECK_NEAR(b.cross_z(a), -(1.0 * 5.0 - 2.0 * 3.0), 1e-12);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(3.0, 5.0);
    CHECK_NEAR(a.dot(b), 1.0 * 3.0 + 2.0 * 5.0, 1e-12);
  }

  {
    Point<double> a(3.0, 4.0);
    CHECK_NEAR(a.norm(), 5.0, 1e-12);
    Point<double> z;
    CHECK_NEAR(z.norm(), 0.0, 1e-12);
  }

  {
    Point<double> a(1.0, 0.0);
    Point<double> b(0.0, 1.0);
    CHECK_NEAR(a.angle_to(b), M_PI / 2.0, 1e-9);

    Point<double> c(1.0, 0.0);
    CHECK_NEAR(a.angle_to(c), 0.0, 1e-9);

    Point<double> z;
    CHECK_NEAR(z.angle_to(a), 0.0, 1e-9);
    CHECK_NEAR(a.angle_to(z), 0.0, 1e-9);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(2.0, 4.0);
    CHECK_TRUE(a.is_parallel_to(b));

    Point<double> c(1.0, 3.0);
    CHECK_TRUE(!a.is_parallel_to(c));

    Point<double> z;
    CHECK_TRUE(a.is_parallel_to(z));
  }

  {
    Point<double> a(3.0, 4.0);
    Point<double> axis(1.0, 0.0);
    Point<double> proj = a.projection_on(axis);
    CHECK_NEAR(proj.x(), 3.0, 1e-9);
    CHECK_NEAR(proj.y(), 0.0, 1e-9);

    Point<double> z;
    bool threw = false;
    try {
      a.projection_on(z);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  {
    Point<double> v(2.0, 7.0);
    Point<double> ex(1.0, 1.0);
    Point<double> ey(-2.0, 1.0);
    Point<double> coords = v.coords_in(ex, ey);
    Point<double> recomposed = coords.x() * ex + coords.y() * ey;
    CHECK_NEAR(recomposed.x(), v.x(), 1e-9);
    CHECK_NEAR(recomposed.y(), v.y(), 1e-9);

    Point<double> z;
    bool threw = false;
    try {
      v.coords_in(z, ey);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);

    threw = false;
    try {
      v.coords_in(ex, ex);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);
  }

  {
    Point<double> a(1.0, 2.0);
    Point<double> b(1.0, 2.0);
    Point<double> c(1.0, 3.0);
    CHECK_TRUE(a == b);
    CHECK_TRUE(!(a == c));
    CHECK_TRUE(a != c);
    CHECK_TRUE(!(a != b));
  }

  {
    Point<double> a(1.5, -2.25);
    std::ostringstream oss;
    oss << a;
    CHECK_TRUE(oss.str() == "(1.5,-2.25)");
  }

  {
    Vector v(1.0, 2.0);
    CHECK_NEAR(v.x(), 1.0, 1e-12);
    CHECK_NEAR(v.y(), 2.0, 1e-12);
    Vector w = v + Vector(3.0, 4.0);
    CHECK_NEAR(w.x(), 4.0, 1e-12);
    CHECK_NEAR(w.y(), 6.0, 1e-12);
  }

  {
    Point<double> p(1.0, 0.0);
    Point<double> rot = p.rotated_by(M_PI / 2.0);
    CHECK_NEAR(rot.x(), 0.0, 1e-9);
    CHECK_NEAR(rot.y(), 1.0, 1e-9);

    Point<double> q(2.0, 3.0);
    Point<double> c(1.0, 1.0);
    Point<double> rot2 = q.rotated_by(M_PI / 2.0, c);
    CHECK_NEAR(rot2.x(), -1.0, 1e-9);
    CHECK_NEAR(rot2.y(), 2.0, 1e-9);
  }

  {
    Point<double> u(1.0, 1.0);
    Point<double> v(1.0, -1.0);
    auto gs = Point<double>::gram_schmidt(u, v);
    Point<double> e1 = gs.first;
    Point<double> e2 = gs.second;
    CHECK_NEAR(e1.norm(), 1.0, 1e-12);
    CHECK_NEAR(e2.norm(), 1.0, 1e-12);
    CHECK_NEAR(e1.dot(e2), 0.0, 1e-9);

    bool threw = false;
    try {
      Point<double> z;
      Point<double>::gram_schmidt(z, v);
    } catch (const std::invalid_argument &) {
      threw = true;
    }
    CHECK_TRUE(threw);

    Point<double> u2(1.0, 1.0);
    Point<double> v2(2.0, 2.0);
    bool threw2 = false;
    try {
      Point<double>::gram_schmidt(u2, v2);
    } catch (const std::invalid_argument &) {
      threw2 = true;
    }
    CHECK_TRUE(threw2);
  }

  if (g_failures == 0) {
    std::cout << "All Point tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Point test(s) failed." << std::endl;
  return 1;
}
