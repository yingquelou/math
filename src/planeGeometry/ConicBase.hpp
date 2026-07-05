#ifndef PLANE_GEOMETRY_CONIC_BASE_HPP
#define PLANE_GEOMETRY_CONIC_BASE_HPP

#include <cmath>
#include <ostream>

#include "Line.hpp"
#include "Point.hpp"

#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief CRTP base class for all conic sections.
 *
 * Stores the general-form coefficients of a conic:
 * @math{
 *   F(x,y) = a x^2 + b y^2 + c x y + d x + e y + f
 * }
 *
 * and provides the common evaluation / classification / relative-position
 * machinery that Circle, Ellipse, Parabola and Hyperbola inherit.
 *
 * @tparam Derived The concrete conic class (e.g. Circle<T>, Ellipse<T>).
 * @tparam T       Numeric type (double, float, etc.).
 *
 * @note Derived classes are expected to override `relative_position` to
 *       expose the correct geometric "inside / outside / on boundary"
 *       semantics for their specific shape.
 */
template <typename Derived, typename T> class ConicBase {
protected:
  T a_, b_, c_, d_, e_, f_;

public:
  /** @brief Default constructor. All coefficients are zero. */
  ConicBase() : a_(T()), b_(T()), c_(T()), d_(T()), e_(T()), f_(T()) {}

  /**
   * @brief Construct from general-form coefficients.
   * @param a Coefficient of @f$ x^2 @f$.
   * @param b Coefficient of @f$ y^2 @f$.
   * @param c Coefficient of @f$ x y @f$.
   * @param d Coefficient of @f$ x @f$.
   * @param e Coefficient of @f$ y @f$.
   * @param f Constant term.
   */
  ConicBase(T a, T b, T c, T d, T e, T f)
      : a_(a), b_(b), c_(c), d_(d), e_(e), f_(f) {}

  ConicBase(const ConicBase &) = default;
  ConicBase(ConicBase &&) noexcept = default;
  ConicBase &operator=(const ConicBase &) = default;
  ConicBase &operator=(ConicBase &&) noexcept = default;
  ~ConicBase() = default;

  /** @brief Coefficient @f$ a @f$ of @f$ x^2 @f$. */
  T coef_x2() const noexcept { return a_; }
  /** @brief Coefficient @f$ b @f$ of @f$ y^2 @f$. */
  T coef_y2() const noexcept { return b_; }
  /** @brief Coefficient @f$ c @f$ of @f$ x y @f$. */
  T coef_xy() const noexcept { return c_; }
  /** @brief Coefficient @f$ d @f$ of @f$ x @f$. */
  T coef_x1() const noexcept { return d_; }
  /** @brief Coefficient @f$ e @f$ of @f$ y @f$. */
  T coef_y1() const noexcept { return e_; }
  /** @brief Constant term @f$ f @f$. */
  T coef_c() const noexcept { return f_; }

  /**
   * @brief Check whether the stored coefficients are not all zero.
   * @return `true` if at least one coefficient is significantly non-zero.
   */
  bool is_valid() const noexcept {
    return !(std::abs(a_) < T(1e-15) && std::abs(b_) < T(1e-15) &&
             std::abs(c_) < T(1e-15) && std::abs(d_) < T(1e-15) &&
             std::abs(e_) < T(1e-15) && std::abs(f_) < T(1e-15));
  }

  /**
   * @brief Compute the conic discriminant @f$ \Delta = c^2 - 4 a b @f$.
   *
   * The sign of @f$ \Delta @f$ classifies the conic:
   *   @li @f$ \Delta < 0 @f$: ellipse (or circle when @f$ a = b @f$ and
   *       @f$ c = 0 @f$).
   *   @li @f$ \Delta = 0 @f$: parabola (or degenerate parallel lines).
   *   @li @f$ \Delta > 0 @f$: hyperbola.
   *
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @return The value of @f$ c^2 - 4 a b @f$.
   */
  static T discriminant(T a, T b, T c) { return c * c - 4 * a * b; }

  /**
   * @brief Evaluate the quadratic form @f$ F(x,y) @f$ at point @p p.
   * @param p The point @f$ (x, y) @f$ to evaluate at.
   * @return The value of the general-form polynomial at @p p.
   */
  T evaluate(const Point<T> &p) const {
    const double x = static_cast<double>(p.x());
    const double y = static_cast<double>(p.y());
    const double v =
        static_cast<double>(a_) * x * x + static_cast<double>(b_) * y * y +
        static_cast<double>(c_) * x * y + static_cast<double>(d_) * x +
        static_cast<double>(e_) * y + static_cast<double>(f_);
    return static_cast<T>(v);
  }

  /**
   * @brief Return the default tag associated with the derived conic.
   *
   * The base implementation returns `ellipse_tag`; each derived class
   * overrides this to return its proper tag (e.g. `parabola_tag`,
   * `hyperbola_tag`).
   */
  static conic_tag derived_tag() { return ellipse_tag{}; }

protected:
  /**
   * @brief Return the raw sign of @f$ F(P) @f$.
   * @param p   Point to evaluate at.
   * @param tol Tolerance: if @f$ |F(P)| \le tol @f$ the point is treated
   *            as lying on the boundary and `0` is returned.
   * @return `+1` if @f$ F(P) > tol @f$, `-1` if @f$ F(P) < -tol @f`,
   *         `0` otherwise.
   */
  int f_sign(const Point<T> &p, T tol) const {
    const double F = static_cast<double>(evaluate(p));
    if (std::abs(F) < static_cast<double>(tol))
      return 0;
    return (F > 0.0) ? 1 : -1;
  }

public:
  /**
   * @brief Base-level semantics: returns the sign of @f$ F(P) @f$ only.
   *
   * Returns
   *   @li `+1` if @f$ F(P) > tol @f$,
   *   @li `-1` if @f$ F(P) < -tol @f$,
   *   @li ` 0` if on boundary (@f$ |F(P)| \le tol @f$).
   *
   * The inside/outside meaning of this sign is per-derived-class semantic
   * (ellipse/circle: inside @f$\Leftrightarrow@f$ sign of @f$ F @f$ equals
   * the opposite sign of @f$ a @f$; hyperbola/parabola: each has its own
   * convention). Derived classes shall override `relative_position` to
   * expose the proper geometric semantics.
   *
   * @param p   Point to classify.
   * @param tol Tolerance for boundary comparison.
   * @return `+1`, `-1` or `0` as described above.
   */
  int relative_position(const Point<T> &p, T tol = T(1e-12)) const {
    return f_sign(p, tol);
  }

  /**
   * @brief Test whether @p p lies strictly inside the conic.
   *
   * The decision is delegated to `Derived::relative_position`.
   *
   * @param p   Point to test.
   * @param tol Tolerance forwarded to `relative_position`.
   * @return `true` if the point is strictly inside the bounded region.
   */
  bool contains(const Point<T> &p, T tol = T(1e-12)) const {
    return static_cast<const Derived &>(*this).relative_position(p, tol) == 1;
  }

  /**
   * @brief Test whether @p p lies exactly on the conic boundary.
   * @param p   Point to test.
   * @param tol Tolerance forwarded to `relative_position`.
   * @return `true` if @f$ |F(P)| \le tol @f$.
   */
  bool on_boundary(const Point<T> &p, T tol = T(1e-12)) const {
    return static_cast<const Derived &>(*this).relative_position(p, tol) == 0;
  }

  /**
   * @brief Stream operator: prints `Conic(a=..., b=..., ...)`.
   * @param os Output stream.
   * @param c  The conic to print.
   * @return Reference to @p os.
   */
  friend std::ostream &operator<<(std::ostream &os, const ConicBase &c) {
    os << "Conic(a=" << c.a_ << ", b=" << c.b_ << ", c=" << c.c_
       << ", d=" << c.d_ << ", e=" << c.e_ << ", f=" << c.f_ << ")";
    return os;
  }
};

} // namespace planeGeometry

#endif
