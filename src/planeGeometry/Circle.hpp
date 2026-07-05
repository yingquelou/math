#ifndef PLANE_GEOMETRY_CIRCLE_HPP
#define PLANE_GEOMETRY_CIRCLE_HPP

#include <cmath>
#include <stdexcept>
#include <utility>

#include "ConicBase.hpp"
#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief Circle class inheriting from ConicBase using CRTP.
 *
 * Represents a circle with center @f$ (c_x, c_y) @f$ and radius @f$ r @f$.
 * The implicit general-form used internally is
 * @math{
 *   F(x,y) = (x - c_x)^2 + (y - c_y)^2 - r^2
 *         = a x^2 + b y^2 + c x y + d x + e y + f
 * }
 * with @f$ a = b = 1 @f$, @f$ c = 0 @f$,
 * @f$ d = -2 c_x @f$, @f$ e = -2 c_y @f$,
 * @f$ f = c_x^2 + c_y^2 - r^2 @f$.
 *
 * Eccentricity is always 0; the two foci coincide at the center.
 *
 * @tparam T Numeric type (double, float, etc.).
 *
 * @note Class invariant:
 *   @li `radius() >= 0` (non-negative radius).
 *   @li `center()` returns the unique center point.
 */
template <typename T> class Circle : public ConicBase<Circle<T>, T> {
  friend class ConicBase<Circle<T>, T>;

private:
  Point<T> center_;
  T radius_;

  /**
   * @brief Recompute geometric representation (center, radius) from the
   *        currently stored general-form coefficients.
   *
   * Uses @f$ c_x = -d / (2a) @f$, @f$ c_y = -e / (2a) @f$ and
   * @f$ r = \sqrt{(d^2 + e^2) / (4 a^2) - f / a} @f$.
   */
  void recompute_from_coeffs() {
    const double a = static_cast<double>(this->a_);
    const double d = static_cast<double>(this->d_);
    const double e = static_cast<double>(this->e_);
    const double f = static_cast<double>(this->f_);
    const double cx = -d / (2.0 * a);
    const double cy = -e / (2.0 * a);
    const double r2 = (d * d + e * e) / (4.0 * a * a) - f / a;
    center_ = Point<T>(static_cast<T>(cx), static_cast<T>(cy));
    radius_ = static_cast<T>(std::sqrt(std::max(0.0, r2)));
  }

public:
  /**
   * @brief Default constructor. Creates the unit circle
   *        @f$ x^2 + y^2 = 1 @f$ (center at origin, radius 1).
   */
  Circle()
      : ConicBase<Circle<T>, T>(T(1), T(1), T(), T(), T(), T(-1)),
        center_(T(), T(0)), radius_(T(1)) {
    recompute_from_coeffs();
  }

  /**
   * @brief Construct a circle from center point and radius.
   *
   * @param center Center point of the circle.
   * @param radius Radius (must be non-negative).
   * @note The general-form coefficients are set so that
   *       @f$ F(x,y) = (x - c_x)^2 + (y - c_y)^2 - r^2 @f$.
   */
  Circle(const Point<T> &center, T radius)
      : ConicBase<Circle<T>, T>(T(1), T(1), T(),
                                static_cast<T>(-2) * center.x(),
                                static_cast<T>(-2) * center.y(),
                                center.x() * center.x() +
                                    center.y() * center.y() - radius * radius),
        center_(center), radius_(radius) {}

  /**
   * @brief Construct a circle from center coordinates and radius.
   * @param cx Center @f$ x @f$.
   * @param cy Center @f$ y @f$.
   * @param radius Radius (must be non-negative).
   */
  Circle(T cx, T cy, T radius) : Circle(Point<T>(cx, cy), radius) {}

  /**
   * @brief Construct a circle from general-form coefficients.
   *
   * Requirements (all validated):
   *   @li Discriminant @f$ \Delta = c^2 - 4 a b < 0 @f$.
   *   @li @f$ a = b @f$ and @f$ c = 0 @f$ (circle, not a general ellipse).
   *   @li @f$ a > 0 @f$.
   *
   * @tparam T Numeric type.
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @param d Coefficient @f$ d @f$.
   * @param e Coefficient @f$ e @f$.
   * @param f Constant term @f$ f @f$.
   * @return A `Circle` whose geometric representation is recovered from
   *         the coefficients.
   * @throws std::invalid_argument if any requirement is violated.
   */
  static Circle from_general(T a, T b, T c, T d, T e, T f) {
    const double disc =
        static_cast<double>(ConicBase<Circle<T>, T>::discriminant(a, b, c));
    if (disc >= 0) {
      throw std::invalid_argument(
          "Circle::from_general: discriminant must be < 0");
    }
    if (std::abs(static_cast<double>(a) - static_cast<double>(b)) > 1e-9) {
      throw std::invalid_argument("Circle::from_general: a must equal b");
    }
    if (std::abs(static_cast<double>(c)) > 1e-9) {
      throw std::invalid_argument("Circle::from_general: c must be 0");
    }
    if (static_cast<double>(a) <= 0) {
      throw std::invalid_argument("Circle::from_general: a must be positive");
    }
    Circle result;
    result.a_ = a;
    result.b_ = b;
    result.c_ = c;
    result.d_ = d;
    result.e_ = e;
    result.f_ = f;
    result.recompute_from_coeffs();
    return result;
  }

  /** @brief Return the conic tag for circles (`ellipse_tag`). */
  static conic_tag derived_tag() { return ellipse_tag{}; }

  /**
   * @brief Return the center point (const reference).
   * @return Const reference to the center.
   */
  const Point<T> &center() const { return center_; }

  /**
   * @brief Return the radius.
   * @return The circle's radius (non-negative).
   */
  T radius() const { return radius_; }

  /**
   * @brief Return the area @f$ \pi r^2 @f$.
   * @return Area of the circle.
   */
  T area() const {
    return static_cast<T>(M_PI * static_cast<double>(radius_) *
                          static_cast<double>(radius_));
  }

  /**
   * @brief Return the circumference @f$ 2 \pi r @f$.
   * @return Circumference of the circle.
   */
  T circumference() const {
    return static_cast<T>(2.0 * M_PI * static_cast<double>(radius_));
  }

  /**
   * @brief Eccentricity of a circle is always 0.
   * @return @c T(0).
   */
  T eccentricity() const { return T(); }

  /**
   * @brief Return the two focal points of the circle.
   *
   * For a circle the two foci coincide at the center, so the returned
   * pair has both entries equal to `center()`.
   *
   * @return A `std::pair` containing the center as both entries.
   */
  std::pair<Point<T>, Point<T>> focal_points() const {
    return std::make_pair(center_, center_);
  }

  /**
   * @brief Classify a point w.r.t. the circle.
   *
   * A point is *inside* the circle when the quadratic form @f$ F(P) @f$
   * has the opposite sign from the leading coefficient @f$ a @f$ (with the
   * normalization used by the Circle constructors, where @f$ F = 0 @f$ on
   * the boundary and the inside value has the opposite sign of @f$ a @f$).
   *
   * @param p   Point to classify.
   * @param tol Tolerance for boundary comparison.
   * @return `+1` if @p p is inside, `-1` if outside, `0` on the boundary.
   */
  int relative_position(const Point<T> &p, T tol = T(1e-12)) const {
    const int fs = this->f_sign(p, tol);
    if (fs == 0)
      return 0;
    const double sign_a = (static_cast<double>(this->a_) > 0.0) ? 1.0 : -1.0;
    const double sign_F = (fs > 0) ? 1.0 : -1.0;
    return (sign_a * sign_F < 0.0) ? 1 : -1;
  }
};

} // namespace planeGeometry

#endif
