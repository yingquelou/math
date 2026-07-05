#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Parabola.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
  using namespace planeGeometry;

  auto run = [](const char *label, Parabola<double> const &p1) {
    std::cout << label << std::endl;
    std::cout << "  geom: vertex=(" << p1.vertex().x() << "," << p1.vertex().y()
              << "), p=" << p1.focal_parameter()
              << ", rot=" << p1.rotation_angle() << std::endl;
    std::cout << "  coef: a=" << p1.coef_x2() << " b=" << p1.coef_y2()
              << " c=" << p1.coef_xy() << " d=" << p1.coef_x1()
              << " e=" << p1.coef_y1() << " f=" << p1.coef_c() << std::endl;
    try {
      Parabola<double> p2 = Parabola<double>::from_general(
          p1.coef_x2(), p1.coef_y2(), p1.coef_xy(), p1.coef_x1(), p1.coef_y1(),
          p1.coef_c());
      std::cout << "  Recovered: vertex=(" << p2.vertex().x() << ","
                << p2.vertex().y() << "), p=" << p2.focal_parameter()
                << ", rot=" << p2.rotation_angle() << std::endl;
      std::cout << "  evaluate(v1)=" << p1.evaluate(p1.vertex())
                << " evaluate(v2)=" << p1.evaluate(p2.vertex()) << std::endl;
    } catch (const std::exception &e) {
      std::cout << "  Exception: " << e.what() << std::endl;
    }
  };

  run("Test 1: (0,0), p=2.5, rot=0",
      Parabola<double>(Point<double>(0.0, 0.0), 2.5));
  run("Test 2: (3,-1), p=2.5, rot=0",
      Parabola<double>(Point<double>(3.0, -1.0), 2.5));
  run("Test 3: (0,0), p=1, rot=PI/3",
      Parabola<double>(Point<double>(0.0, 0.0), 1.0, M_PI / 3.0));
  run("Test 4: (-2.5,3.75), p=1.25, rot=PI/6",
      Parabola<double>(Point<double>(-2.5, 3.75), 1.25, M_PI / 6.0));
  run("Test 5: (1,2), p=3, rot=PI/4",
      Parabola<double>(Point<double>(1.0, 2.0), 3.0, M_PI / 4.0));
  run("Test 6: (1,2), p=2, rot=PI/2",
      Parabola<double>(Point<double>(1.0, 2.0), 2.0, M_PI / 2.0));

  return 0;
}
