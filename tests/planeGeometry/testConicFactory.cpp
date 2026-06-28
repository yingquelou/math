#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "conic_factory.hpp"
#include "Point.hpp"

using namespace planeGeometry;

namespace {

struct classify_visitor {
  typedef std::string result_t;

  result_t operator()() const { return "monostate"; }
#if PLANE_GEOMETRY_HAS_STD_VARIANT
  result_t operator()(std::monostate) const { return "monostate"; }
#endif
  result_t operator()(const Circle<double> &c) const {
    (void)c;
    return "Circle";
  }
  result_t operator()(const Ellipse<double> &e) const {
    (void)e;
    return "Ellipse";
  }
  result_t operator()(const Parabola<double> &p) const {
    (void)p;
    return "Parabola";
  }
  result_t operator()(const Hyperbola<double> &h) const {
    (void)h;
    return "Hyperbola";
  }
};

#if PLANE_GEOMETRY_HAS_STD_VARIANT
struct classify_lambda_visitor {
  typedef std::string result_t;
  result_t operator()(std::monostate) const { return "monostate"; }
  result_t operator()(const Circle<double> &c) const {
    (void)c;
    return "Circle";
  }
  result_t operator()(const Ellipse<double> &e) const {
    (void)e;
    return "Ellipse";
  }
  result_t operator()(const Parabola<double> &p) const {
    (void)p;
    return "Parabola";
  }
  result_t operator()(const Hyperbola<double> &h) const {
    (void)h;
    return "Hyperbola";
  }
};
#endif

struct property_visitor {
  typedef std::string result_t;

#if PLANE_GEOMETRY_HAS_STD_VARIANT
  result_t operator()(std::monostate) const { return std::string("monostate"); }
#else
  result_t operator()() const { return std::string("monostate"); }
#endif
  result_t operator()(const Circle<double> &c) const {
    std::ostringstream oss;
    oss << "Circle(center=(" << c.center().x() << "," << c.center().y()
        << "),r=" << c.radius() << ")";
    return oss.str();
  }
  result_t operator()(const Ellipse<double> &e) const {
    std::ostringstream oss;
    oss << "Ellipse(a=" << e.semi_major() << ",b=" << e.semi_minor() << ")";
    return oss.str();
  }
  result_t operator()(const Parabola<double> &p) const {
    std::ostringstream oss;
    oss << "Parabola(vertex=(" << p.vertex().x() << "," << p.vertex().y()
        << "),p=" << p.focal_parameter() << ")";
    return oss.str();
  }
  result_t operator()(const Hyperbola<double> &h) const {
    std::ostringstream oss;
    oss << "Hyperbola(a=" << h.real_semi_axis()
        << ",b=" << h.imag_semi_axis() << ")";
    return oss.str();
  }
};

void require(bool cond, const std::string &msg) {
  if (!cond) {
    std::cerr << "FAIL: " << msg << std::endl;
    std::abort();
  }
  std::cout << "PASS: " << msg << std::endl;
}

}  // namespace

int main() {
  std::cout << "=== conic_factory tests ===" << std::endl;

  // Circle: x^2 + y^2 = 1  →  a=1,b=1,c=0,d=0,e=0,f=-1
  {
    auto v = make_conic(1.0, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(v, classify_visitor());
    require(kind == "Circle", "circle classified as Circle");
  }

  // Ellipse: x^2/4 + y^2 = 1  →  a=1/4,b=1,c=0,d=0,e=0,f=-1
  {
    auto v = make_conic(0.25, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(v, classify_visitor());
    require(kind == "Ellipse", "ellipse classified as Ellipse");
  }

  // Parabola: y = x^2  →  a=1,b=0,c=0,d=0,e=-1,f=0
  {
    auto v = make_conic(1.0, 0.0, 0.0, 0.0, -1.0, 0.0);
    std::string kind = visit_conic(v, classify_visitor());
    require(kind == "Parabola", "parabola classified as Parabola");
  }

  // Hyperbola: x^2 - y^2 = 1  →  a=1,b=-1,c=0,d=0,e=0,f=-1
  {
    auto v = make_conic(1.0, -1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(v, classify_visitor());
    require(kind == "Hyperbola", "hyperbola classified as Hyperbola");
  }

  // Visit with property visitor extracts different properties per type.
  {
    auto vc = make_conic(1.0, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string sc = visit_conic(vc, property_visitor());
    require(sc.size() == 26, "property_visitor(Circle): size");
    require(sc.compare(0, 7, "Circle(") == 0, "property_visitor(Circle): prefix");

    auto ve = make_conic(0.25, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string se = visit_conic(ve, property_visitor());
    require(se.compare(0, 8, "Ellipse(") == 0, "property_visitor(Ellipse): prefix");

    auto vp = make_conic(1.0, 0.0, 0.0, 0.0, -1.0, 0.0);
    std::string sp = visit_conic(vp, property_visitor());
    require(sp.compare(0, 9, "Parabola(") == 0, "property_visitor(Parabola): prefix");

    auto vh = make_conic(1.0, -1.0, 0.0, 0.0, 0.0, -1.0);
    std::string sh = visit_conic(vh, property_visitor());
    require(sh.compare(0, 10, "Hyperbola(") == 0, "property_visitor(Hyperbola): prefix");
  }

  // Variant-based no-if/switch tag dispatch (C++17 only)
#if PLANE_GEOMETRY_HAS_STD_VARIANT
  {
    auto vc = make_conic(1.0, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(vc, classify_lambda_visitor());
    require(kind == "Circle", "variant dispatch: Circle");
  }
  {
    auto ve = make_conic(0.25, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(ve, classify_lambda_visitor());
    require(kind == "Ellipse", "variant dispatch: Ellipse");
  }
  {
    auto vp = make_conic(1.0, 0.0, 0.0, 0.0, -1.0, 0.0);
    std::string kind = visit_conic(vp, classify_lambda_visitor());
    require(kind == "Parabola", "variant dispatch: Parabola");
  }
  {
    auto vh = make_conic(1.0, -1.0, 0.0, 0.0, 0.0, -1.0);
    std::string kind = visit_conic(vh, classify_lambda_visitor());
    require(kind == "Hyperbola", "variant dispatch: Hyperbola");
  }
#endif

  // Default-constructed variant yields monostate.
  {
    conic_variant<double> v;
    std::string kind = visit_conic(v, classify_visitor());
    require(kind == "monostate", "default variant is monostate");
  }

  // Verify the factory produces a Circle with expected geometry.
  {
    auto v = make_conic(1.0, 1.0, 0.0, 0.0, 0.0, -1.0);
    std::string info = visit_conic(v, property_visitor());
    // Find last "r=" which is the radius (since "center" also contains "r=").
    std::size_t rp = info.rfind("r=");
    require(rp != std::string::npos, "Circle info contains 'r='");
    double radius = std::strtod(info.c_str() + rp + 2, nullptr);
    require(std::abs(radius - 1.0) < 1e-9, "Circle radius == 1");
  }

  std::cout << "conic_factory tests: ALL PASSED" << std::endl;
  return 0;
}
