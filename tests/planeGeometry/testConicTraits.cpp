#include <iostream>
#include <type_traits>

#include "conic_traits.hpp"

using namespace planeGeometry;

static_assert(std::is_same<conic_traits<Circle<double>>::tag, ellipse_tag>::value,
              "Circle<double> should map to ellipse_tag");
static_assert(std::is_same<conic_traits<Ellipse<double>>::tag, ellipse_tag>::value,
              "Ellipse<double> should map to ellipse_tag");

int main() {
  auto tag_circle = conic_tag_v(Circle<double>(Point<double>(0.0, 0.0), 1.0));
  auto tag_ellipse = conic_tag_v(Ellipse<double>(Point<double>(0.0, 0.0), 3.0, 2.0));
  (void)tag_circle;
  (void)tag_ellipse;

  std::cout << "Circle derived_tag returns: "
            << (std::is_same<decltype(Circle<double>::derived_tag()), conic_tag>::value
                ? "conic_tag (ellipse)"
                : "something else")
            << std::endl;
  std::cout << "Ellipse derived_tag returns: "
            << (std::is_same<decltype(Ellipse<double>::derived_tag()), conic_tag>::value
                ? "conic_tag (ellipse)"
                : "something else")
            << std::endl;

  std::cout << "conic_traits / conic_tag_v smoke test passed." << std::endl;
  return 0;
}
