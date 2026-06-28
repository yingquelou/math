#include <cmath>
#include <iostream>
#include "transform2D.hpp"

static int g_failures = 0;
#define CHECK_TRUE(expr) do { if (!(expr)) { std::cerr << "FAIL: " #expr << " at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failures; } } while (0)
#define CHECK_NEAR(a, b, tol) do { if (std::abs((a)-(b)) > (tol)) { std::cerr << "FAIL: " #a " near " #b " at " << __FILE__ << ":" << __LINE__ << " (" << (a) << " vs " << (b) << ")" << std::endl; ++g_failures; } } while (0)

int main() {
  using namespace planeGeometry;

  {
    auto m = rotation_matrix<double>(0.0);
    CHECK_NEAR(m(0, 0), 1.0, 1e-12);
    CHECK_NEAR(m(0, 1), 0.0, 1e-12);
    CHECK_NEAR(m(1, 0), 0.0, 1e-12);
    CHECK_NEAR(m(1, 1), 1.0, 1e-12);
  }

  {
    auto m = rotation_matrix<double>(M_PI / 2.0);
    CHECK_NEAR(m(0, 0), 0.0, 1e-9);
    CHECK_NEAR(m(0, 1), -1.0, 1e-9);
    CHECK_NEAR(m(1, 0), 1.0, 1e-9);
    CHECK_NEAR(m(1, 1), 0.0, 1e-9);
  }

  {
    auto m = rotation_matrix<double>(0.7);
    auto prod = m * m.transpose();
    for (std::size_t i = 0; i < 2; ++i) {
      for (std::size_t j = 0; j < 2; ++j) {
        CHECK_NEAR(prod(i, j), (i == j ? 1.0 : 0.0), 1e-12);
      }
    }
    CHECK_NEAR(m.determinant(), 1.0, 1e-9);
  }

  {
    Point<double> p(3.0, 4.0);
    double orig_len = p.norm();
    Point<double> r = rotate_about_origin(p, 1.2);
    CHECK_NEAR(r.norm(), orig_len, 1e-12);

    Point<double> r90 = rotate_about_origin(p, M_PI / 2.0);
    CHECK_NEAR(r90.x(), -4.0, 1e-9);
    CHECK_NEAR(r90.y(), 3.0, 1e-9);
  }

  {
    Point<double> p(2.0, 3.0);
    Point<double> center(1.0, 1.0);
    double orig_dist = (p - center).norm();
    Point<double> r = rotate_about(p, center, M_PI / 2.0);
    double new_dist = (r - center).norm();
    CHECK_NEAR(new_dist, orig_dist, 1e-12);
    auto d1 = (rotate_about_origin(Point<double>(p.x() - center.x(), p.y() - center.y()), M_PI / 2.0));
    CHECK_NEAR(r.x(), center.x() + d1.x(), 1e-9);
    CHECK_NEAR(r.y(), center.y() + d1.y(), 1e-9);
  }

  {
    Point<double> p(1.0, 2.0);
    Point<double> t = translate(p, 3.5, -1.0);
    CHECK_NEAR(t.x(), 4.5, 1e-12);
    CHECK_NEAR(t.y(), 1.0, 1e-12);
  }

  {
    Point<double> p(1.0, 0.0);
    Point<double> r = rotate_about_origin(p, M_PI / 3.0);
    Point<double> t = translate(r, 2.0, -3.0);
    Point<double> rt = rigid_transform(p, M_PI / 3.0, 2.0, -3.0);
    CHECK_NEAR(rt.x(), t.x(), 1e-12);
    CHECK_NEAR(rt.y(), t.y(), 1e-12);
  }

  {
    Point<double> p(2.0, -3.0);
    Point<double> s = scale(p, 5.0, 7.0);
    CHECK_NEAR(s.x(), 10.0, 1e-12);
    CHECK_NEAR(s.y(), -21.0, 1e-12);
  }

  {
    Point<double> p(1.0, 2.0);
    double theta = 0.9;
    Point<double> r1 = rotate_about_origin(p, theta);
    Point<double> r2 = rotate_about_origin(r1, -theta);
    CHECK_NEAR(r2.x(), p.x(), 1e-9);
    CHECK_NEAR(r2.y(), p.y(), 1e-9);
  }

  {
    Point<double> p(5.0, -7.0);
    Point<double> center(2.0, 3.0);
    double theta = 1.3;
    Point<double> r1 = rotate_about(p, center, theta);
    Point<double> r2 = rotate_about(r1, center, -theta);
    CHECK_NEAR(r2.x(), p.x(), 1e-9);
    CHECK_NEAR(r2.y(), p.y(), 1e-9);
  }

  if (g_failures == 0) {
    std::cout << "All Transform2D tests passed." << std::endl;
    return 0;
  }
  std::cerr << g_failures << " Transform2D test(s) failed." << std::endl;
  return 1;
}
