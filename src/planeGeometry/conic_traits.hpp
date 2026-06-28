#ifndef PLANE_GEOMETRY_CONIC_TRAITS_HPP
#define PLANE_GEOMETRY_CONIC_TRAITS_HPP

#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief Primary template mapping a concrete conic type to its tag.
 *
 * The primary template is deliberately left undefined so that attempts to
 * use `conic_traits<T>` for an unsupported conic type fail with a clear
 * compile error. Full specializations are provided for each concrete conic
 * (Circle, Ellipse, Hyperbola, Parabola) after the headers that define
 * those types have been included below.
 *
 * @tparam T A concrete conic type (e.g. `Circle<double>`).
 */
template <typename T>
struct conic_traits;

}  // namespace planeGeometry

#include "Circle.hpp"
#include "Ellipse.hpp"
#include "Hyperbola.hpp"
#include "Parabola.hpp"

namespace planeGeometry {

/**
 * @brief Specialization for Circle: tag is `ellipse_tag` (a circle is a
 *        special case of an ellipse).
 * @tparam T Numeric type.
 */
template <typename T>
struct conic_traits<Circle<T>> {
  using tag = ellipse_tag;
};

/**
 * @brief Specialization for Ellipse: tag is `ellipse_tag`.
 * @tparam T Numeric type.
 */
template <typename T>
struct conic_traits<Ellipse<T>> {
  using tag = ellipse_tag;
};

/**
 * @brief Specialization for Hyperbola: tag is `hyperbola_tag`.
 * @tparam T Numeric type.
 */
template <typename T>
struct conic_traits<Hyperbola<T>> {
  using tag = hyperbola_tag;
};

/**
 * @brief Specialization for Parabola: tag is `parabola_tag`.
 * @tparam T Numeric type.
 */
template <typename T>
struct conic_traits<Parabola<T>> {
  using tag = parabola_tag;
};

/**
 * @brief Helper that returns the tag associated with a conic instance.
 *
 * This is the single entry point for tag-based dispatch:
 * @code
 *   auto t = conic_tag_v(my_conic);  // returns ellipse_tag{} etc.
 * @endcode
 *
 * @tparam T A concrete conic type (deduced from the argument).
 * @param conic A concrete conic instance (only its type is inspected).
 * @return A default-constructed tag of the appropriate type.
 */
template <typename T>
inline constexpr typename conic_traits<T>::tag conic_tag_v(const T &conic) noexcept {
  return typename conic_traits<T>::tag{};
}

}  // namespace planeGeometry

#endif
