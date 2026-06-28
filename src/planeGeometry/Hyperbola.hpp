#ifndef PLANE_GEOMETRY_HYPERBOLA_HPP
#define PLANE_GEOMETRY_HYPERBOLA_HPP

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "ConicBase.hpp"
#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief Hyperbola class inheriting from ConicBase using CRTP.
 *
 * Represents a hyperbola with center @f$ (c_x, c_y) @f$, real (transverse)
 * semi-axis @f$ a @f$, imaginary (conjugate) semi-axis @f$ b @f$, and a
 * rotation angle @f$ \theta @f$ of the transverse axis.
 *
 * The canonical form (aligned with the axes) is
 * @math{
 *   \frac{x^2}{a^2} - \frac{y^2}{b^2} = 1.
 * }
 *
 * Eccentricity is @f$ e = c / a @f$ with @f$ c = \sqrt{a^2 + b^2} @f$,
 * always @f$ e > 1 @f$.
 *
 * @tparam T Numeric type (double, float, etc.).
 *
 * @note Class invariant:
 *   @li `real_semi_axis() >= imag_semi_axis()` (enforced in all
 *       constructors; when the user supplies @f$ a < b @f$ the axes are
 *       swapped and the rotation is adjusted by @f$ \pi/2 @f$).
 */
template <typename T>
class Hyperbola : public ConicBase<Hyperbola<T>, T> {
  friend class ConicBase<Hyperbola<T>, T>;

 private:
  Point<T> center_;
  T real_a_;     // real semi-axis (transverse)
  T imag_b_;     // imaginary semi-axis (conjugate)
  T rotation_;   // angle (radians) of the transverse axis

  /**
   * @brief Recompute geometric representation (center, semi-axes, rotation)
   *        from the currently stored general-form coefficients.
   *
   * After recovery the axes are re-ordered so that
   * @f$ a \ge b @f$; if a swap is required the rotation is adjusted by
   * @f$ \pi/2 @f$.
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
      real_a_ = T(1);
      imag_b_ = T(1);
      rotation_ = T();
      return;
    }

    const double theta = 0.5 * std::atan2(C, A - B);
    rotation_ = static_cast<T>(theta);

    const double den = C * C - 4.0 * A * B;
    if (std::abs(den) < 1e-15) {
      center_ = Point<T>(T(), T());
      real_a_ = T(1);
      imag_b_ = T(1);
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

    if (std::abs(Ap) < 1e-15 || std::abs(Bp) < 1e-15 || std::abs(Fp) < 1e-15) {
      real_a_ = T(1);
      imag_b_ = T(1);
      return;
    }

    // Canonical form after rotation:  Ap U^2 + Bp V^2 = -Fp
    // where U = c(x-cx) + s(y-cy), V = -s(x-cx) + c(y-cy).
    // With signs s_A = sign(Ap), s_B = sign(Bp):
    //   (s_A Ap) U^2 + (s_B Bp) V^2 = -Fp   =>  a^2 = -Fp/(s_A Ap), b^2 = -Fp/(s_B Bp)
    const double sA = (Ap >= 0.0) ? 1.0 : -1.0;
    const double sB = (Bp >= 0.0) ? 1.0 : -1.0;
    double a2 = -Fp / (sA * Ap);
    double b2 = -Fp / (sB * Bp);
    if (a2 <= 0.0) a2 = 1.0;
    if (b2 <= 0.0) b2 = 1.0;
    double a = std::sqrt(a2);
    double b = std::sqrt(b2);
    if (a < b) {
      using std::swap;
      swap(a, b);
      // Swap the roles of transverse/conjugate axes by rotating pi/2.
      rotation_ = static_cast<T>(theta + 0.5 * M_PI);
      if (rotation_ > static_cast<T>(M_PI)) {
        rotation_ = static_cast<T>(rotation_ - 2.0 * M_PI);
      }
    }
    real_a_ = static_cast<T>(a);
    imag_b_ = static_cast<T>(b);
  }

 public:
  /**
   * @brief Default constructor. Creates the unit hyperbola
   *        @f$ x^2 - y^2 = 1 @f$.
   */
  Hyperbola()
      : ConicBase<Hyperbola<T>, T>(T(), T(), T(), T(), T(), T()),
        center_(T(), T(0)),
        real_a_(T(1)),
        imag_b_(T(1)),
        rotation_(T()) {
    // Unit hyperbola x^2 - y^2 = 1: a = b = 1, center at origin.
    // A = 1, B = -1, C = D = E = 0, F = -1.
    this->a_ = T(1);
    this->b_ = T(-1);
    this->c_ = T(0);
    this->d_ = T(0);
    this->e_ = T(0);
    this->f_ = T(-1);
  }

