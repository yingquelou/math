#ifndef PLANE_GEOMETRY_PARABOLA_HPP
#define PLANE_GEOMETRY_PARABOLA_HPP

#include <cmath>
#include <limits>
#include <ostream>
#include <stdexcept>

#include "ConicBase.hpp"
#include "conic_tags.hpp"

namespace planeGeometry {

/**
 * @brief Parabola class inheriting from ConicBase using CRTP.
 *
 * Represents a parabola with vertex (cx, cy), signed focal length p (the
 * distance from vertex to focus with sign indicating the opening direction
 * along the axis), and a rotation angle theta of the axis direction.
 *
 * The signed implicit form used internally is
 *   F(x,y) = 4 p (c (x - cx) + s (y - cy))
 *          - (s (x - cx) - c (y - cy))^2
 * where s = sin(theta), c = cos(theta). With this convention F(P) > 0 on
 * the focal (interior) side, F(P) < 0 on the opposite side and F(P) = 0
 * exactly on the parabola. The axis direction (focus direction) is (c, s).
 *
 * Expanding to general form F(x,y) = a x^2 + b y^2 + c x y + d x + e y + f:
 *   a = -s^2,  b = -c^2,  c = 2 s c
 *   d = 4 p c + 2 s^2 cx - 2 s c cy
 *   e = 4 p s + 2 c^2 cy - 2 s c cx
 *   f = -s^2 cx^2 - c^2 cy^2 + 2 s c cx cy
 *       - 4 p c cx - 4 p s cy
 *
 * Eccentricity is always 1.
 *
 * @tparam T Numeric type (e.g., double, float).
 */
template <typename T> class Parabola : public ConicBase<Parabola<T>, T> {
  friend class ConicBase<Parabola<T>, T>;

private:
  Point<T> vertex_;
  T p_;        // signed focal length
  T rotation_; // angle (radians) of the axis direction

  /**
   * @brief Set coefficients from the geometric representation.
   *
   * Computes a,b,c,d,e,f matching
   *   4 p (c (x - cx) + s (y - cy)) - (s (x - cx) - c (y - cy))^2 = 0.
   */
  void set_coeffs_from_vertex_form(double cx, double cy) {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const double pval = static_cast<double>(p_);

    this->a_ = static_cast<T>(-ss * ss);
    this->b_ = static_cast<T>(-cc * cc);
    this->c_ = static_cast<T>(2.0 * ss * cc);
    this->d_ = static_cast<T>(4.0 * pval * cc + 2.0 * ss * ss * cx -
                              2.0 * ss * cc * cy);
    this->e_ = static_cast<T>(4.0 * pval * ss + 2.0 * cc * cc * cy -
                              2.0 * ss * cc * cx);
    this->f_ = static_cast<T>(-ss * ss * cx * cx - cc * cc * cy * cy +
                              2.0 * ss * cc * cx * cy - 4.0 * pval * cc * cx -
                              4.0 * pval * ss * cy);
    vertex_ = Point<T>(static_cast<T>(cx), static_cast<T>(cy));
  }

  /**
   * @brief Recover the geometric representation (vertex, p, rotation)
   * from the currently stored general-form coefficients.
   *
   * Requires Delta = c^2 - 4ab = 0 and (a,b) != (0,0). The algorithm
   * rotates the coordinate system to eliminate the xy term, completes
   * the square in the surviving quadratic variable, and rotates the
   * result back to the original frame. On success this updates
   * vertex_, p_ and rotation_ without modifying a_, b_, c_, d_, e_, f_.
   */
  void recover_vertex_and_parameters() {
    const double A = static_cast<double>(this->a_);
    const double B = static_cast<double>(this->b_);
    const double C = static_cast<double>(this->c_);
    const double D = static_cast<double>(this->d_);
    const double E = static_cast<double>(this->e_);
    const double F0 = static_cast<double>(this->f_);

    if (std::abs(A + B) < 1e-15) {
      throw std::invalid_argument("Parabola: degenerate (a+b ~ 0)");
    }

    // Step 1: rotate to principal axes to eliminate the xy term.
    const double theta = 0.5 * std::atan2(C, A - B);
    const double ct = std::cos(theta);
    const double st = std::sin(theta);

    // Rotated coefficients (A', B', D', E', F').
    const double Ap = A * ct * ct + C * ct * st + B * st * st;
    const double Bp = A * st * st - C * ct * st + B * ct * ct;
    const double Dp = D * ct + E * st;
    const double Ep = -D * st + E * ct;
    const double Fp = F0;

    // Step 2: in rotated coordinates (u, v) the parabola has either
    //   Ap u^2 + Dp u + Ep v + Fp = 0   (if Ap dominates)  or
    //   Bp v^2 + Dp u + Ep v + Fp = 0   (if Bp dominates).
    // Exactly one of {Ap, Bp} is non-zero for a true parabola.
    double u_vtx = 0.0;
    double v_vtx = 0.0;

    if (std::abs(Bp) >= std::abs(Ap)) {
      // Canonical form Bp v^2 + Dp u + Ep v + Fp = 0.
      if (std::abs(Dp) < 1e-12) {
        throw std::invalid_argument(
            "Parabola: degenerate (parallel lines after rotation)");
      }
      v_vtx = -Ep / (2.0 * Bp);
      u_vtx = (Ep * Ep / (4.0 * Bp) - Fp) / Dp;
    } else {
      // Canonical form Ap u^2 + Dp u + Ep v + Fp = 0.
      if (std::abs(Ep) < 1e-12) {
        throw std::invalid_argument(
            "Parabola: degenerate (parallel lines after rotation)");
      }
      u_vtx = -Dp / (2.0 * Ap);
      v_vtx = (Dp * Dp / (4.0 * Ap) - Fp) / Ep;
    }

    // Step 3: rotate the vertex back to original coordinates.
    const double cx = ct * u_vtx - st * v_vtx;
    const double cy = st * u_vtx + ct * v_vtx;

    // Step 4: recover axis direction (c_axis, s_axis) and p.
    // From the implicit form
    //   F(x,y) = 4 p (c_axis (x - cx) + s_axis (y - cy))
    //          - (s_axis (x - cx) - c_axis (y - cy))^2
    // the gradient at the vertex equals 4 p (c_axis, s_axis).
    const double gx = 2.0 * A * cx + C * cy + D;
    const double gy = C * cx + 2.0 * B * cy + E;
    const double gnorm = std::sqrt(gx * gx + gy * gy);
    if (gnorm < 1e-12) {
      throw std::invalid_argument("Parabola: zero gradient at vertex");
    }
    const double c_axis = gx / gnorm;
    const double s_axis = gy / gnorm;
    const double pval = gnorm / 4.0;

    // Sanity check: the recovered vertex must satisfy F(cx, cy) ≈ 0.
    const double Fv =
        A * cx * cx + B * cy * cy + C * cx * cy + D * cx + E * cy + F0;
    if (std::abs(Fv) > 1e-6) {
      throw std::invalid_argument(
          "Parabola: recovered vertex does not lie on the conic");
    }

    rotation_ = static_cast<T>(std::atan2(s_axis, c_axis));
    p_ = static_cast<T>(pval);
    vertex_ = Point<T>(static_cast<T>(cx), static_cast<T>(cy));
  }

public:
  /** @brief Default constructor. Creates the unit parabola y^2 = 4x
   *  (p = 1, vertex at origin, opening along +x axis). */
  Parabola()
      : ConicBase<Parabola<T>, T>(T(0), T(-1), T(0), T(4), T(0), T(0)),
        vertex_(T(0), T(0)), p_(T(1)), rotation_(T(0)) {}

  /**
   * @brief Construct a parabola from vertex, signed focal length p, and
   * rotation angle of the axis direction.
   *
   * @param center Vertex of the parabola.
   * @param p Signed focal length. p > 0 means the parabola opens along
   *          the positive axis direction (cos theta, sin theta).
   * @param rotation Angle (radians) of the axis direction.
   */
  Parabola(const Point<T> &center, T p, T rotation = T())
      : vertex_(center), p_(p), rotation_(rotation) {
    set_coeffs_from_vertex_form(static_cast<double>(center.x()),
                                static_cast<double>(center.y()));
  }

  /**
   * @brief Construct a parabola from general-form coefficients.
   *
   * @throws std::invalid_argument if discriminant c^2 - 4ab != 0, if the
   *         conic is degenerate (a,b = 0,0), or if the recovered vertex
   *         does not satisfy the equation.
   */
  Parabola(T a, T b, T c, T d, T e, T f)
      : ConicBase<Parabola<T>, T>(a, b, c, d, e, f), vertex_(T(0), T(0)),
        p_(T(1)), rotation_(T(0)) {
    const double disc =
        static_cast<double>(ConicBase<Parabola<T>, T>::discriminant(a, b, c));
    if (std::abs(disc) > 1e-9) {
      throw std::invalid_argument(
          "Parabola(a,b,c,d,e,f): discriminant must be 0 (parabola)");
    }
    if (std::abs(static_cast<double>(a) + static_cast<double>(b)) < 1e-15) {
      throw std::invalid_argument(
          "Parabola(a,b,c,d,e,f): not a true parabola (a+b ~ 0)");
    }
    recover_vertex_and_parameters();
  }

  /**
   * @brief Factory: construct a parabola from general-form coefficients.
   *
   * Validates the discriminant and rejects degenerate cases (parallel
   * lines) before recovery. The original coefficients are preserved
   * (only vertex_, p_ and rotation_ are recovered).
   */
  static Parabola from_general(T a, T b, T c, T d, T e, T f) {
    const double disc =
        static_cast<double>(ConicBase<Parabola<T>, T>::discriminant(a, b, c));
    if (std::abs(disc) > 1e-9) {
      throw std::invalid_argument(
          "Parabola::from_general: discriminant must be 0 (parabola)");
    }
    if (std::abs(static_cast<double>(a) + static_cast<double>(b)) < 1e-15) {
      throw std::invalid_argument(
          "Parabola::from_general: not a true parabola (a+b ~ 0)");
    }
    // Detect the degenerate parallel-lines case: after rotating to
    // eliminate xy, both linear coefficients vanish.
    const double theta =
        0.5 * std::atan2(static_cast<double>(c),
                         static_cast<double>(a) - static_cast<double>(b));
    const double cc = std::cos(theta);
    const double ss = std::sin(theta);
    const double Dp = static_cast<double>(d) * cc + static_cast<double>(e) * ss;
    const double Ep =
        -static_cast<double>(d) * ss + static_cast<double>(e) * cc;
    if (std::abs(Dp) < 1e-15 && std::abs(Ep) < 1e-15) {
      throw std::invalid_argument(
          "Parabola::from_general: degenerate (parallel lines)");
    }
    Parabola result;
    result.a_ = a;
    result.b_ = b;
    result.c_ = c;
    result.d_ = d;
    result.e_ = e;
    result.f_ = f;
    result.recover_vertex_and_parameters();
    return result;
  }

  static conic_tag derived_tag() { return parabola_tag{}; }

  /** @brief Return the vertex point. */
  const Point<T> &vertex() const { return vertex_; }
  /** @brief Return the signed focal length. */
  T focal_parameter() const { return p_; }
  /** @brief Return the rotation angle of the axis direction (radians). */
  T rotation_angle() const { return rotation_; }
  /** @brief Eccentricity is always 1 for a parabola. */
  T eccentricity() const { return T(1); }

  /** @brief Return the focus point: vertex + p * (cos theta, sin theta). */
  Point<T> focus() const {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    return Point<T>(vertex_.x() + static_cast<T>(p_ * cc),
                    vertex_.y() + static_cast<T>(p_ * ss));
  }

  /**
   * @brief Return the directrix line.
   *
   * The directrix is perpendicular to the axis direction (c, s) and
   * passes through the vertex shifted by -p along the axis direction.
   * It is parameterized as n x + C = 0 where n = (cos theta, sin theta)
   * and C = p - (cx cos theta + cy sin theta).
   */
  Line<T> directrix() const {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const T nx = static_cast<T>(cc);
    const T ny = static_cast<T>(ss);
    const double Cval =
        static_cast<double>(p_) - (cc * static_cast<double>(vertex_.x()) +
                                   ss * static_cast<double>(vertex_.y()));
    return Line<T>(nx, ny, static_cast<T>(Cval));
  }

  /** @brief Return the axis of symmetry, passing through the vertex
   *  along the direction (-sin theta, cos theta). */
  Line<T> axis() const {
    const double cc = std::cos(static_cast<double>(rotation_));
    const double ss = std::sin(static_cast<double>(rotation_));
    const double Cval = -(-ss * static_cast<double>(vertex_.x()) +
                          cc * static_cast<double>(vertex_.y()));
    return Line<T>(static_cast<T>(-ss), static_cast<T>(cc),
                   static_cast<T>(Cval));
  }

  /** @brief Length of the latus rectum = 4 * |p|. */
  T latus_rectum_length() const { return static_cast<T>(4) * p_; }

  /** @brief Relative position w.r.t. the parabola.
   *  @return +1 if P is on the focal (interior) side,
   *          -1 if on the opposite side,
   *           0 if exactly on the boundary. */
  int relative_position(const Point<T> &p, T tol = T(1e-12)) const {
    return this->f_sign(p, tol);
  }

  /** @brief Returns true if the point lies strictly on the focal
   *  (interior) side of the parabola. */
  bool contains(const Point<T> &p, T tol = T(1e-12)) const {
    return relative_position(p, tol) == 1;
  }

  friend std::ostream &operator<<(std::ostream &os, const Parabola &pb) {
    os << "Parabola(a=" << pb.a_ << ", b=" << pb.b_ << ", c=" << pb.c_
       << ", d=" << pb.d_ << ", e=" << pb.e_ << ", f=" << pb.f_
       << ", vertex=" << pb.vertex_ << ", p=" << pb.p_
       << ", rotation=" << pb.rotation_ << ")";
    return os;
  }
};

} // namespace planeGeometry

#endif
