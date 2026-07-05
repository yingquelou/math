#ifndef PLANE_GEOMETRY_CONIC_INTERSECT_HPP
#define PLANE_GEOMETRY_CONIC_INTERSECT_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "Circle.hpp"
#include "ConicBase.hpp"
#include "Ellipse.hpp"
#include "Hyperbola.hpp"
#include "Line.hpp"
#include "Parabola.hpp"
#include "Point.hpp"
#include "conic_factory.hpp"

namespace planeGeometry {

/**
 * @brief A type-tagged visitor that extracts a concrete conic of type
 *        @p ConicType from a `conic_variant<T>` via `visit`.
 *
 * The matching overload returns the extracted conic; the non-matching
 * overloads return a default-constructed object of @p ConicType.
 *
 * @tparam T          Numeric type (matches the variant).
 * @tparam ConicType Concrete conic type to extract.
 */
template <typename T, typename ConicType> struct conic_extractor {
  typedef ConicType result_t;
  result_t operator()() const { return result_t(); }
  result_t operator()(const Circle<T> &v) const { return select(v); }
  result_t operator()(const Ellipse<T> &v) const { return select(v); }
  result_t operator()(const Parabola<T> &v) const { return select(v); }
  result_t operator()(const Hyperbola<T> &v) const { return select(v); }

private:
  template <typename U>
  typename std::enable_if<std::is_same<U, ConicType>::value, ConicType>::type
  select(const U &u) const {
    return u;
  }
  template <typename U>
  typename std::enable_if<!std::is_same<U, ConicType>::value, ConicType>::type
  select(const U &) const {
    return ConicType();
  }
};

/**
 * @brief Intersection of two conic sections.
 *
 * Two distinct conics defined by their general quadratic forms
 * @f$ F(x,y) = a_1 x^2 + b_1 y^2 + c_1 x y + d_1 x + e_1 y + f_1 @f$ and
 * @f$ G(x,y) = a_2 x^2 + b_2 y^2 + c_2 x y + d_2 x + e_2 y + f_2 @f$
 * have at most four intersection points (Bezout's theorem).
 *
 * The solver uses the classical approach:
 *   - Build the @a resultant of @a F and @a G with respect to one
 *     variable (Sylvester's 4×4 determinant), which is a degree-4
 *     polynomial in the remaining variable.
 *   - Extract the real roots of that quartic.
 *   - For each root, solve the resulting (at most quadratic) equation
 *     in the other variable and refine via Newton iteration on the
 *     original conic polynomials.
 *
 * Overlapping points are deduplicated up to a configurable tolerance.
 *
 * @tparam T Numeric type (e.g. double).
 */
template <typename T> class conic_intersect {
public:
  /** @brief Maximum number of real intersection points (Bezout bound). */
  static constexpr std::size_t kMaxPoints = 4;

  /** @brief Real intersection points. Size <= @p kMaxPoints. */
  std::vector<Point<T>> points;

  /** @brief Default constructor: empty intersection. */
  conic_intersect() = default;

  /**
   * @brief Dispatch two `conic_variant<T>` objects through the visitor
   *        machinery and compute their intersection.
   *
   * Helper used by the free `intersect()` function.
   */
  static conic_intersect dispatch_variant(conic_variant<T> &f,
                                          conic_variant<T> &g, double tol) {
    // Use 'kind' enum (same underlying value as index_t).
    using kind_t = typename detail::conic_storage<T>::kind;
    const kind_t fk = static_cast<kind_t>(f.which());
    const kind_t gk = static_cast<kind_t>(g.which());
    if (fk == kind_t::monostate_k || gk == kind_t::monostate_k) {
      return conic_intersect();
    }
    // Direct access via a public helper on the variant.
    // We call get_copy<ConicType>(variant) for both sides.
    switch (fk) {
    case kind_t::circle_k:
      return dispatch_f<Circle<T>>(f, g, gk, tol);
    case kind_t::ellipse_k:
      return dispatch_f<Ellipse<T>>(f, g, gk, tol);
    case kind_t::parabola_k:
      return dispatch_f<Parabola<T>>(f, g, gk, tol);
    case kind_t::hyperbola_k:
      return dispatch_f<Hyperbola<T>>(f, g, gk, tol);
    default:
      break;
    }
    return conic_intersect();
  }

private:
  template <typename ConicA>
  static conic_intersect dispatch_f(conic_variant<T> &f, conic_variant<T> &g,
                                    typename detail::conic_storage<T>::kind gk,
                                    double tol) {
    using kind_t = typename detail::conic_storage<T>::kind;
    // Extract f's value via a visitor.  Use std::move to force rvalue
    // deduction of the visitor template parameter, otherwise the
    // `Visitor::result_t` substitution in `variant::visit` fails with
    // an lvalue reference.
    conic_extractor<T, ConicA> exA;
    conic_variant<T> &fc = f;
    ConicA fa = fc.visit(std::move(exA));
    switch (gk) {
    case kind_t::circle_k:
      return conic_intersect::compute(fa, extract_g<T, Circle<T>>(g), tol);
    case kind_t::ellipse_k:
      return conic_intersect::compute(fa, extract_g<T, Ellipse<T>>(g), tol);
    case kind_t::parabola_k:
      return conic_intersect::compute(fa, extract_g<T, Parabola<T>>(g), tol);
    case kind_t::hyperbola_k:
      return conic_intersect::compute(fa, extract_g<T, Hyperbola<T>>(g), tol);
    default:
      break;
    }
    return conic_intersect();
  }

  template <typename T_, typename ConicType>
  static ConicType extract_g(conic_variant<T_> &g) {
    conic_extractor<T_, ConicType> ex;
    return g.visit(std::move(ex));
  }

public:
  /**
   * @brief Compute the intersection of two conic sections.
   *
   * Accepts any of the concrete conic types
   * (Circle, Ellipse, Parabola, Hyperbola). The two conics may be of
   * different concrete types.
   *
   * @tparam ConicA Type of the first conic.
   * @tparam ConicB Type of the second conic.
   * @param f First conic.
   * @param g Second conic.
   * @param tol Tolerance for root extraction / duplicate removal.
   * @return A `conic_intersect` whose @p points field contains the
   *         (possibly empty) set of intersection points.
   */
  template <typename ConicA, typename ConicB>
  static conic_intersect compute(const ConicA &f, const ConicB &g,
                                 double tol = 1e-12) {
    const double a1 = static_cast<double>(f.coef_x2());
    const double b1 = static_cast<double>(f.coef_y2());
    const double c1 = static_cast<double>(f.coef_xy());
    const double d1 = static_cast<double>(f.coef_x1());
    const double e1 = static_cast<double>(f.coef_y1());
    const double ff1 = static_cast<double>(f.coef_c());

    const double a2 = static_cast<double>(g.coef_x2());
    const double b2 = static_cast<double>(g.coef_y2());
    const double c2 = static_cast<double>(g.coef_xy());
    const double d2 = static_cast<double>(g.coef_x1());
    const double e2 = static_cast<double>(g.coef_y1());
    const double ff2 = static_cast<double>(g.coef_c());

    conic_intersect result;
    std::vector<Point<T>> raw;

    bool eliminated_y = (std::abs(b1) > tol) || (std::abs(b2) > tol);
    if (eliminated_y) {
      eliminate_and_solve_y(a1, b1, c1, d1, e1, ff1, a2, b2, c2, d2, e2, ff2,
                            tol, raw);
    } else if ((std::abs(a1) > tol) || (std::abs(a2) > tol)) {
      // Swap x <-> y (and a <-> b, d <-> e), eliminate y in the swapped
      // system, then swap back the result points.
      std::vector<Point<T>> swapped;
      eliminate_and_solve_y(b1, a1, c1, e1, d1, ff1, b2, a2, c2, e2, d2, ff2,
                            tol, swapped);
      for (const auto &p : swapped) {
        raw.push_back(Point<T>(p.y(), p.x()));
      }
    } else {
      // Both linear in x and y -> at most one intersection (line-line).
      Point<T> p;
      Line<T> l1(static_cast<T>(d1), static_cast<T>(e1), static_cast<T>(ff1));
      Line<T> l2(static_cast<T>(d2), static_cast<T>(e2), static_cast<T>(ff2));
      if (l1.intersection_with(l2, p))
        raw.push_back(p);
    }

    // --- Dedup & refine -------------------------------------------------
    for (const auto &p : raw) {
      bool dup = false;
      for (const auto &q : result.points) {
        const double dx = static_cast<double>(p.x() - q.x());
        const double dy = static_cast<double>(p.y() - q.y());
        if (dx * dx + dy * dy < 1e-6) {
          dup = true;
          break;
        }
      }
      if (!dup && result.points.size() < kMaxPoints) {
        Point<T> q = p;
        refine_on_conics(q, a1, b1, c1, d1, e1, ff1, a2, b2, c2, d2, e2, ff2);
        result.points.push_back(q);
      }
    }
    return result;
  }

private:
  /**
   * @brief Eliminate @f$ y @f$ from the two conics, producing a
   *        polynomial in @f$ x @f$ whose roots give the @f$ x @f$
   *        coordinates of the intersections.
   *
   * @param a1,b1,c1,d1,e1,f1 Coefficients of the first conic.
   * @param a2,b2,c2,d2,e2,f2 Coefficients of the second conic.
   * @param tol Tolerance.
   * @param out Output: intersection points (unsorted, possibly dup).
   */
  static void eliminate_and_solve_y(double a1, double b1, double c1, double d1,
                                    double e1, double ff1, double a2, double b2,
                                    double c2, double d2, double e2, double ff2,
                                    double tol, std::vector<Point<T>> &out) {
    // Write F and G as polynomials in y with coefficients in R[x]:
    //   F(x, y) = b1 y^2 + c1 y + R1(x)
    //   G(x, y) = b2 y^2 + c2 y + R2(x)
    // where R1(x) = a1 x^2 + d1 x + f1, R2(x) = a2 x^2 + d2 x + f2.
    //
    // Step 1 — eliminate y^2:
    //   H(x, y) = b2 F - b1 G = (b2 c1 - b1 c2) y + (b2 R1 - b1 R2)
    //            =:  C y + D(x)
    // where D(x) is a degree-2 polynomial in x.
    //
    // If C = 0 (i.e. b1 c2 - b2 c1 = 0, including the case b1 = b2 and
    // c1 = c2), then H = D(x) = 0 gives a degree-2 polynomial whose
    // roots (when they also satisfy F = 0, which is automatic in
    // the projective sense) are the x-coordinates of intersections.
    // Otherwise, we solve y = -D(x) / C and plug back into F to obtain
    // a degree-4 polynomial in x.
    //
    // In what follows we always work with ascending-power order
    // coefficient vectors of polynomials in x.

    // Coefficients of R1(x) = a1 x^2 + d1 x + ff1 (ascending order)
    const double R1_coef[3] = {ff1, d1, a1};
    const double R2_coef[3] = {ff2, d2, a2};

    // Build C = b2 c1 - b1 c2, and coefficients of D(x) = b2 R1 - b1 R2
    const double C = b2 * c1 - b1 * c2;
    double D_coef[3];
    for (int k = 0; k < 3; ++k) {
      D_coef[k] = b2 * R1_coef[k] - b1 * R2_coef[k];
    }

    // Output polynomial q (in ascending-power order) whose roots we want.
    std::vector<double> q;
    if (std::abs(C) < tol) {
      // D(x) = 0 directly; q(x) = D(x), degree <= 2.
      q.assign(D_coef, D_coef + 3);
    } else {
      // y(x) = -D(x) / C.  Each coefficient of y is -1/C times the
      // corresponding coefficient of D.
      double y_coef[3];
      for (int k = 0; k < 3; ++k)
        y_coef[k] = -D_coef[k] / C;

      // Substitute into F(x, y) = b1 y^2 + c1 y + R1(x) = 0:
      //   b1 y(x)^2 + c1 y(x) + R1(x) = 0
      // Build q(x) = b1 * y(x)^2 + c1 * y(x) + R1(x).
      auto poly_scale = [](const std::vector<double> &p, double s) {
        std::vector<double> r(p.size());
        for (size_t i = 0; i < p.size(); ++i)
          r[i] = s * p[i];
        return r;
      };
      auto poly_add = [](const std::vector<double> &p,
                         const std::vector<double> &q) {
        size_t n = std::max(p.size(), q.size());
        std::vector<double> r(n, 0.0);
        for (size_t i = 0; i < p.size(); ++i)
          r[i] += p[i];
        for (size_t i = 0; i < q.size(); ++i)
          r[i] += q[i];
        return r;
      };
      auto poly_mul = [](const std::vector<double> &p,
                         const std::vector<double> &q) {
        std::vector<double> r(p.size() + q.size() - 1, 0.0);
        for (size_t i = 0; i < p.size(); ++i) {
          for (size_t j = 0; j < q.size(); ++j) {
            r[i + j] += p[i] * q[j];
          }
        }
        return r;
      };

      std::vector<double> y_vec(y_coef, y_coef + 3);
      std::vector<double> y2 = poly_mul(y_vec, y_vec); // y(x)^2, degree 4
      std::vector<double> y2b1 = poly_scale(y2, b1);
      std::vector<double> yc = poly_scale(y_vec, c1);
      std::vector<double> R1(R1_coef, R1_coef + 3);
      q = poly_add(y2b1, poly_add(yc, R1));
    }

    // Strip leading (highest-degree) near-zero coefficients.
    while (q.size() > 1 && std::abs(q.back()) < 1e-14)
      q.pop_back();

    // Solve q(x) = 0 for real x.
    std::vector<double> xs;
    if (q.size() == 1) {
      // Constant nonzero polynomial: no finite roots.
      return;
    }
    if (q.size() == 2) {
      // Linear: q[0] + q[1] x = 0.
      double x = -q[0] / q[1];
      if (std::isfinite(x))
        xs.push_back(x);
    } else if (q.size() == 3) {
      // Quadratic: q[0] + q[1] x + q[2] x^2 = 0.
      solve_quadratic_real(q[2], q[1], q[0], xs, tol);
    } else if (q.size() == 4) {
      // Cubic: q[0] + ... + q[3] x^3 = 0.
      solve_cubic_real(q[3], q[2], q[1], q[0], xs, tol);
    } else {
      // Quartic or higher: reduce to degree-4 if possible via deflation;
      // otherwise treat as quartic (with trailing near-zero coefficients
      // stripped above).
      if (q.size() > 5)
        q.resize(5);
      double quartic[5];
      for (int i = 0; i < 5; ++i)
        quartic[i] = q[4 - i];
      solve_quartic_real(quartic, xs, tol);
    }

    for (double x : xs) {
      // Plug x into F and G -> two quadratics in y. Solve using the
      // conic with the larger leading coefficient in y.
      const double A1y = b1;
      const double B1y = c1;
      const double C1y = a1 * x * x + d1 * x + ff1;
      const double A2y = b2;
      const double B2y = c2;
      const double C2y = a2 * x * x + d2 * x + ff2;

      std::vector<double> ys;
      if (std::abs(A1y) > std::abs(A2y)) {
        solve_quadratic_real(A1y, B1y, C1y, ys, tol);
      } else if (std::abs(A2y) > std::abs(A1y)) {
        solve_quadratic_real(A2y, B2y, C2y, ys, tol);
      } else if (std::abs(B1y) > std::abs(B2y)) {
        if (std::abs(B1y) > tol)
          ys.push_back(-C1y / B1y);
      } else if (std::abs(B2y) > tol) {
        ys.push_back(-C2y / B2y);
      } else {
        // Both |A1y| ~ |A2y| and both |B1y| ~ |B2y| are zero or
        // nearly zero.  Use the equation with the smaller constant
        // term (in magnitude) to solve for y directly: A y^2 + C = 0.
        if (std::abs(A1y) > tol) {
          const double disc = -C1y / A1y;
          if (disc >= 0.0) {
            const double sq = std::sqrt(disc);
            ys.push_back(sq);
            if (sq > tol)
              ys.push_back(-sq);
          }
        } else if (std::abs(A2y) > tol) {
          const double disc = -C2y / A2y;
          if (disc >= 0.0) {
            const double sq = std::sqrt(disc);
            ys.push_back(sq);
            if (sq > tol)
              ys.push_back(-sq);
          }
        }
      }

      for (double y : ys) {
        const double err_f = b1 * y * y + c1 * y + a1 * x * x + d1 * x + ff1;
        const double err_g = b2 * y * y + c2 * y + a2 * x * x + d2 * x + ff2;
        if (std::abs(err_f) < 1e-6 && std::abs(err_g) < 1e-6) {
          out.push_back(Point<T>(static_cast<T>(x), static_cast<T>(y)));
        }
      }
    }
  }

  /** @brief 4x4 determinant (general-purpose). */
  static double det4(double M[4][4]) {
    return M[0][0] * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                      M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
                      M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])) -
           M[0][1] * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                      M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                      M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])) +
           M[0][2] * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
                      M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                      M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0])) -
           M[0][3] * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
                      M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
                      M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
  }

  /**
   * @brief Compute coefficients of @f$ (a z^2 + b z + c)^p @f$,
   *        returning the vector of coefficients
   *        @f$ [z^0, z^1, ..., z^{2p}] @f$ (ascending power order).
   */
  static std::vector<double> poly_pow(double a, double b, double c, int p) {
    if (p == 0)
      return {1.0};
    // Compute (a x^2 + b x + c)^p in ascending-power order [x^0, x^1, ...].
    //
    // Given p(x) = sum_{k=0}^n p_k x^k stored as [p_0, p_1, ..., p_n],
    // the product q(x) = (a x^2 + b x + c) p(x) has
    //   q_k = c p_k + b p_{k-1} + a p_{k-2}   (with p_{-1} = p_{-2} = 0)
    // The resulting polynomial has degree n+2 (at most).
    std::vector<double> result = {c}; // degree-0 polynomial "c"
    for (int i = 0; i < p; ++i) {
      const int n = (int)result.size();  // current degree = n - 1
      std::vector<double> v(n + 2, 0.0); // degree increases by 2
      for (int k = 0; k < n + 2; ++k) {
        double s = c * (k < n ? result[k] : 0.0) +
                   b * (k >= 1 && k - 1 < n ? result[k - 1] : 0.0) +
                   a * (k >= 2 && k - 2 < n ? result[k - 2] : 0.0);
        v[k] = s;
      }
      result = std::move(v);
    }
    return result;
  }

  /**
   * @brief Newton refinement on both conic polynomials simultaneously.
   */
  static void refine_on_conics(Point<T> &p, double a1, double b1, double c1,
                               double d1, double e1, double ff1, double a2,
                               double b2, double c2, double d2, double e2,
                               double ff2) {
    double x = static_cast<double>(p.x());
    double y = static_cast<double>(p.y());
    for (int iter = 0; iter < 10; ++iter) {
      const double Fx = 2.0 * a1 * x + c1 * y + d1;
      const double Fy = 2.0 * b1 * y + c1 * x + e1;
      const double Gx = 2.0 * a2 * x + c2 * y + d2;
      const double Gy = 2.0 * b2 * y + c2 * x + e2;
      const double Fv =
          a1 * x * x + b1 * y * y + c1 * x * y + d1 * x + e1 * y + ff1;
      const double Gv =
          a2 * x * x + b2 * y * y + c2 * x * y + d2 * x + e2 * y + ff2;
      const double det = Fx * Gy - Fy * Gx;
      if (std::abs(det) < 1e-18)
        break;
      const double dx = -(Fv * Gy - Fy * Gv) / det;
      const double dy = -(Fx * Gv - Fv * Gx) / det;
      x += dx;
      y += dy;
      if (dx * dx + dy * dy < 1e-20)
        break;
    }
    p = Point<T>(static_cast<T>(x), static_cast<T>(y));
  }

  /** @brief Solve 6x6 linear system via Gaussian elimination with partial
   *         pivoting (in-place on @p A and @p b, returns solution in
   *         @p x). */
  static void solve_linear_6x6(double A[6][6], double b[6], double x[6]) {
    for (int col = 0; col < 6; ++col) {
      int pivot = col;
      double max_val = std::abs(A[col][col]);
      for (int row = col + 1; row < 6; ++row) {
        const double v = std::abs(A[row][col]);
        if (v > max_val) {
          max_val = v;
          pivot = row;
        }
      }
      if (pivot != col) {
        for (int k = 0; k < 6; ++k)
          std::swap(A[col][k], A[pivot][k]);
        std::swap(b[col], b[pivot]);
      }
      if (std::abs(A[col][col]) < 1e-14)
        continue;
      for (int row = col + 1; row < 6; ++row) {
        const double f = A[row][col] / A[col][col];
        for (int k = col; k < 6; ++k)
          A[row][k] -= f * A[col][k];
        b[row] -= f * b[col];
      }
    }
    for (int row = 5; row >= 0; --row) {
      double s = b[row];
      for (int k = row + 1; k < 6; ++k)
        s -= A[row][k] * x[k];
      const double d = A[row][row];
      x[row] = (std::abs(d) < 1e-14) ? 0.0 : s / d;
    }
  }

  /** @brief Solve @f$ a t^2 + b t + c = 0 @f$ for real @f$ t @f$. */
  static void solve_quadratic_real(double a, double b, double c,
                                   std::vector<double> &out, double tol) {
    if (std::abs(a) < tol) {
      if (std::abs(b) < tol)
        return;
      out.push_back(-c / b);
      return;
    }
    const double disc = b * b - 4.0 * a * c;
    if (disc < -tol)
      return;
    if (disc < tol) {
      out.push_back(-b / (2.0 * a));
      return;
    }
    const double sq = std::sqrt(disc);
    out.push_back((-b + sq) / (2.0 * a));
    out.push_back((-b - sq) / (2.0 * a));
  }

  /** @brief Solve the quartic @f$ p_0 t^4 + p_1 t^3 + p_2 t^2 + p_3 t + p_4
   *         = 0 @f$ for real @f$ t @f$. */
  static void solve_quartic_real(const double p[5], std::vector<double> &out,
                                 double tol) {
    if (std::abs(p[0]) < tol) {
      std::vector<double> sub;
      solve_cubic_real(p[1], p[2], p[3], p[4], sub, tol);
      out.insert(out.end(), sub.begin(), sub.end());
      return;
    }
    double a = p[1] / p[0];
    double b = p[2] / p[0];
    double c = p[3] / p[0];
    double d = p[4] / p[0];

    // depressed: u^4 + P u^2 + Q u + R = 0 where t = u - a/4
    const double P = b - 3.0 * a * a / 8.0;
    const double Q = c - a * b / 2.0 + a * a * a / 8.0;
    const double R =
        d - a * c / 4.0 + a * a * b / 16.0 - 3.0 * a * a * a * a / 256.0;

    // Resolvent cubic: m^3 - P m^2 / 2 - R m - (4 P R - Q^2) / 8 = 0
    const double c0 = 1.0;
    const double c1 = -P / 2.0;
    const double c2 = -R;
    const double c3 = -(4.0 * P * R - Q * Q) / 8.0;
    std::vector<double> mroots;
    solve_cubic_real(c0, c1, c2, c3, mroots, tol);
    if (mroots.empty())
      return;
    double m = mroots.front();
    if (m < 0)
      m = 0;

    const double s1 =
        (std::abs(2.0 * m - P) < tol) ? 0.0 : std::sqrt(2.0 * m - P);
    const double s2 =
        (std::abs(s1) < tol) ? std::sqrt(std::abs(P)) : Q / (4.0 * s1);

    std::vector<double> uroots;
    {
      std::vector<double> r1;
      solve_quadratic_real(1.0, s1, m + s2, r1, tol);
      uroots.insert(uroots.end(), r1.begin(), r1.end());
    }
    {
      std::vector<double> r2;
      solve_quadratic_real(1.0, -s1, m - s2, r2, tol);
      uroots.insert(uroots.end(), r2.begin(), r2.end());
    }

    for (double u : uroots) {
      double t = u - a / 4.0;
      // Newton polish on the original polynomial.
      const double fp0 = p[0];
      const double fp1 = p[1];
      const double fp2 = p[2];
      const double fp3 = p[3];
      const double fp4 = p[4];
      for (int it = 0; it < 8; ++it) {
        const double val = (((fp0 * t + fp1) * t + fp2) * t + fp3) * t + fp4;
        const double der =
            (4.0 * fp0 * t + 3.0 * fp1) * t * t + 2.0 * fp2 * t + fp3;
        if (std::abs(der) < tol)
          break;
        t -= val / der;
      }
      const double val = (((fp0 * t + fp1) * t + fp2) * t + fp3) * t + fp4;
      if (std::abs(val) < 1e-4)
        out.push_back(t);
    }

    std::sort(out.begin(), out.end());
    std::vector<double> clean;
    for (double v : out) {
      if (clean.empty() || std::abs(v - clean.back()) > 1e-6)
        clean.push_back(v);
    }
    out.swap(clean);
  }

  /** @brief Solve @f$ a t^3 + b t^2 + c t + d = 0 @f$ for real @f$ t @f$. */
  static void solve_cubic_real(double a, double b, double c, double d,
                               std::vector<double> &out, double tol) {
    if (std::abs(a) < tol) {
      solve_quadratic_real(b, c, d, out, tol);
      return;
    }
    const double p = b / a;
    const double q = c / a;
    const double r = d / a;
    const double P = q - p * p / 3.0;
    const double Q = r - p * q / 3.0 + 2.0 * p * p * p / 27.0;
    const double disc = -4.0 * P * P * P - 27.0 * Q * Q;
    std::vector<double> roots;
    if (disc > tol) {
      // Three distinct real roots (trigonometric case).
      const double m = 2.0 * std::sqrt(-P / 3.0);
      const double theta = std::acos(3.0 * Q / (P * m)) / 3.0;
      roots.push_back(m * std::cos(theta) - p / 3.0);
      roots.push_back(m * std::cos(theta - 2.0 * M_PI / 3.0) - p / 3.0);
      roots.push_back(m * std::cos(theta - 4.0 * M_PI / 3.0) - p / 3.0);
    } else if (disc < -tol) {
      // One real root.  We use Cardano's formula, which yields the
      // unique real root in the standard depressed-cubic form.
      const double D = Q * Q / 4.0 + P * P * P / 27.0;
      const double sqD = std::sqrt(std::max(0.0, D));
      const double u = std::cbrt(-Q / 2.0 + sqD);
      const double v = std::cbrt(-Q / 2.0 - sqD);
      roots.push_back(u + v - p / 3.0);
    } else {
      // Repeated roots.
      if (std::abs(Q) < tol) {
        roots.push_back(-p / 3.0);
      } else {
        roots.push_back(3.0 * Q / P - p / 3.0);
        roots.push_back(-3.0 * Q / (2.0 * P) - p / 3.0);
      }
    }
    for (auto &t : roots) {
      for (int it = 0; it < 8; ++it) {
        const double val = a * t * t * t + b * t * t + c * t + d;
        const double der = 3.0 * a * t * t + 2.0 * b * t + c;
        if (std::abs(der) < tol)
          break;
        t -= val / der;
      }
    }
    std::sort(roots.begin(), roots.end());
    for (double v : roots) {
      if (out.empty() || std::abs(v - out.back()) > 1e-6)
        out.push_back(v);
    }
  }
};

/**
 * @brief Convenience overload intersecting two `conic_variant<T>`
 *        objects.
 *
 * Dispatches on the concrete stored types via the variant visitor
 * machinery (see `conic_factory.hpp`), then delegates to
 * `conic_intersect::compute`.
 */
template <typename T>
conic_intersect<T> intersect(const conic_variant<T> &f_in,
                             const conic_variant<T> &g_in, double tol = 1e-12) {
  conic_variant<T> &f = const_cast<conic_variant<T> &>(f_in);
  conic_variant<T> &g = const_cast<conic_variant<T> &>(g_in);
  return conic_intersect<T>::dispatch_variant(f, g, tol);
}

} // namespace planeGeometry

#endif