  /**
   * @brief Construct a hyperbola from general-form coefficients.
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @param d Coefficient @f$ d @f$.
   * @param e Coefficient @f$ e @f$.
   * @param f Constant term @f$ f @f$.
   */
  Hyperbola(T a, T b, T c, T d, T e, T f)
      : ConicBase<Hyperbola<T>, T>(a, b, c, d, e, f),
        center_(T(), T(0)),
        real_a_(T(1)),
        imag_b_(T(1)),
        rotation_(T()) {
    recompute_from_coeffs();
  }

  /**
   * @brief Construct a hyperbola from center, real/imaginary semi-axes
   *        and rotation.
   *
   * If @p a < @p b the two values are swapped and the rotation is
   * adjusted by @f$ \pi/2 @f$.
   *
   * @param center   Center of the hyperbola.
   * @param a        Real (transverse) semi-axis length.
   * @param b        Imaginary (conjugate) semi-axis length.
   * @param rotation Rotation angle of the transverse axis (radians).
   */
  Hyperbola(const Point<T> &center, T a, T b, T rotation = T())
      : center_(center),
        real_a_(a),
        imag_b_(b),
        rotation_(rotation) {
    if (real_a_ < imag_b_) {
      using std::swap;
      swap(real_a_, imag_b_);
      rotation_ = static_cast<T>(rotation + 0.5 * M_PI);
      if (rotation_ > static_cast<T>(M_PI)) {
        rotation_ = static_cast<T>(rotation_ - 2.0 * M_PI);
      }
    }
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const double inv_a2 = 1.0 / (static_cast<double>(real_a_) *
                                  static_cast<double>(real_a_));
    const double inv_b2 = 1.0 / (static_cast<double>(imag_b_) *
                                  static_cast<double>(imag_b_));
    // With shifted coordinates x' = x - cx, y' = y - cy, the hyperbola is
    //   (c x' + s y')^2 / a^2 - (-s x' + c y')^2 / b^2 = 1.
    // Expanding and collecting x^2, y^2, xy, x, y and constant terms gives
    //   Ac = c^2/a^2 - s^2/b^2,  Bc = s^2/a^2 - c^2/b^2,
    //   Cc = 2 c s (1/a^2 + 1/b^2),
    //   Dcoef = -(2 Ac cx + Cc cy),
    //   Ecoef = -(2 Bc cy + Cc cx),
    //   Fcoef = Ac cx^2 + Bc cy^2 + Cc cx cy - 1.
    const double Ac = cc * cc * inv_a2 - ss * ss * inv_b2;
    const double Bc = ss * ss * inv_a2 - cc * cc * inv_b2;
    const double Cc = 2.0 * cc * ss * (inv_a2 + inv_b2);
    const double cx = static_cast<double>(center.x());
    const double cy = static_cast<double>(center.y());
    const double Dcoef = -(2.0 * Ac * cx + Cc * cy);
    const double Ecoef = -(2.0 * Bc * cy + Cc * cx);
    const double Fcoef =
        Ac * cx * cx + Bc * cy * cy + Cc * cx * cy - 1.0;
    this->a_ = static_cast<T>(Ac);
    this->b_ = static_cast<T>(Bc);
    this->c_ = static_cast<T>(Cc);
    this->d_ = static_cast<T>(Dcoef);
    this->e_ = static_cast<T>(Ecoef);
    this->f_ = static_cast<T>(Fcoef);
  }

