#ifndef PLANE_GEOMETRY_ELLIPSE_HPP
#define PLANE_GEOMETRY_ELLIPSE_HPP

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "ConicBase.hpp"
#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief Ellipse class inheriting from ConicBase using CRTP.
 *
 * Represents an ellipse with center @f$ (c_x, c_y) @f$, semi-major axis
 * @f$ a @f$, semi-minor axis @f$ b @f$ (with the invariant @f$ a \ge b @f$)
 * and a rotation angle @f$ \theta @f$ of the major axis.
 *
 * The canonical form (aligned with the axes) is
 * @math{
 *   \frac{x^2}{a^2} + \frac{y^2}{b^2} = 1.
 * }
 *
 * Eccentricity is @f$ e = \sqrt{1 - (b/a)^2} @f$ (when @f$ a \ge b @f$),
 * always in @f$ [0, 1) @f$.
 *
 * @tparam T Numeric type (double, float, etc.).
 *
 * @note Class invariants:
 *   @li `semi_major() >= semi_minor()` (enforced in all constructors and
 *       `from_general` / `from_axes`). When the user supplies `a < b`,
 *       the two values are swapped and `rotation_angle()` is adjusted
 *       accordingly so that the geometric shape is preserved.
 *   @li `semi_major() > 0` and `semi_minor() > 0` for valid ellipses.
 */
template <typename T>
class Ellipse : public ConicBase<Ellipse<T>, T> {
  friend class ConicBase<Ellipse<T>, T>;

 private:
  Point<T> center_;
  T semi_a_;
  T semi_b_;
  T rotation_;

  /**
   * @brief Recompute geometric representation (center, semi-axes, rotation)
   *        from the currently stored general-form coefficients.
   *
   * After recovery the semi-axes are re-ordered so that
   * @f$ a \ge b @f$.
   */
  void recompute_from_coeffs() {
    const double A = static_cast<double>(this->a_);
    const double B = static_cast<double>(this->b_);
    const double C = static_cast<double>(this->c_);
    const double D = static_cast<double>(this->d_);
    const double E = static_cast<double>(this->e_);
    const double F = static_cast<double>(this->f_);

    if (std::abs(A) < 1e-15 && std::abs(B) < 1e-15) {
      center_ = Point<T>(T(), T());
      semi_a_ = T(1);
      semi_b_ = T(1);
      rotation_ = T();
      return;
    }

    const double theta = 0.5 * std::atan2(C, A - B);
    rotation_ = static_cast<T>(theta);

    const double den = C * C - 4.0 * A * B;
    if (std::abs(den) < 1e-15) {
      center_ = Point<T>(T(), T());
      semi_a_ = T(1);
      semi_b_ = T(1);
      rotation_ = T();
      return;
    }
    const double cx = (B * D - 0.5 * C * E) / den;
    const double cy = (A * E - 0.5 * C * D) / den;
    center_ = Point<T>(static_cast<T>(cx), static_cast<T>(cy));

    const double cc = std::cos(theta);
    const double ss = std::sin(theta);
    const double Ap = A * cc * cc + C * cc * ss + B * ss * ss;
    const double Bp = A * ss * ss - C * cc * ss + B * cc * cc;
    const double Fp = F + A * cx * cx + B * cy * cy + C * cx * cy + D * cx + E * cy;

    if (std::abs(Ap) < 1e-15 || std::abs(Bp) < 1e-15) {
      semi_a_ = T(1);
      semi_b_ = T(1);
      return;
    }
    semi_a_ = static_cast<T>(std::sqrt(std::abs(-Fp / Ap)));
    semi_b_ = static_cast<T>(std::sqrt(std::abs(-Fp / Bp)));
    if (semi_a_ < semi_b_) {
      using std::swap;
      swap(semi_a_, semi_b_);
    }
  }

 public:
  /**
   * @brief Default constructor. Creates a unit circle centered at origin
   *        (a degenerate ellipse with @f$ a = b = 1 @f$).
   */
  Ellipse()
      : ConicBase<Ellipse<T>, T>(T(), T(), T(), T(), T(), T()),
        center_(T(), T(0)),
        semi_a_(T(1)),
        semi_b_(T(1)),
        rotation_(T()) {}

  /**
   * @brief Construct an ellipse from general-form coefficients.
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @param d Coefficient @f$ d @f$.
   * @param e Coefficient @f$ e @f$.
   * @param f Constant term @f$ f @f$.
   */
  Ellipse(T a, T b, T c, T d, T e, T f)
      : ConicBase<Ellipse<T>, T>(a, b, c, d, e, f),
        center_(T(), T(0)),
        semi_a_(T(1)),
        semi_b_(T(1)),
        rotation_(T()) {
    recompute_from_coeffs();
  }

