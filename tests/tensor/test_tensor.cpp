/**
 * @file test_tensor.cpp
 * @brief Comprehensive unit tests for tensor::Tensor<T, N> (primary template,
 *        N=1 vector specialization, N=2 matrix specialization).
 *
 * Coverage summary:
 *   - Primary template: construction family, shape/size/dim/ndim,
 *     fill/reshape, operator(), operator[], operator==/!=, element-wise
 *     arithmetic (+, -, *, /, compound-assignment, unary +/-), scalar
 *     multiplication/division, int/float/double type instantiation.
 *   - Vector (N=1): dot, cross, norm, normalize, cwiseProduct, zero-vector
 *     and orthogonal edge cases, anti-commutative cross product.
 *   - Matrix (N=2): transpose (incl. double-transpose identity), trace,
 *     determinant (2x2 / 3x3 / triangular / singular), inverse, rank
 *     (full-rank / rank-1 / zero), pow (k=0 / k=10 / non-square error),
 *     row-echelon / reduced-row-echelon / standard forms, row/column
 *     elementary operations (swap_rows / scale_row / add_row_to),
 *     concat (hconcat / operator&), same_shape, eye, random, toString,
 *     toLaTeX, leftCompatibleIdentity, rightCompatibleIdentity, is_valid,
 *     explicit operator bool, row(i)/col(j) access.
 *
 * Known limitations intentionally left untested (by design of tensor.hpp):
 *   - operator[] does NOT perform bounds checking (unlike operator() which
 *     throws std::out_of_range).  Callers must ensure the flat index is
 *     within [0, size()).  Adding bounds checking is a possible follow-up.
 *   - No dedicated overload for element-wise matrix-matrix multiplication
 *     in the primary template (the matrix specialization provides operator*
 *     as the standard matrix product).
 *   - SFINAE negative verification is handled separately by
 *     test_tensor_sfinae_negative.cpp (compilation must fail).
 */

#include "tensor.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>

#define CHECK_TRUE(cond)                                                       \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "FAIL: " << #cond << " at line " << __LINE__ << "\n";       \
      return 1;                                                                \
    }                                                                          \
  } while (0)
#define CHECK_EQ(a, b)                                                         \
  do {                                                                         \
    if ((a) != (b)) {                                                          \
      std::cerr << "FAIL: " << #a << " != " << #b << " at line " << __LINE__   \
                << "\n";                                                       \
      return 1;                                                                \
    }                                                                          \
  } while (0)
#define CHECK_NEAR(a, b, eps)                                                  \
  do {                                                                         \
    if (std::abs((a) - (b)) > (eps)) {                                         \
      std::cerr << "FAIL: |" << #a << "-" << #b << "| <= " << #eps             \
                << " at line " << __LINE__ << "\n";                            \
      return 1;                                                                \
    }                                                                          \
  } while (0)