  /**
   * @brief Construct a hyperbola from general-form coefficients.
   *
   * Validates that the discriminant satisfies
   * @f$ \Delta = c^2 - 4 a b > 0 @f$.
   *
   * @tparam T Numeric type.
   * @param a Coefficient @f$ a @f$.
   * @param b Coefficient @f$ b @f$.
   * @param c Coefficient @f$ c @f$.
   * @param d Coefficient @f$ d @f$.
   * @param e Coefficient @f$ e @f$.
   * @param f Constant term @f$ f @f$.
   * @return A fully-reconstructed `Hyperbola`.
   * @throws std::invalid_argument if @f$ \Delta \le 0 @f$.
   */
  static Hyperbola from_general(T a, T b, T c, T d, T e, T f) {
    const double disc =
        static_cast<double>(ConicBase<Hyperbola<T>, T>::discriminant(a, b, c));
    if (disc <= 0.0) {
      throw std::invalid_argument(
          "Hyperbola::from_general: discriminant must be > 0");
    }
    Hyperbola result;
    result.a_ = a;
    result.b_ = b;
    result.c_ = c;
    result.d_ = d;
    result.e_ = e;
    result.f_ = f;
    result.recompute_from_coeffs();
    return result;
  }

  /** @brief Return the conic tag (`hyperbola_tag`). */
  static conic_tag derived_tag() { return hyperbola_tag{}; }

  /**
   * @brief Return the center point.
   * @return Const reference to the center.
   */
  const Point<T> &center() const { return center_; }

  /**
   * @brief Return the real (transverse) semi-axis length @f$ a @f$.
   *
   * Guaranteed to satisfy `real_semi_axis() >= imag_semi_axis()`.
   *
   * @return Real semi-axis length.
   */
  T real_semi_axis() const { return real_a_; }

  /**
   * @brief Return the imaginary (conjugate) semi-axis length @f$ b @f$.
   *
   * Guaranteed to satisfy `imag_semi_axis() <= real_semi_axis()`.
   *
   * @return Imaginary semi-axis length.
   */
  T imag_semi_axis() const { return imag_b_; }

  /**
   * @brief Return the rotation angle of the transverse axis (radians).
   * @return Rotation angle in radians.
   */
  T rotation_angle() const { return rotation_; }

  /**
   * @brief Return the focal distance @f$ c = \sqrt{a^2 + b^2} @f$.
   * @return Distance from the center to each focus.
   */
  T focal_distance() const {
    const double a = static_cast<double>(real_a_);
    const double b = static_cast<double>(imag_b_);
    return static_cast<T>(std::sqrt(a * a + b * b));
  }

  /**
   * @brief Eccentricity @f$ e = c / a @f$ with @f$ e > 1 @f$.
   * @return Eccentricity (always @f$ > 1 @f$).
   */
  T eccentricity() const {
    const double a = static_cast<double>(real_a_);
    const double c = static_cast<double>(focal_distance());
    return static_cast<T>(c / a);
  }

  /**
   * @brief Return the two focal points.
   * @return Pair of foci along the transverse axis, offset by `focal_distance()`.
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
   *
   * The directrices are perpendicular to the transverse direction and
   * pass through the center offset by @f$ \pm a^2 / c @f$.
   *
   * @return Pair of directrix lines.
   */
  std::pair<Line<T>, Line<T>> directrices() const {
    const double a = static_cast<double>(real_a_);
    const double c = static_cast<double>(focal_distance());
    const double d1 = a * a / std::max(1e-15, c);
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    // Normal to directrices = transverse direction = (cc, ss).
    const double An = cc;
    const double Bn = ss;
    const double C1 = -(An * center_.x() + Bn * center_.y() - d1);
    const double C2 = -(An * center_.x() + Bn * center_.y() + d1);
    return std::make_pair(Line<T>(static_cast<T>(An), static_cast<T>(Bn),
                                  static_cast<T>(C1)),
                          Line<T>(static_cast<T>(An), static_cast<T>(Bn),
                                  static_cast<T>(C2)));
  }