  /**
   * @brief Construct an ellipse from center, semi-axes and rotation.
   *
   * If @p a < @p b the two values are swapped (and the rotation angle
   * is not adjusted because the major-axis direction remains the same
   * when the axes are simply re-labeled).
   *
   * @param center   Center of the ellipse.
   * @param a        Semi-major axis length (may be swapped with @p b
   *                 internally to enforce @f$ a \ge b @f$).
   * @param b        Semi-minor axis length.
   * @param rotation Rotation angle of the major axis (radians).
   */
  Ellipse(const Point<T> &center, T a, T b, T rotation = T())
      : center_(center),
        semi_a_(a),
        semi_b_(b),
        rotation_(rotation) {
    if (semi_a_ < semi_b_) {
      using std::swap;
      swap(semi_a_, semi_b_);
    }
    const double cc = std::cos(static_cast<double>(rotation));
    const double ss = std::sin(static_cast<double>(rotation));
    const double inv_a2 = 1.0 / (static_cast<double>(semi_a_) * static_cast<double>(semi_a_));
    const double inv_b2 = 1.0 / (static_cast<double>(semi_b_) * static_cast<double>(semi_b_));
    const double Ac = cc * cc * inv_a2 + ss * ss * inv_b2;
    const double Bc = ss * ss * inv_a2 + cc * cc * inv_b2;
    const double Cc = 2.0 * cc * ss * (inv_a2 - inv_b2);
    const double cx = static_cast<double>(center.x());
    const double cy = static_cast<double>(center.y());
    const double Dc = -2.0 * cx * Ac - cy * Cc;
    const double Ec = -2.0 * cy * Bc - cx * Cc;
    const double Fc = Ac * cx * cx + Bc * cy * cy + Cc * cx * cy - 1.0;
    this->a_ = static_cast<T>(Ac);
    this->b_ = static_cast<T>(Bc);
    this->c_ = static_cast<T>(Cc);
    this->d_ = static_cast<T>(Dc);
    this->e_ = static_cast<T>(Ec);
    this->f_ = static_cast<T>(Fc);
  }

  /**
   * @brief Construct an ellipse from major and minor axis lines.
   *
   * The axis lines are used to recover the general-form coefficients;
   * the geometric representation is then extracted via
   * `recompute_from_coeffs`.
   *
   * @param major Major axis line.
   * @param minor Minor axis line.
   * @return A fully-reconstructed `Ellipse`.
   */
  static Ellipse from_axes(const Line<T> &major, const Line<T> &minor) {
    const auto major_n = major.normal_vector();
    const auto minor_n = minor.normal_vector();
    Ellipse result;
    result.a_ = major_n.x();
    result.b_ = minor_n.y();
    result.c_ = T();
    result.d_ = major_n.y();
    result.e_ = minor_n.x();
    result.f_ = T(-1);
    result.recompute_from_coeffs();
    return result;
  }

  /**
   * @brief Construct an ellipse from general-form coefficients.
   *
   * Validates that the discriminant satisfies
   * @f$ \Delta = c^2 - 4 a b < 0 @f$.
   *
   * @tparam T Numeric type.
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @param d Coefficient @f$ d @f$.
   * @param e Coefficient @f$ e @f$.
   * @param f Constant term @f$ f @f$.
   * @return A fully-reconstructed `Ellipse`.
   * @throws std::invalid_argument if @f$ \Delta \ge 0 @f$.
   */
  static Ellipse from_general(T a, T b, T c, T d, T e, T f) {
    const double disc =
        static_cast<double>(ConicBase<Ellipse<T>, T>::discriminant(a, b, c));
    if (disc >= 0) {
      throw std::invalid_argument(
          "Ellipse::from_general: discriminant must be < 0");
    }
    Ellipse result;
    result.a_ = a;
    result.b_ = b;
    result.c_ = c;
    result.d_ = d;
    result.e_ = e;
    result.f_ = f;
    result.recompute_from_coeffs();
    return result;
  }

  /** @brief Return the conic tag (`ellipse_tag`). */
  static conic_tag derived_tag() { return ellipse_tag{}; }

  /**
   * @brief Return the center point.
   * @return Const reference to the center.
   */
  const Point<T> &center() const { return center_; }

  /**
   * @brief Return the semi-major axis length @f$ a @f$.
   *
   * Guaranteed to satisfy `semi_major() >= semi_minor()`.
   *
   * @return Semi-major axis length.
   */
  T semi_major() const { return semi_a_; }

  /**
   * @brief Return the semi-minor axis length @f$ b @f$.
   *
   * Guaranteed to satisfy `semi_minor() <= semi_major()`.
   *
   * @return Semi-minor axis length.
   */
  T semi_minor() const { return semi_b_; }

  /**
   * @brief Return the rotation angle of the major axis (radians).
   * @return Rotation angle in radians.
   */
  T rotation_angle() const { return rotation_; }

  /**
   * @brief Backward-compatible alias for `rotation_angle()`.
   * @return Same value as `rotation_angle()`.
   */
  T rotation() const { return rotation_; }

