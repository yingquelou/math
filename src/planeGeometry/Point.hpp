#ifndef PLANE_GEOMETRY_POINT_HPP
#define PLANE_GEOMETRY_POINT_HPP

#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "tensor/tensor.hpp"

namespace planeGeometry {

/**
 * @brief 2D 平面中的点/向量，模板类
 *
 * Point<T> 以 tensor::Tensor<T, 1> 为存储后端，提供 2 维点/向量的
 * 构造、线性运算、几何运算（模、夹角、投影、基底坐标、平行判定等）。
 *
 * @tparam T 数值类型，须满足 tensor::detail::is_numeric 约束（整数或浮点）
 *
 * @note 默认构造产生零向量 (0, 0)。类同时可作为"点"与"向量"使用，
 *       并提供 using Vector = Point<double>; 作为常见别名。
 */
template <typename T>
class Point {
 public:
  /** @brief 底层存储类型别名（tensor::Tensor<T, 1>） */
  using tensor_type = tensor::Tensor<T, 1>;

 private:
  tensor_type vec_;

 public:
  /**
   * @brief 默认构造，零向量 (0, 0)
   */
  Point() : vec_(2, T()) {}

  /**
   * @brief 按 (x, y) 坐标构造
   * @param x x 坐标
   * @param y y 坐标
   */
  Point(T x, T y) : vec_({x, y}) {}

  /**
   * @brief 从 tensor::Tensor<T, 1> 构造
   * @param t 长度必须为 2，否则抛出 std::invalid_argument
   * @throws std::invalid_argument 当 t.size() != 2
   */
  explicit Point(const tensor_type &t) : vec_(t) {
    if (vec_.size() != 2) {
      throw std::invalid_argument(
          "Point(const tensor_type&): tensor must have size 2");
    }
  }

  /**
   * @brief 从初始化列表构造
   * @param list 必须包含恰好 2 个元素
   * @throws std::invalid_argument 当 list.size() != 2
   */
  Point(std::initializer_list<T> list) : vec_(list) {
    if (list.size() != 2) {
      throw std::invalid_argument(
          "Point: initializer_list must have exactly 2 elements");
    }
  }

  /** @brief 拷贝构造 */
  Point(const Point &) = default;
  /** @brief 移动构造 */
  Point(Point &&) noexcept = default;
  /** @brief 拷贝赋值 */
  Point &operator=(const Point &) = default;
  /** @brief 移动赋值 */
  Point &operator=(Point &&) noexcept = default;
  /** @brief 析构 */
  ~Point() = default;

  /** @brief 获取 x 坐标（只读） */
  const T &x() const { return vec_[0]; }
  /** @brief 获取 y 坐标（只读） */
  const T &y() const { return vec_[1]; }
  /** @brief 获取 x 坐标（可写） */
  T &x() { return vec_[0]; }
  /** @brief 获取 y 坐标（可写） */
  T &y() { return vec_[1]; }

  /** @brief 获取底层 Tensor 的只读引用 */
  const tensor_type &as_tensor() const { return vec_; }
  /** @brief 获取底层 Tensor 的可写引用 */
  tensor_type &as_tensor() { return vec_; }

  /** @brief 向量加法 */
  Point operator+(const Point &other) const {
    return Point(vec_[0] + other.vec_[0], vec_[1] + other.vec_[1]);
  }

  /** @brief 向量减法 */
  Point operator-(const Point &other) const {
    return Point(vec_[0] - other.vec_[0], vec_[1] - other.vec_[1]);
  }

  /** @brief 数乘（右乘） */
  Point operator*(T s) const { return Point(vec_[0] * s, vec_[1] * s); }

  /** @brief 取反 */
  Point operator-() const { return Point(-vec_[0], -vec_[1]); }

  /** @brief 复合加法 */
  Point &operator+=(const Point &other) {
    vec_[0] += other.vec_[0];
    vec_[1] += other.vec_[1];
    return *this;
  }

  /** @brief 复合减法 */
  Point &operator-=(const Point &other) {
    vec_[0] -= other.vec_[0];
    vec_[1] -= other.vec_[1];
    return *this;
  }

  /** @brief 复合数乘 */
  Point &operator*=(T s) {
    vec_[0] *= s;
    vec_[1] *= s;
    return *this;
  }

  /** @brief 数乘（左乘） */
  friend Point operator*(T s, const Point &p) {
    return Point(p.vec_[0] * s, p.vec_[1] * s);
  }

  /**
   * @brief 点积（内积）
   * @param other 另一向量
   * @return (*this) · other
   */
  T dot(const Point &other) const { return vec_.dot(other.vec_); }

  /**
   * @brief 2D 叉积的 z 分量（行列式 x1*y2 - x2*y1）
   * @param other 另一向量
   * @return 有符号面积，正值表示逆时针方向
   */
  T cross_z(const Point &other) const {
    return vec_[0] * other.vec_[1] - vec_[1] * other.vec_[0];
  }

  /**
   * @brief 向量的欧几里得模
   * @return sqrt(x² + y²)
   */
  T norm() const { return vec_.norm(); }