  /**
   * @brief Return the two asymptote lines.
   *
   * In the aligned (u, v) coordinates the asymptotes are
   * @f$ v = \pm (b/a) u @f$. Expanding back to Cartesian coordinates
   * produces two lines through the center with slopes @f$ \pm b/a @f$
   * relative to the transverse direction.
   *
   * @return Pair of asymptote lines.
   */
  std::pair<Line<T>, Line<T>> asymptotes() const {
    const double a = static_cast<double>(real_a_);
    const double b = static_cast<double>(imag_b_);
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const double r = b / std::max(1e-15, a);
    double A1 = -ss - r * cc;
    double B1 = cc - r * ss;
    double C1 = (ss * center_.x() - cc * center_.y()) -
                r * (cc * center_.x() + ss * center_.y());
    double A2 = -ss + r * cc;
    double B2 = cc + r * ss;
    double C2 = (ss * center_.x() - cc * center_.y()) +
                r * (cc * center_.x() + ss * center_.y());
    return std::make_pair(Line<T>(static_cast<T>(A1), static_cast<T>(B1),
                                  static_cast<T>(C1)),
                          Line<T>(static_cast<T>(A2), static_cast<T>(B2),
                                  static_cast<T>(C2)));
  }

  /**
   * @brief Classify a point w.r.t. the hyperbola.
   *
   * Convention (chosen to match the geometric "branch" region):
   *   @li @f$ F(P) > 0 @f$: inside (on the same side as the foci / a branch).
   *   @li @f$ F(P) < 0 @f$: outside (in the valley between the two branches).
   *   @li @f$ F(P) = 0 @f$: on boundary.
   *
   * @param p   Point to classify.
   * @param tol Tolerance for boundary comparison.
   * @return `+1`, `-1` or `0` as described above.
   */
  int relative_position(const Point<T> &p, T tol = T(1e-12)) const {
    return this->f_sign(p, tol);
  }

  /**
   * @brief Test whether a line separates the two branches of the hyperbola.
   *
   * A line @f$ L @f$ separates the two branches when the two foci lie on
   * opposite sides of @f$ L @f$. Equivalently, the branch regions (the
   * connected components of @f$ F(P) > 0 @f$) are mapped to opposite
   * half-planes defined by @f$ L @f$.
   *
   * @param line Line to test.
   * @param tol  Tolerance for the signed-distance comparison.
   * @return `true` if the line separates the two branches, `false` otherwise.
   */
  bool branches_separated(const Line<T> &line, T tol = T(1e-9)) const {
    const auto f = focal_points();
    const double d1 = line.distance_to(f.first);
    const double d2 = line.distance_to(f.second);
    if (d1 <= tol || d2 <= tol) return false;
    const double sign1 =
        (static_cast<double>(line.A()) * static_cast<double>(f.first.x()) +
         static_cast<double>(line.B()) * static_cast<double>(f.first.y()) +
         static_cast<double>(line.C()) > 0.0)
            ? 1.0
            : -1.0;
    const double sign2 =
        (static_cast<double>(line.A()) * static_cast<double>(f.second.x()) +
         static_cast<double>(line.B()) * static_cast<double>(f.second.y()) +
         static_cast<double>(line.C()) > 0.0)
            ? 1.0
            : -1.0;
    (void)d1;
    (void)d2;
    return sign1 * sign2 < 0.0;
  }

  /**
   * @brief Stream operator: prints the general-form coefficients and the
   *        recovered geometric parameters.
   * @param os Output stream.
   * @param h  The hyperbola to print.
   * @return Reference to @p os.
   */
  friend std::ostream &operator<<(std::ostream &os, const Hyperbola &h) {
    os << "Hyperbola(a=" << h.a_ << ", b=" << h.b_ << ", c=" << h.c_
       << ", d=" << h.d_ << ", e=" << h.e_ << ", f=" << h.f_
       << ", center=" << h.center_ << ", real_a=" << h.real_a_
       << ", imag_b=" << h.imag_b_ << ", rotation=" << h.rotation_ << ")";
    return os;
  }
};

}  // namespace planeGeometry

#endif