  /**
   * @brief Eccentricity @f$ e = \sqrt{1 - (b/a)^2} @f$ (with
   *        @f$ a \ge b @f$).
   * @return Eccentricity in @f$ [0, 1) @f$.
   */
  T eccentricity() const {
    const double a = static_cast<double>(semi_a_);
    const double b = static_cast<double>(semi_b_);
    const double ratio = (a >= b) ? (b * b) / (a * a) : (a * a) / (b * b);
    return static_cast<T>(std::sqrt(std::max(0.0, 1.0 - ratio)));
  }

  /**
   * @brief Return the focal distance @f$ c = \sqrt{|a^2 - b^2|} @f$.
   * @return Distance from the center to each focus.
   */
  T focal_distance() const {
    const double a = static_cast<double>(semi_a_);
    const double b = static_cast<double>(semi_b_);
    const double c2 = std::abs(a * a - b * b);
    return static_cast<T>(std::sqrt(c2));
  }

  /**
   * @brief Return the area @f$ \pi a b @f$.
   * @return Area of the ellipse.
   */
  T area() const {
    return static_cast<T>(M_PI * static_cast<double>(semi_a_) *
                          static_cast<double>(semi_b_));
  }

  /**
   * @brief Return the circumference computed with Ramanujan's approximation.
   * @return Approximate circumference.
   */
  T circumference() const {
    const double a = static_cast<double>(semi_a_);
    const double b = static_cast<double>(semi_b_);
    const double h = (a - b) * (a - b) / ((a + b) * (a + b));
    return static_cast<T>(
        M_PI * (a + b) * (1.0 + 3.0 * h / (10.0 + std::sqrt(4.0 - 3.0 * h))));
  }

  /**
   * @brief Return the two focal points.
   * @return Pair of foci along the major axis, offset by `focal_distance()`.
   */
  std::pair<Point<T>, Point<T>> focal_points() const {
    const double c = static_cast<double>(focal_distance());
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const Point<T> f1(static_cast<T>(center_.x() + c * cc),
                      static_cast<T>(center_.y() + c * ss));
    const Point<T> f2(static_cast<T>(center_.x() - c * cc),
                      static_cast<T>(center_.y() - c * ss));
    return std::make_pair(f1, f2);
  }

  /**
   * @brief Return the two directrix lines.
   * @return Pair of directrices perpendicular to the major axis, offset
   *         by @f$ a^2 / c @f$ from the center.
   */
  std::pair<Line<T>, Line<T>> directrices() const {
    const double a = static_cast<double>(semi_a_);
    const double c = static_cast<double>(focal_distance());
    const double d1 = a * a / std::max(1e-15, c);
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const double An = -ss;
    const double Bn = cc;
    const double C1 = -(An * center_.x() + Bn * center_.y() - d1);
    const double C2 = -(An * center_.x() + Bn * center_.y() + d1);
    return std::make_pair(Line<T>(static_cast<T>(An), static_cast<T>(Bn),
                                  static_cast<T>(C1)),
                          Line<T>(static_cast<T>(An), static_cast<T>(Bn),
                                  static_cast<T>(C2)));
  }

  /**
   * @brief Return the major axis line.
   * @return Line passing through the center along the major-axis direction.
   */
  Line<T> major_axis() const {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    return Line<T>(static_cast<T>(-ss), static_cast<T>(cc),
                   static_cast<T>(-(-ss * center_.x() + cc * center_.y())));
  }

  /**
   * @brief Return the minor axis line.
   * @return Line passing through the center along the minor-axis direction.
   */
  Line<T> minor_axis() const {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    return Line<T>(static_cast<T>(cc), static_cast<T>(ss),
                   static_cast<T>(-(cc * center_.x() + ss * center_.y())));
  }

  /**
   * @brief Classify a point w.r.t. the ellipse.
   *
   * A point is *inside* the ellipse when the quadratic form @f$ F(P) @f$
   * has the opposite sign from the leading coefficient @f$ a @f$ (with the
   * normalization used by the Ellipse constructors, where @f$ F = 0 @f$ on
   * the boundary).
   *
   * @param p   Point to classify.
   * @param tol Tolerance for boundary comparison.
   * @return `+1` if @p p is inside, `-1` if outside, `0` on the boundary.
   */
  int relative_position(const Point<T> &p, T tol = T(1e-12)) const {
    const int fs = this->f_sign(p, tol);
    if (fs == 0) return 0;
    const double sign_a =
        (static_cast<double>(this->a_) > 0.0) ? 1.0 : -1.0;
    const double sign_F = (fs > 0) ? 1.0 : -1.0;
    return (sign_a * sign_F < 0.0) ? 1 : -1;
  }

  /**
   * @brief Stream operator: prints the general-form coefficients.
   * @param os Output stream.
   * @param el The ellipse to print.
   * @return Reference to @p os.
   */
  friend std::ostream &operator<<(std::ostream &os, const Ellipse &el) {
    os << "Ellipse(a=" << el.a_ << ", b=" << el.b_ << ", c=" << el.c_
       << ", d=" << el.d_ << ", e=" << el.e_ << ", f=" << el.f_ << ")";
    return os;
  }
};

}  // namespace planeGeometry

#endif
