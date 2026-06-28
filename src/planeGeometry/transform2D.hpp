#ifndef PLANE_GEOMETRY_TRANSFORM2D_HPP
#define PLANE_GEOMETRY_TRANSFORM2D_HPP

#include <cmath>
#include <utility>

#include "Point.hpp"
#include "tensor/tensor.hpp"

namespace planeGeometry {

/**
 * @brief Compute the 2x2 rotation matrix
 *        @f$ R(\theta) = \begin{bmatrix} \cos\theta & -\sin\theta \\
 *        \sin\theta & \cos\theta \end{bmatrix} @f$.
 *
 * @tparam T Numeric type.
 * @param theta Rotation angle in radians.
 * @return A `tensor::Tensor<T, 2>` containing the 2x2 rotation matrix.
 */
template <typename T>
tensor::Tensor<T, 2> rotation_matrix(T theta) {
  const T c = std::cos(theta);
  const T s = std::sin(theta);
  tensor::Tensor<T, 2> m(2, 2);
  m(0, 0) = c;  m(0, 1) = -s;
  m(1, 0) = s;  m(1, 1) = c;
  return m;
}

/**
 * @brief Rotate a point about the origin.
 *
 * Computes @f$ R(\theta)\,p = (c x - s y,\; s x + c y) @f$.
 *
 * @tparam T Numeric type.
 * @param p     The point @f$ (x, y) @f$.
 * @param theta Rotation angle in radians.
 * @return The rotated point.
 */
template <typename T>
Point<T> rotate_about_origin(const Point<T>& p, T theta) {
  tensor::Tensor<T, 2> R = rotation_matrix(theta);
  tensor::Tensor<T, 1> v(2);
  v[0] = p.x();
  v[1] = p.y();
  auto r = R * v;
  return Point<T>(r[0], r[1]);
}

/**
 * @brief Rotate a point about an arbitrary center.
 *
 * Translates so that the center is the origin, rotates, and translates
 * back: @f$ \text{center} + R(\theta)\,(p - \text{center}) @f$.
 *
 * @tparam T Numeric type.
 * @param p      Point to rotate.
 * @param center Center of rotation.
 * @param theta  Rotation angle in radians.
 * @return The rotated point.
 */
template <typename T>
Point<T> rotate_about(const Point<T>& p, const Point<T>& center, T theta) {
  tensor::Tensor<T, 2> R = rotation_matrix(theta);
  tensor::Tensor<T, 1> v(2);
  v[0] = p.x() - center.x();
  v[1] = p.y() - center.y();
  auto r = R * v;
  return Point<T>(center.x() + r[0], center.y() + r[1]);
}

/**
 * @brief Translate a point by @f$ (dx, dy) @f$.
 *
 * @tparam T Numeric type.
 * @param p  Original point.
 * @param dx Translation in @f$ x @f$.
 * @param dy Translation in @f$ y @f$.
 * @return The translated point @f$ (x + dx,\; y + dy) @f$.
 */
template <typename T>
Point<T> translate(const Point<T>& p, T dx, T dy) {
  return Point<T>(p.x() + dx, p.y() + dy);
}

/**
 * @brief Apply a rigid transform (rotation about the origin + translation).
 *
 * Computes @f$ (c x - s y + dx,\; s x + c y + dy) @f$.
 *
 * @tparam T Numeric type.
 * @param p     Point to transform.
 * @param theta Rotation angle in radians.
 * @param dx    Translation in @f$ x @f$.
 * @param dy    Translation in @f$ y @f$.
 * @return The transformed point.
 */
template <typename T>
Point<T> rigid_transform(const Point<T>& p, T theta, T dx, T dy) {
  const T c = std::cos(theta);
  const T s = std::sin(theta);
  return Point<T>(c * p.x() - s * p.y() + dx,
                  s * p.x() + c * p.y() + dy);
}

/**
 * @brief Scale a point about the origin.
 *
 * Computes @f$ (s_x x,\; s_y y) @f$.
 *
 * @tparam T Numeric type.
 * @param p  Point to scale.
 * @param sx Scale factor for @f$ x @f$.
 * @param sy Scale factor for @f$ y @f$.
 * @return The scaled point.
 */
template <typename T>
Point<T> scale(const Point<T>& p, T sx, T sy) {
  return Point<T>(sx * p.x(), sy * p.y());
}

}  // namespace planeGeometry

#endif