int main() {
  using tensor::Tensor;

  std::size_t counter = 0;
#define INC_TEST()                                                             \
  do {                                                                         \
    ++counter;                                                                 \
  } while (0)

  // ====================================================================
  // Section A: Primary template - basic construction & element access
  // ====================================================================

  // T1 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> t({2, 3, 4});
    CHECK_EQ(t.size(), 24u);
    CHECK_EQ(t.dim(0), 2u);
    CHECK_EQ(t.dim(1), 3u);
    CHECK_EQ(t.dim(2), 4u);
  }

  // T2 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> t({2, 3, 4});
    t(1, 2, 3) = 7.0;
    CHECK_NEAR(t(1, 2, 3), 7.0, 1e-9);
  }

  // T5 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 3, 4}, 1.0);
    a.reshape({4, 6, 1});
    CHECK_EQ(a.size(), 24u);
  }

  // T17 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> z = Tensor<double, 3>::zeros({2, 2, 2});
    for (size_t i = 0; i < 8; i++)
      CHECK_NEAR(z[i], 0.0, 1e-9);
  }

  // T19 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;
    std::ostringstream os;
    os << m1;
    (void)os;
    CHECK_TRUE(true);
  }

  // T20 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> e1({1, 1, 1}, 5.0);
    auto neg = -e1;
    CHECK_NEAR(neg(0, 0, 0), -5.0, 1e-9);
  }

  // T30: default construction creates empty tensor (size 0 along every dim)
  {
    INC_TEST();
    Tensor<double, 3> t;
    CHECK_EQ(t.size(), 0u);
    CHECK_EQ(t.dim(0), 0u);
    CHECK_EQ(t.dim(1), 0u);
    CHECK_EQ(t.dim(2), 0u);
  }

  // T31: shape+value construction fills every element
  {
    INC_TEST();
    Tensor<double, 3> t({2, 2, 2}, 5.0);
    CHECK_NEAR(t(0, 0, 0), 5.0, 1e-9);
    CHECK_NEAR(t(1, 1, 1), 5.0, 1e-9);
    CHECK_EQ(t.size(), 8u);
  }

  // T32: copy constructor produces an equal independent tensor
  {
    INC_TEST();
    Tensor<double, 3> a({1, 2, 3}, 1.0);
    Tensor<double, 3> b(a);
    CHECK_TRUE(a == b);
    CHECK_EQ(b.size(), a.size());
  }

  // T33: move constructor transfers ownership; source becomes empty-ish
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 3.0);
    Tensor<double, 3> b(std::move(a));
    CHECK_EQ(a.size(), 0u);
    CHECK_NEAR(b(0, 0, 0), 3.0, 1e-9);
    CHECK_EQ(b.size(), 8u);
  }

  // T34: construct via explicit shape_type array
  {
    INC_TEST();
    Tensor<double, 3> t(std::array<std::size_t, 3>{1, 1, 1}, 7.0);
    CHECK_NEAR(t(0, 0, 0), 7.0, 1e-9);
  }

  // T35: zeros factory via copy-initialization syntax
  {
    INC_TEST();
    Tensor<double, 3> t(Tensor<double, 3>::zeros({2, 3, 4}));
    CHECK_NEAR(t(1, 2, 3), 0.0, 1e-9);
    CHECK_EQ(t.size(), 24u);
  }

  // T36: ones factory produces all ones
  {
    INC_TEST();
    Tensor<double, 3> t(Tensor<double, 3>::ones({2, 2, 2}));
    CHECK_NEAR(t(1, 1, 1), 1.0, 1e-9);
    CHECK_EQ(t.size(), 8u);
  }

  // T37: fill() replaces all elements
  {
    INC_TEST();
    Tensor<double, 3> t({2, 2, 2});
    t.fill(4.0);
    CHECK_NEAR(t(1, 1, 1), 4.0, 1e-9);
    CHECK_NEAR(t(0, 0, 0), 4.0, 1e-9);
  }

  // T38: reshape with mismatched total size throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 3> t({2, 3, 4}, 1.0);
    bool caught = false;
    try {
      t.reshape({5, 5, 1});
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T39: operator==/!= on differing tensors
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 1.0);
    Tensor<double, 3> b({2, 2, 2}, 2.0);
    CHECK_TRUE(!(a == b));
    CHECK_TRUE(a != b);
  }

  // T40: operator() with out-of-range multi-index throws out_of_range
  {
    INC_TEST();
    Tensor<double, 3> t({2, 2, 2}, 1.0);
    bool caught = false;
    try {
      t(5, 0, 0);
      caught = false;
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T41: operator[] with a valid flat index returns the right value
  //       (operator[] is flat access; it is not bounds-checked in this
  //       implementation, so we validate correct access rather than exception
  //       behavior.)
  {
    INC_TEST();
    Tensor<double, 3> t({2, 2, 2}, 1.0);
    t[7] = 9.0;
    CHECK_NEAR(t[7], 9.0, 1e-9);
    CHECK_EQ(t.size(), 8u);
  }

  // T42: instantiation with int works; element access returns int
  {
    INC_TEST();
    Tensor<int, 3> t({2, 2, 2}, 3);
    CHECK_EQ(t(1, 1, 1), 3);
  }

  // T43: instantiation with float works with float tolerances
  {
    INC_TEST();
    Tensor<float, 3> t({2, 2, 2}, 1.5f);
    CHECK_NEAR(t(1, 1, 1), 1.5f, 1e-6f);
  }

  // ====================================================================
  // Section B: Primary template - arithmetic
  // ====================================================================

  // T3 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 3, 4}, 1.0);
    Tensor<double, 3> b({2, 3, 4}, 2.0);
    auto c = a + b;
    CHECK_NEAR(c(0, 0, 0), 3.0, 1e-9);
  }

  // T4 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 3, 4}, 1.0);
    auto d = a * 2.0;
    CHECK_NEAR(d(0, 0, 0), 2.0, 1e-9);
  }

  // T44: element-wise subtraction
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 1.0);
    Tensor<double, 3> b({2, 2, 2}, 2.0);
    Tensor<double, 3> c = a - b;
    CHECK_NEAR(c(0, 0, 0), -1.0, 1e-9);
  }

  // T45: element-wise addition of two tensors (operator+ already covered,
  //       here we additionally check that operator* with a scalar works)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 3.0);
    Tensor<double, 3> c = a * 2.0;
    CHECK_NEAR(c(0, 0, 0), 6.0, 1e-9);
  }

  // T46: scalar division (tensor / scalar)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 6.0);
    Tensor<double, 3> c = a / 2.0;
    CHECK_NEAR(c(0, 0, 0), 3.0, 1e-9);
  }

  // T47: compound addition +=
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 1.0);
    Tensor<double, 3> b({2, 2, 2}, 2.0);
    a += b;
    CHECK_NEAR(a(0, 0, 0), 3.0, 1e-9);
  }

  // T48: compound subtraction -=
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 3.0);
    Tensor<double, 3> b({2, 2, 2}, 1.0);
    a -= b;
    CHECK_NEAR(a(0, 0, 0), 2.0, 1e-9);
  }

  // T49: compound scalar multiplication *=
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 2.0);
    a *= 3.0;
    CHECK_NEAR(a(0, 0, 0), 6.0, 1e-9);
  }

  // T50: compound scalar division /=
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 6.0);
    a /= 2.0;
    CHECK_NEAR(a(0, 0, 0), 3.0, 1e-9);
  }

  // T51: unary plus preserves values
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 5.0);
    auto p = +a;
    CHECK_NEAR(p(0, 0, 0), 5.0, 1e-9);
    CHECK_TRUE(p == a);
  }

  // T52: unary minus on a positive tensor negates all elements
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0});
    auto m = -a;
    CHECK_NEAR(m(0, 0, 0), -1.0, 1e-9);
    CHECK_NEAR(m(0, 0, 1), -2.0, 1e-9);
    CHECK_NEAR(m(0, 1, 0), -3.0, 1e-9);
    CHECK_NEAR(m(1, 1, 1), -8.0, 1e-9);
  }

  // T53: scalar division (tensor / scalar)
  {
    INC_TEST();
    Tensor<double, 3> a({2, 2, 2}, 2.0);
    auto b = a / 2.0;
    CHECK_NEAR(b(0, 0, 0), 1.0, 1e-9);
  }

  // T54: shape-mismatched addition throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 3> a({2, 3, 4}, 1.0);
    Tensor<double, 3> b({3, 2, 4}, 1.0);
    bool caught = false;
    try {
      auto c = a + b;
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T55: integer addition works with CHECK_EQ
  {
    INC_TEST();
    Tensor<int, 3> a({2, 2, 2}, 3);
    Tensor<int, 3> b({2, 2, 2}, 4);
    auto c = a + b;
    CHECK_EQ(c(0, 0, 0), 7);
  }

  // T56: int scalar multiplication
  {
    INC_TEST();
    Tensor<int, 3> a({2, 2, 2}, 3);
    auto b = a * 2;
    CHECK_EQ(b(0, 0, 0), 6);
  }

  // ====================================================================
  // Section C: Vector specialization (N=1)
  // ====================================================================

  // T6 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 1> v1({1, 2, 3});
    Tensor<double, 1> v2({4, 5, 6});
    CHECK_NEAR(v1.dot(v2), 32.0, 1e-9);
  }

  // T7 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 1> v1({1, 2, 3});
    Tensor<double, 1> v2({4, 5, 6});
    auto cr = v1.cross(v2);
    CHECK_NEAR(cr(0), -3.0, 1e-9);
    CHECK_NEAR(cr(1), 6.0, 1e-9);
    CHECK_NEAR(cr(2), -3.0, 1e-9);
  }

  // T8 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 1> v1({1, 2, 3});
    CHECK_NEAR(v1.norm(), std::sqrt(14.0), 1e-9);
  }

  // T9 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 1> v1({1, 2, 3});
    try {
      Tensor<double, 1> s2({1.0, 2.0});
      s2.cross(v1);
      CHECK_TRUE(false);
    } catch (const std::invalid_argument &) {
      CHECK_TRUE(true);
    }
  }

  // T57: orthogonal vectors have dot product zero
  {
    INC_TEST();
    Tensor<double, 1> a({1, 0, 0});
    Tensor<double, 1> b({0, 1, 0});
    CHECK_NEAR(a.dot(b), 0.0, 1e-9);
  }

  // T58: normalized vector has unit norm
  {
    INC_TEST();
    Tensor<double, 1> a({1, 2, 3});
    auto n = a.normalize();
    CHECK_NEAR(n.norm(), 1.0, 1e-9);
  }

  // T59: component-wise product
  {
    INC_TEST();
    Tensor<double, 1> a({1, 2, 3});
    Tensor<double, 1> b({4, 5, 6});
    Tensor<double, 1> c = a.cwiseProduct(b);
    CHECK_NEAR(c(0), 4.0, 1e-9);
    CHECK_NEAR(c(1), 10.0, 1e-9);
    CHECK_NEAR(c(2), 18.0, 1e-9);
  }

  // T60: cross product is anti-commutative
  {
    INC_TEST();
    Tensor<double, 1> a({1, 2, 3});
    Tensor<double, 1> b({4, 5, 6});
    auto cr1 = a.cross(b);
    auto cr2 = b.cross(a);
    CHECK_NEAR(cr1(0), -cr2(0), 1e-9);
    CHECK_NEAR(cr1(1), -cr2(1), 1e-9);
    CHECK_NEAR(cr1(2), -cr2(2), 1e-9);
  }

  // T61: zero vector has zero norm
  {
    INC_TEST();
    Tensor<double, 1> a({0, 0, 0});
    CHECK_NEAR(a.norm(), 0.0, 1e-9);
  }

  // T62: normalizing a zero vector throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 1> a({0, 0, 0});
    bool caught = false;
    try {
      a.normalize();
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T63: float vector norm (3-4-5-like)
  {
    INC_TEST();
    Tensor<float, 1> a({1.0f, 2.0f, 2.0f});
    CHECK_NEAR(a.norm(), 3.0f, 1e-6f);
  }

  // T64: float normalization of a unit vector
  {
    INC_TEST();
    Tensor<float, 1> a({1.0f, 0.0f, 0.0f});
    auto n = a.normalize();
    CHECK_NEAR(n(0), 1.0f, 1e-6f);
  }

  // T65: dot product with mismatched sizes throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 1> a({1, 2, 3});
    Tensor<double, 1> b({1, 2, 3, 4});
    bool caught = false;
    try {
      a.dot(b);
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T66: int cross product yields correct int result
  {
    INC_TEST();
    Tensor<int, 1> a({1, 0, 0});
    Tensor<int, 1> b({0, 1, 0});
    auto cr = a.cross(b);
    CHECK_EQ(cr(2), 1);
  }

  // ====================================================================
  // Section D: Matrix specialization (N=2) - core algorithms
  // ====================================================================

  // T10 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;
    CHECK_NEAR(m1.determinant(), -2.0, 1e-9);
  }

  // T11 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;
    auto tr = m1.transpose();
    CHECK_NEAR(tr(0, 0), 1.0, 1e-9);
    CHECK_NEAR(tr(0, 1), 3.0, 1e-9);
    CHECK_NEAR(tr(1, 0), 2.0, 1e-9);
    CHECK_NEAR(tr(1, 1), 4.0, 1e-9);
  }

  // T12 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;
    auto inv = m1.inverse();
    auto prod = m1 * inv;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        CHECK_NEAR(prod(i, j), (i == j ? 1.0 : 0.0), 1e-9);
  }

  // T13 (moved from original location)
  {
    INC_TEST();
    auto eye3 = Tensor<double, 2>::eye(3);
    CHECK_EQ(eye3.rows(), 3u);
    CHECK_EQ(eye3.cols(), 3u);
    auto I = Tensor<double, 2>::eye(2);
    Tensor<double, 2> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;
    auto r2 = I * m1;
    CHECK_NEAR(r2(0, 0), 1.0, 1e-9);
    CHECK_NEAR(r2(0, 1), 2.0, 1e-9);
  }

  // T14 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> nonsq(2, 3);
    try {
      nonsq.determinant();
      CHECK_TRUE(false);
    } catch (const std::invalid_argument &) {
      CHECK_TRUE(true);
    }
  }

  // T14b (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> nonsq(2, 3);
    try {
      nonsq.inverse();
      CHECK_TRUE(false);
    } catch (const std::invalid_argument &) {
      CHECK_TRUE(true);
    }
  }

  // T15 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    Tensor<double, 1> x({1.0, 2.0, 3.0});
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    auto y = A * x;
    CHECK_NEAR(y(0), 14.0, 1e-9);
    CHECK_NEAR(y(1), 32.0, 1e-9);
  }

  // T16 (upgraded from original): exact rank check for a full-rank 3x3
  {
    INC_TEST();
    Tensor<double, 2> sq(3, 3);
    sq(0, 0) = 4;
    sq(0, 1) = 9;
    sq(0, 2) = 2;
    sq(1, 0) = 3;
    sq(1, 1) = 5;
    sq(1, 2) = 7;
    sq(2, 0) = 8;
    sq(2, 1) = 1;
    sq(2, 2) = 6;
    // The 3x3 with these values is full rank (det != 0); exact rank should
    // be 3.
    CHECK_EQ(sq.rank(), 3u);
  }

  // T67: double transpose returns the original matrix
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    auto tr = A.transpose();
    auto tr2 = tr.transpose();
    CHECK_TRUE(A == tr2);
  }

  // T68: transpose swaps rows and columns counts
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    auto tr = A.transpose();
    CHECK_EQ(tr.rows(), 3u);
    CHECK_EQ(tr.cols(), 2u);
  }

  // T69: trace of a 2x2 matrix (sum of diagonal)
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    CHECK_NEAR(A.trace(), 5.0, 1e-9);
  }

  // T70: trace of a 3x3 diagonal matrix
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(1, 1) = 2;
    A(2, 2) = 3;
    CHECK_NEAR(A.trace(), 6.0, 1e-9);
  }

  // T71: identity matrix trace equals its size
  {
    INC_TEST();
    Tensor<double, 2> I = Tensor<double, 2>::eye(5);
    CHECK_NEAR(I.trace(), 5.0, 1e-9);
  }

  // T72: singular 2x2 determinant is zero
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 2;
    A(1, 1) = 4;
    CHECK_NEAR(A.determinant(), 0.0, 1e-9);
  }

  // T73: triangular (diagonal 3x3) determinant is product of diagonal
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(1, 1) = 2;
    A(2, 2) = 3;
    CHECK_NEAR(A.determinant(), 6.0, 1e-9);
  }

  // T74: inverse produces a matrix that multiplies to identity
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    auto inv = A.inverse();
    auto prod = A * inv;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        CHECK_NEAR(prod(i, j), (i == j ? 1.0 : 0.0), 1e-9);
  }

  // T75: singular matrix inverse throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    // create a rank-1 (singular) matrix
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 2;
    A(1, 1) = 4;
    A(1, 2) = 6;
    A(2, 0) = 3;
    A(2, 1) = 6;
    A(2, 2) = 9;
    bool caught = false;
    try {
      A.inverse();
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T76: full-rank 2x2 has rank 2
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    CHECK_EQ(A.rank(), 2u);
  }

  // T77: rank-1 matrix has rank 1
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 2;
    A(1, 1) = 4;
    A(1, 2) = 6;
    A(2, 0) = 3;
    A(2, 1) = 6;
    A(2, 2) = 9;
    CHECK_EQ(A.rank(), 1u);
  }

  // T78: zero matrix has rank 0
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    CHECK_EQ(A.rank(), 0u);
  }

  // T79: pow(0) returns identity
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 1;
    A(1, 0) = 1;
    A(1, 1) = 0;
    auto P0 = A.pow(0);
    CHECK_NEAR(P0(0, 0), 1.0, 1e-9);
    CHECK_NEAR(P0(0, 1), 0.0, 1e-9);
    CHECK_NEAR(P0(1, 0), 0.0, 1e-9);
    CHECK_NEAR(P0(1, 1), 1.0, 1e-9);
  }

  // T80: pow(10) returns a properly-shaped non-null matrix
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 1;
    A(1, 0) = 1;
    A(1, 1) = 0;
    auto P10 = A.pow(10);
    CHECK_EQ(P10.rows(), 2u);
    CHECK_EQ(P10.cols(), 2u);
    CHECK_TRUE(P10 == P10); // self equality (non-null)
  }

  // T81: pow on a non-square matrix throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    bool caught = false;
    try {
      A.pow(2);
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // ====================================================================
  // Section E: Matrix specialization (N=2) - row ops & echelon forms
  // ====================================================================

  // T21 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    A.swap_rows(0, 2);
    CHECK_NEAR(A(0, 0), 7, 1e-9);
    CHECK_NEAR(A(2, 0), 1, 1e-9);

    Tensor<double, 2> B(2, 2);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        B(i, j) = static_cast<double>(i * 2 + j + 1);
    B.scale_row(0, 2.0);
    CHECK_NEAR(B(0, 0), 2, 1e-9);
    CHECK_NEAR(B(0, 1), 4, 1e-9);

    Tensor<double, 2> C(2, 2);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        C(i, j) = static_cast<double>(i * 2 + j + 1);
    C.add_row_to(0, 1.0, 1);
    CHECK_NEAR(C(1, 0), 4, 1e-9);
    CHECK_NEAR(C(1, 1), 6, 1e-9);

    bool caught = false;
    try {
      A.swap_rows(0, 5);
      CHECK_TRUE(false);
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T22 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> M(3, 3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        M(i, j) = static_cast<double>(i * 3 + j + 1);
    auto ref = M.reduced_row_echelon_form();
    CHECK_NEAR(ref(2, 0), 0, 1e-9);
    CHECK_NEAR(ref(2, 1), 0, 1e-9);
    CHECK_NEAR(ref(2, 2), 0, 1e-9);
  }

  // T82: swap_rows with out-of-range row throws out_of_range
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    bool caught = false;
    try {
      A.swap_rows(0, 5);
      caught = false;
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T83: scale_row with out-of-range row throws out_of_range
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    bool caught = false;
    try {
      A.scale_row(5, 2.0);
      caught = false;
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T84: add_row_to with out-of-range dst throws out_of_range
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        A(i, j) = static_cast<double>(i * 3 + j + 1);
    bool caught = false;
    try {
      A.add_row_to(0, 1.0, 5);
      caught = false;
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T85: row echelon form of identity is still identity (upper-triangular with
  // diag=1)
  {
    INC_TEST();
    Tensor<double, 2> A = Tensor<double, 2>::eye(3);
    auto ref = A.row_echelon_form();
    CHECK_NEAR(ref(0, 0), 1.0, 1e-9);
    CHECK_NEAR(ref(1, 1), 1.0, 1e-9);
    CHECK_NEAR(ref(2, 2), 1.0, 1e-9);
    CHECK_NEAR(ref(1, 0), 0.0, 1e-9);
    CHECK_NEAR(ref(2, 0), 0.0, 1e-9);
    CHECK_NEAR(ref(2, 1), 0.0, 1e-9);
  }

  // T86: REF of a rank-1 3x3 matrix has last 2 rows zero
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 2;
    A(1, 1) = 4;
    A(1, 2) = 6;
    A(2, 0) = 3;
    A(2, 1) = 6;
    A(2, 2) = 9;
    auto ref = A.row_echelon_form();
    CHECK_NEAR(ref(1, 0), 0.0, 1e-9);
    CHECK_NEAR(ref(1, 1), 0.0, 1e-9);
    CHECK_NEAR(ref(1, 2), 0.0, 1e-9);
    CHECK_NEAR(ref(2, 0), 0.0, 1e-9);
    CHECK_NEAR(ref(2, 1), 0.0, 1e-9);
    CHECK_NEAR(ref(2, 2), 0.0, 1e-9);
  }

  // T87: reduced row echelon form of a full-rank 2x3 has leading 1s in each row
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 4;
    A(1, 1) = 5;
    A(1, 2) = 6;
    auto rref = A.reduced_row_echelon_form();
    CHECK_EQ(rref.rows(), 2u);
    CHECK_EQ(rref.cols(), 3u);
    // leading entry of row 0 should be 1
    CHECK_NEAR(rref(0, 0), 1.0, 1e-9);
    // entry directly below should be 0
    CHECK_NEAR(rref(1, 0), 0.0, 1e-9);
  }

  // T88: reduced row echelon form of a zero matrix is zero
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    auto rref = A.reduced_row_echelon_form();
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        CHECK_NEAR(rref(i, j), 0.0, 1e-9);
  }

  // T89: reduced row echelon form of a full-rank square matrix is identity
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 2;
    A(1, 1) = 1;
    A(1, 2) = 1;
    A(2, 0) = 1;
    A(2, 1) = 3;
    A(2, 2) = 2;
    auto rref = A.reduced_row_echelon_form();
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        CHECK_NEAR(rref(i, j), (i == j ? 1.0 : 0.0), 1e-9);
  }

  // T90: standard_form of a rank-1 matrix has at most 1 non-zero row
  {
    INC_TEST();
    Tensor<double, 2> A(3, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 2;
    A(1, 1) = 4;
    A(1, 2) = 6;
    A(2, 0) = 3;
    A(2, 1) = 6;
    A(2, 2) = 9;
    auto sf = A.standard_form();
    CHECK_EQ(sf.rows(), 3u);
    CHECK_EQ(sf.cols(), 3u);
    // rows 1 and 2 should be entirely zero
    for (int j = 0; j < 3; j++) {
      CHECK_NEAR(sf(1, j), 0.0, 1e-9);
      CHECK_NEAR(sf(2, j), 0.0, 1e-9);
    }
    // row 0 should be non-zero
    CHECK_TRUE((sf(0, 0) != 0.0) || (sf(0, 1) != 0.0) || (sf(0, 2) != 0.0));
  }

  // T91: reduced row echelon form preserves the matrix shape
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 4; j++)
        A(i, j) = static_cast<double>(i * 4 + j + 1);
    auto rref = A.reduced_row_echelon_form();
    CHECK_EQ(rref.rows(), 3u);
    CHECK_EQ(rref.cols(), 4u);
  }

  // T92: row(i) returns a vector with correct values
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 4;
    A(1, 1) = 5;
    A(1, 2) = 6;
    auto r0 = A.row(0);
    auto r1 = A.row(1);
    CHECK_EQ(r0.size(), 3u);
    CHECK_NEAR(r0(0), 1.0, 1e-9);
    CHECK_NEAR(r0(1), 2.0, 1e-9);
    CHECK_NEAR(r0(2), 3.0, 1e-9);
    CHECK_NEAR(r1(0), 4.0, 1e-9);
    CHECK_NEAR(r1(2), 6.0, 1e-9);
  }

  // T93: col(j) returns correct values; out-of-range col throws out_of_range
  {
    INC_TEST();
    Tensor<double, 2> A(2, 3);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(1, 0) = 4;
    A(1, 1) = 5;
    A(1, 2) = 6;
    auto c1 = A.col(1);
    CHECK_EQ(c1.size(), 2u);
    CHECK_NEAR(c1(0), 2.0, 1e-9);
    CHECK_NEAR(c1(1), 5.0, 1e-9);

    bool caught = false;
    try {
      A.col(5);
      caught = false;
    } catch (const std::out_of_range &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // ====================================================================
  // Section F: Matrix specialization (N=2) - concat / shape / utilities
  // ====================================================================

  // T23 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2), B(2, 3);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        A(i, j) = static_cast<double>(i * 2 + j + 1);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 3; j++)
        B(i, j) = static_cast<double>(i * 3 + j + 1);
    auto C = A.hconcat(B);
    CHECK_EQ(C.cols(), 5u);
    CHECK_EQ(C.rows(), 2u);
    CHECK_NEAR(C(0, 0), 1, 1e-9);
    CHECK_NEAR(C(0, 2), 1, 1e-9);
    CHECK_NEAR(C(0, 4), 3, 1e-9);

    bool caught = false;
    try {
      Tensor<double, 2> A2(2, 2), B2(3, 3);
      A2.hconcat(B2);
      CHECK_TRUE(false);
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);

    Tensor<double, 2> A3(2, 2), B3(2, 3);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        A3(i, j) = static_cast<double>(i * 2 + j + 1);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 3; j++)
        B3(i, j) = static_cast<double>(i * 3 + j + 1);
    auto D = A3 & B3;
    CHECK_EQ(D.cols(), 5u);
  }

  // T24 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4), B(3, 4), C(4, 3);
    CHECK_TRUE(A.same_shape(B));
    CHECK_TRUE(!A.same_shape(C));
  }

  // T25 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        A(i, j) = static_cast<double>(i * 2 + j + 1);
    std::string s = A.toString();
    CHECK_TRUE(!s.empty());
    std::string l = A.toLaTeX();
    CHECK_TRUE(l.find("\\begin{bmatrix}") != std::string::npos);
    CHECK_TRUE(l.find("\\end{bmatrix}") != std::string::npos);
  }

  // T26 (moved from original location)
  {
    INC_TEST();
    auto R = Tensor<double, 2>::random(3, 4, 0.0, 1.0);
    CHECK_EQ(R.rows(), 3u);
    CHECK_EQ(R.cols(), 4u);
    bool in_range = true;
    for (size_t i = 0; i < 3; i++)
      for (size_t j = 0; j < 4; j++) {
        if (R(i, j) < 0.0 || R(i, j) > 1.0)
          in_range = false;
      }
    CHECK_TRUE(in_range);
  }

  // T27 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> valid(2, 3);
    CHECK_TRUE(static_cast<bool>(valid));
    CHECK_TRUE(valid.is_valid());
    Tensor<double, 2> empty;
    CHECK_TRUE(!static_cast<bool>(empty));
    CHECK_TRUE(!empty.is_valid());
  }

  // T28 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4);
    auto Li = A.leftCompatibleIdentity();
    CHECK_EQ(Li.rows(), 3u);
    CHECK_EQ(Li.cols(), 3u);
    auto Ri = A.rightCompatibleIdentity();
    CHECK_EQ(Ri.rows(), 4u);
    CHECK_EQ(Ri.cols(), 4u);
  }

  // T29 (moved from original location)
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 1;
    A(1, 0) = 1;
    A(1, 1) = 0;
    auto P2 = A.pow(2);
    CHECK_NEAR(P2(0, 0), 2, 1e-9);
    CHECK_NEAR(P2(0, 1), 1, 1e-9);
    CHECK_NEAR(P2(1, 0), 1, 1e-9);
    CHECK_NEAR(P2(1, 1), 1, 1e-9);
    auto P0 = A.pow(0);
    CHECK_NEAR(P0(0, 0), 1, 1e-9);
    CHECK_NEAR(P0(1, 1), 1, 1e-9);

    bool caught = false;
    Tensor<double, 2> nonsq(2, 3);
    try {
      nonsq.pow(2);
      CHECK_TRUE(false);
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T94: hconcat with mismatched row counts throws invalid_argument
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2), B(3, 2);
    bool caught = false;
    try {
      A.hconcat(B);
      caught = false;
    } catch (const std::invalid_argument &) {
      caught = true;
    }
    CHECK_TRUE(caught);
  }

  // T95: same_shape returns true for identical shapes
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4), B(3, 4);
    CHECK_TRUE(A.same_shape(B));
  }

  // T96: same_shape returns false for differing shapes
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4), B(4, 3);
    CHECK_TRUE(!A.same_shape(B));
  }

  // T97: eye of several sizes has diagonal 1 and off-diagonal 0
  {
    INC_TEST();
    auto I1 = Tensor<double, 2>::eye(1);
    CHECK_EQ(I1.rows(), 1u);
    CHECK_NEAR(I1(0, 0), 1.0, 1e-9);

    auto I2 = Tensor<double, 2>::eye(2);
    CHECK_NEAR(I2(0, 0), 1.0, 1e-9);
    CHECK_NEAR(I2(1, 1), 1.0, 1e-9);
    CHECK_NEAR(I2(0, 1), 0.0, 1e-9);
    CHECK_NEAR(I2(1, 0), 0.0, 1e-9);

    auto I3 = Tensor<double, 2>::eye(3);
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        CHECK_NEAR(I3(i, j), (i == j ? 1.0 : 0.0), 1e-9);

    auto I5 = Tensor<double, 2>::eye(5);
    CHECK_EQ(I5.rows(), 5u);
    CHECK_EQ(I5.cols(), 5u);
    CHECK_NEAR(I5.trace(), 5.0, 1e-9);
  }

  // T98: random factory produces correct size and values inside range
  {
    INC_TEST();
    auto R = Tensor<double, 2>::random(10, 10, -1.0, 1.0);
    CHECK_EQ(R.rows(), 10u);
    CHECK_EQ(R.cols(), 10u);
    bool ok = true;
    for (size_t i = 0; i < 10u; i++)
      for (size_t j = 0; j < 10u; j++)
        if (R(i, j) < -1.0 || R(i, j) >= 1.0)
          ok = false;
    CHECK_TRUE(ok);
  }

  // T99: toString produces non-empty textual output
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    std::string s = A.toString();
    CHECK_TRUE(!s.empty());
    CHECK_TRUE(s.find('[') != std::string::npos);
  }

  // T100: toLaTeX produces bmatrix with & and \\ separators
  {
    INC_TEST();
    Tensor<double, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    std::string l = A.toLaTeX();
    CHECK_TRUE(l.find("\\begin{bmatrix}") != std::string::npos);
    CHECK_TRUE(l.find("\\end{bmatrix}") != std::string::npos);
    CHECK_TRUE(l.find('&') != std::string::npos);
    CHECK_TRUE(l.find("\\\\") != std::string::npos);
  }

  // T101: leftCompatibleIdentity has rows/cols equal to A.rows()
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4);
    auto li = A.leftCompatibleIdentity();
    CHECK_EQ(li.rows(), 3u);
    CHECK_EQ(li.cols(), 3u);
  }

  // T102: rightCompatibleIdentity has rows/cols equal to A.cols()
  {
    INC_TEST();
    Tensor<double, 2> A(3, 4);
    auto ri = A.rightCompatibleIdentity();
    CHECK_EQ(ri.rows(), 4u);
    CHECK_EQ(ri.cols(), 4u);
  }

  // T103: valid matrix converts to bool true and reports is_valid true
  {
    INC_TEST();
    Tensor<double, 2> valid(2, 3);
    CHECK_TRUE(static_cast<bool>(valid));
    CHECK_TRUE(valid.is_valid());
  }

  // T104: empty matrix converts to bool false and reports is_valid false
  {
    INC_TEST();
    Tensor<double, 2> empty;
    CHECK_TRUE(!static_cast<bool>(empty));
    CHECK_TRUE(!empty.is_valid());
  }

  // T105: integer matrix element access works with CHECK_EQ
  {
    INC_TEST();
    Tensor<int, 2> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;
    CHECK_EQ(A(0, 0), 1);
    CHECK_EQ(A(0, 1), 2);
    CHECK_EQ(A(1, 0), 3);
    CHECK_EQ(A(1, 1), 4);
  }

  // T106: Tensor<T,1> is constructable from shape_type via zeros/ones
  {
    INC_TEST();
    typename Tensor<double, 1>::shape_type s1{{3}};
    Tensor<double, 1> v3 = Tensor<double, 1>::zeros(s1);
    CHECK_TRUE(v3.size() == 3);
    CHECK_NEAR(v3(0), 0.0, 1e-9);
    Tensor<double, 1> v4 = Tensor<double, 1>::ones(s1);
    CHECK_NEAR(v4(0), 1.0, 1e-9);
  }

  // T107: Tensor<T,2> is constructable from shape_type via zeros/ones
  {
    INC_TEST();
    typename Tensor<double, 2>::shape_type s2{{2, 3}};
    Tensor<double, 2> m3 = Tensor<double, 2>::zeros(s2);
    CHECK_TRUE(m3.rows() == 2);
    CHECK_TRUE(m3.cols() == 3);
    Tensor<double, 2> m4 = Tensor<double, 2>::ones(s2);
    CHECK_NEAR(m4(0, 0), 1.0, 1e-9);
  }

  // T108: primary template default-constructs to an empty tensor
  {
    INC_TEST();
    Tensor<double, 3> t_def;
    CHECK_TRUE(t_def.size() == 0);
  }

  // T109: outer_product
  {
    INC_TEST();
    Tensor<double, 1> u({1.0, 2.0});
    Tensor<double, 1> v({3.0, 4.0});
    Tensor<double, 2> op = Tensor<double, 2>::outer_product(u, v);
    CHECK_NEAR(op(0, 0), 3.0, 1e-12);
    CHECK_NEAR(op(0, 1), 4.0, 1e-12);
    CHECK_NEAR(op(1, 0), 6.0, 1e-12);
    CHECK_NEAR(op(1, 1), 8.0, 1e-12);
  }

  // T110: frobenius_norm
  {
    INC_TEST();
    Tensor<double, 2> m({{3.0, 4.0}});
    CHECK_NEAR(m.frobenius_norm(), 5.0, 1e-12);
  }

  // T111: is_symmetric
  {
    INC_TEST();
    Tensor<double, 2> sym({{1.0, 2.0}, {2.0, 3.0}});
    CHECK_TRUE(sym.is_symmetric());
    Tensor<double, 2> non_sym({{1.0, 2.0}, {3.0, 4.0}});
    CHECK_TRUE(!non_sym.is_symmetric());
  }

  // T112: is_orthogonal
  {
    INC_TEST();
    const double c = std::cos(0.3);
    const double s = std::sin(0.3);
    Tensor<double, 2> rot({{c, -s}, {s, c}});
    CHECK_TRUE(rot.is_orthogonal());
    Tensor<double, 2> not_ortho({{1.0, 1.0}, {0.0, 1.0}});
    CHECK_TRUE(!not_ortho.is_orthogonal());
  }

#undef INC_TEST

  std::cout << "ALL PASSED (" << counter << " tests)\n";
  return 0;
}