  /**
   * @brief 与另一向量的夹角 [0, π]
   * @param other 另一向量
   * @return 弧度制夹角；若任一向量近似为零向量则返回 0
   */
  double angle_to(const Point &other) const {
    const double n1 = norm();
    const double n2 = other.norm();
    if (n1 < 1e-15 || n2 < 1e-15) {
      return 0.0;
    }
    const double cosine =
        static_cast<double>(dot(other)) / (n1 * n2);
    const double clamped = cosine > 1.0 ? 1.0 : (cosine < -1.0 ? -1.0 : cosine);
    return std::acos(clamped);
  }

  /**
   * @brief 计算 (*this) 在 target 上的正交投影向量
   * @param target 投影目标方向向量
   * @return 投影向量（方向同 target）
   * @throws std::invalid_argument 当 target 近似为零向量
   */
  Point projection_on(const Point &target) const {
    const double n2 = target.norm();
    if (n2 < 1e-15) {
      throw std::invalid_argument(
          "Point::projection_on: target is a zero vector");
    }
    const double scale =
        static_cast<double>(dot(target)) / (n2 * n2);
    return Point(static_cast<T>(target.vec_[0] * scale),
                 static_cast<T>(target.vec_[1] * scale));
  }

  /**
   * @brief 计算 (*this) 在基底 (ex, ey) 下的坐标
   * @param ex 基底 x 方向向量
   * @param ey 基底 y 方向向量
   * @return 坐标 (l1, l2) 使得 l1*ex + l2*ey ≈ *this
   * @throws std::invalid_argument 当基底线性相关
   */
  Point coords_in(const Point &ex, const Point &ey) const {
    const double det =
        static_cast<double>(ex.vec_[0]) * static_cast<double>(ey.vec_[1]) -
        static_cast<double>(ex.vec_[1]) * static_cast<double>(ey.vec_[0]);
    if (std::abs(det) < 1e-15) {
      throw std::invalid_argument(
          "Point::coords_in: basis vectors are linearly dependent (zero "
          "basis)");
    }
    const double l1 =
        (static_cast<double>(vec_[0]) * static_cast<double>(ey.vec_[1]) -
         static_cast<double>(vec_[1]) * static_cast<double>(ey.vec_[0])) /
        det;
    const double l2 =
        (static_cast<double>(ex.vec_[0]) * static_cast<double>(vec_[1]) -
         static_cast<double>(ex.vec_[1]) * static_cast<double>(vec_[0])) /
        det;
    return Point(static_cast<T>(l1), static_cast<T>(l2));
  }

  /**
   * @brief 判定两向量是否平行
   * @param other 另一向量
   * @return true 当 |cross_z| < 1e-12
   */
  bool is_parallel_to(const Point &other) const {
    return std::abs(cross_z(other)) < 1e-12;
  }

  Point rotated_by(T angle) const {
    const double c = std::cos(static_cast<double>(angle));
    const double s = std::sin(static_cast<double>(angle));
    return Point(static_cast<T>(vec_[0] * c - vec_[1] * s),
                 static_cast<T>(vec_[0] * s + vec_[1] * c));
  }

  Point rotated_by(T angle, const Point &center) const {
    const Point rel(vec_[0] - center.vec_[0], vec_[1] - center.vec_[1]);
    const Point rot = rel.rotated_by(angle);
    return Point(rot.vec_[0] + center.vec_[0], rot.vec_[1] + center.vec_[1]);
  }

  static std::pair<Point, Point> gram_schmidt(const Point &u, const Point &v) {
    const double n_u = u.norm();
    if (n_u < 1e-15) {
      throw std::invalid_argument("Point::gram_schmidt: u is zero");
    }
    Point e1(static_cast<T>(u.vec_[0] / n_u), static_cast<T>(u.vec_[1] / n_u));
    const double v_along = v.dot(e1);
    Point proj(static_cast<T>(e1.vec_[0] * v_along), static_cast<T>(e1.vec_[1] * v_along));
    Point v_perp(v.vec_[0] - proj.vec_[0], v.vec_[1] - proj.vec_[1]);
    const double n_perp = v_perp.norm();
    if (n_perp < 1e-15) {
      throw std::invalid_argument("Point::gram_schmidt: v linearly dependent on u");
    }
    Point e2(static_cast<T>(v_perp.vec_[0] / n_perp),
             static_cast<T>(v_perp.vec_[1] / n_perp));
    return std::make_pair(e1, e2);
  }

  /** @brief 分量相等比较 */
  bool operator==(const Point &other) const {
    return vec_[0] == other.vec_[0] && vec_[1] == other.vec_[1];
  }

  /** @brief 分量不等比较 */
  bool operator!=(const Point &other) const { return !(*this == other); }

  /**
   * @brief 流输出，格式 "(x,y)"
   * @param os 输出流
   * @param p  待输出点
   * @return 输出流引用
   */
  friend std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.vec_[0] << "," << p.vec_[1] << ")";
    return os;
  }
};

/** @brief 常用别名：Point<double> */
using Vector = Point<double>;

}  // namespace planeGeometry

#endif
