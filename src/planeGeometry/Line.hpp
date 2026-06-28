#ifndef PLANE_GEOMETRY_LINE_HPP
#define PLANE_GEOMETRY_LINE_HPP

#include <cmath>
#include <ostream>
#include <stdexcept>

#include "Point.hpp"

namespace planeGeometry {

/**
 * @brief 平面中的直线（一般式 Ax + By + C = 0）
 *
 * Line<T> 以三个系数 (A, B, C) 表示平面直线的一般式。
 * 提供多种命名构造器（点斜式、法线式、两点式、点方向式、点法向式），
 * 以及直线间的平行判定、交点、方向/法向量、夹角、点到线/线到线距离等运算。
 *
 * @tparam T 数值类型，通常为 double
 *
 * @note 当 A 与 B 同时为零时，直线处于"不存在"状态（is_valid() 返回 false）。
 *       多数计算方法在此状态下将返回 false 或 0。
 */
template <typename T>
class Line {
 private:
  T A_;
  T B_;
  T C_;

 public:
  /**
   * @brief 默认构造，A=B=C=0（is_valid() 为 false）
   */
  Line() : A_(T()), B_(T()), C_(T()) {}

  /**
   * @brief 按一般式系数构造
   * @param A 系数 A
   * @param B 系数 B
   * @param C 系数 C
   */
  Line(T A, T B, T C) : A_(A), B_(B), C_(C) {}

  /** @brief 拷贝构造 */
  Line(const Line &) = default;
  /** @brief 移动构造 */
  Line(Line &&) noexcept = default;
  /** @brief 拷贝赋值 */
  Line &operator=(const Line &) = default;
  /** @brief 移动赋值 */
  Line &operator=(Line &&) noexcept = default;
  /** @brief 析构 */
  ~Line() = default;

  /** @brief 获取系数 A（只读） */
  const T &A() const { return A_; }
  /** @brief 获取系数 B（只读） */
  const T &B() const { return B_; }
  /** @brief 获取系数 C（只读） */
  const T &C() const { return C_; }

  /**
   * @brief 点斜式构造：通过点 @p p 斜率为 @p k
   * @param p 直线上一点
   * @param k 斜率（可为有限值）
   * @return 新 Line 实例
   */
  static Line from_point_slope(const Point<T> &p, T k) {
    return Line(k, T(-1), p.y() - k * p.x());
  }

  /**
   * @brief 法线式构造：x·cos(a) + y·sin(a) = s
   * @param a 法线与 x 轴的夹角（弧度）
   * @param s 原点到直线的有符号距离
   * @return 新 Line 实例
   */
  static Line from_normal_polar(T a, T s) {
    return Line(std::cos(a), std::sin(a), -s);
  }

  /**
   * @brief 两点式构造：通过 p1 与 p2
   * @param p1 第一点
   * @param p2 第二点
   * @return 新 Line；若 p1 == p2 则返回默认（invalid）状态
   */
  static Line from_two_points(const Point<T> &p1, const Point<T> &p2) {
    if (p1 == p2) {
      return Line();
    }
    const T dx = p2.x() - p1.x();
    const T dy = p2.y() - p1.y();
    if (std::abs(dx) < 1e-15 && std::abs(dy) < 1e-15) {
      return Line();
    }
    if (std::abs(dx) < 1e-15) {
      return Line(T(1), T(0), -p1.x());
    }
    return Line(dy, -dx, -(dy * p1.x() - dx * p1.y()));
  }

  /**
   * @brief 点方向式构造：通过点 @p p 沿方向 @p dir
   * @param p 直线上一点
   * @param dir 方向向量
   * @return 新 Line；若 dir 为零向量则返回 invalid 状态
   */
  static Line from_point_direction(const Point<T> &p, const Point<T> &dir) {
    if (std::abs(dir.x()) < 1e-15 && std::abs(dir.y()) < 1e-15) {
      return Line();
    }
    const T A = dir.y();
    const T B = -dir.x();
    const T C = -(A * p.x() + B * p.y());
    return Line(A, B, C);
  }

  /**
   * @brief 点法向式构造：通过点 @p p 且法向为 @p normal
   * @param p 直线上一点
   * @param normal 法向量
   * @return 新 Line；若 normal 为零向量则返回 invalid 状态
   */
  static Line from_point_normal(const Point<T> &p,
                                const Point<T> &normal) {
    if (std::abs(normal.x()) < 1e-15 && std::abs(normal.y()) < 1e-15) {
      return Line();
    }
    const T A = normal.x();
    const T B = normal.y();
    const T C = -(normal.x() * p.x() + normal.y() * p.y());
    return Line(A, B, C);
  }

  /**
   * @brief 判定直线是否有效（A、B 不全为零）
   * @return true 当直线合法
   */
  bool is_valid() const {
    return !(std::abs(A_) < 1e-15 && std::abs(B_) < 1e-15);
  }

  /**
   * @brief 判定两直线是否平行
   * @param other 另一直线
   * @param tol   行列式阈值
   * @return true 当 |A1*B2 - A2*B1| < tol
   */
  bool is_parallel_to(const Line &other, double tol = 1e-12) const {
    const double det =
        static_cast<double>(A_) * static_cast<double>(other.B_) -
        static_cast<double>(B_) * static_cast<double>(other.A_);
    return std::abs(det) < tol;
  }

  /**
   * @brief 给定 x 求直线上对应的 y
   * @param x   给定的 x 坐标
   * @param out 输出点（若成功则坐标为 (x, y)）
   * @return true 当 B 非零（即直线不垂直）时成功
   */
  bool point_at_x(T x, Point<T> &out) const {
    if (std::abs(B_) < 1e-15) {
      return false;
    }
    const T y = -(A_ * x + C_) / B_;
    out = Point<T>(x, y);
    return true;
  }

  /**
   * @brief 给定 y 求直线上对应的 x
   * @param y   给定的 y 坐标
   * @param out 输出点（若成功则坐标为 (x, y)）
   * @return true 当 A 非零（即直线不水平）时成功
   */
  bool point_at_y(T y, Point<T> &out) const {
    if (std::abs(A_) < 1e-15) {
      return false;
    }
    const T x = -(B_ * y + C_) / A_;
    out = Point<T>(x, y);
    return true;
  }

  /**
   * @brief 计算两直线交点
   * @param other 另一直线
   * @param out   交点输出
   * @param tol   行列式阈值（平行判定）
   * @return true 当两直线有效且相交（非平行）
   */
  bool intersection_with(const Line &other, Point<T> &out,
                        double tol = 1e-12) const {
    if (!is_valid() || !other.is_valid()) {
      return false;
    }
    const double det =
        static_cast<double>(A_) * static_cast<double>(other.B_) -
        static_cast<double>(B_) * static_cast<double>(other.A_);
    if (std::abs(det) < tol) {
      return false;
    }
    const double x =
        (static_cast<double>(B_) * static_cast<double>(other.C_) -
         static_cast<double>(C_) * static_cast<double>(other.B_)) /
        det;
    const double y =
        (static_cast<double>(C_) * static_cast<double>(other.A_) -
         static_cast<double>(A_) * static_cast<double>(other.C_)) /
        det;
    out = Point<T>(static_cast<T>(x), static_cast<T>(y));
    return true;
  }

  /**
   * @brief 获取直线的方向向量 (-B, A)
   * @return 方向向量
   */
  Point<T> direction_vector() const { return Point<T>(-B_, A_); }

  /**
   * @brief 获取直线的法向量 (A, B)
   * @return 法向量
   */
  Point<T> normal_vector() const { return Point<T>(A_, B_); }

  /**
   * @brief 计算两直线的锐角夹角 [0, π/2]
   * @param other 另一直线
   * @return 锐角弧度数；若任一方向向量近似为零向量则返回 0
   */
  double angle_to(const Line &other) const {
    const Point<T> d1 = direction_vector();
    const Point<T> d2 = other.direction_vector();
    double n1 = std::sqrt(static_cast<double>(d1.dot(d1)));
    double n2 = std::sqrt(static_cast<double>(d2.dot(d2)));
    if (n1 < 1e-15 || n2 < 1e-15) {
      return 0.0;
    }
    double cosine =
        static_cast<double>(d1.dot(d2)) / (n1 * n2);
    if (cosine > 1.0) cosine = 1.0;
    if (cosine < -1.0) cosine = -1.0;
    const double acute = std::acos(std::abs(cosine));
    if (acute > M_PI / 2.0) {
      return M_PI - acute;
    }
    return acute;
  }

  /**
   * @brief 点到直线的距离
   * @param p 点
   * @return |A*px + B*py + C| / sqrt(A² + B²)；若分母为零返回 0
   */
  double distance_to(const Point<T> &p) const {
    const double num =
        std::abs(static_cast<double>(A_ * p.x() + B_ * p.y() + C_));
    const double den =
        std::sqrt(static_cast<double>(A_ * A_ + B_ * B_));
    if (den < 1e-15) {
      return 0.0;
    }
    return num / den;
  }

  /**
   * @brief 两条平行线之间的距离
   * @param other 另一直线
   * @return 距离；若两线不平行返回 0（相交）；若法向量为零返回 0
   */
  double distance_to(const Line &other) const {
    if (!is_parallel_to(other)) {
      return 0.0;
    }
    const double len2 =
        static_cast<double>(A_ * A_ + B_ * B_);
    if (len2 < 1e-15) {
      return 0.0;
    }
    return std::abs(static_cast<double>(C_ - other.C_)) /
           std::sqrt(len2);
  }

  Point<T> foot_of_perpendicular(const Point<T> &p) const {
    const double A = static_cast<double>(A_);
    const double B = static_cast<double>(B_);
    const double C = static_cast<double>(C_);
    const double denom = A * A + B * B;
    if (denom < 1e-15) return Point<T>(p);
    const double t = -(A * p.x() + B * p.y() + C) / denom;
    return Point<T>(static_cast<T>(p.x() + A * t),
                    static_cast<T>(p.y() + B * t));
  }

  Point<T> reflected_point(const Point<T> &p) const {
    const Point<T> foot = foot_of_perpendicular(p);
    return Point<T>(static_cast<T>(2.0 * foot.x() - p.x()),
                    static_cast<T>(2.0 * foot.y() - p.y()));
  }

  bool angle_bisector(const Line &other, Line &bisector1,
                      Line &bisector2) const {
    if (!is_valid() || !other.is_valid()) return false;
    if (is_parallel_to(other)) return false;
    Point<T> inter;
    if (!intersection_with(other, inter)) return false;
    const double len1 = std::sqrt(static_cast<double>(A_ * A_ + B_ * B_));
    const double len2 = std::sqrt(static_cast<double>(other.A_ * other.A_ + other.B_ * other.B_));
    if (len1 < 1e-15 || len2 < 1e-15) return false;
    const double nx1 = static_cast<double>(A_) / len1;
    const double ny1 = static_cast<double>(B_) / len1;
    const double nx2 = static_cast<double>(other.A_) / len2;
    const double ny2 = static_cast<double>(other.B_) / len2;
    const double p = nx1 + nx2;
    const double q = ny1 + ny2;
    const double r = nx1 - nx2;
    const double s = ny1 - ny2;
    if (std::abs(p * p + q * q) > 1e-15) {
      bisector1 = Line<T>(static_cast<T>(p), static_cast<T>(q),
                           static_cast<T>(-(p * inter.x() + q * inter.y())));
    }
    if (std::abs(r * r + s * s) > 1e-15) {
      bisector2 = Line<T>(static_cast<T>(r), static_cast<T>(s),
                           static_cast<T>(-(r * inter.x() + s * inter.y())));
    }
    return true;
  }

  /**
   * @brief 流输出，格式 "{A,B,C}"
   * @param os 输出流
   * @param l  待输出直线
   * @return 输出流引用
   */
  friend std::ostream &operator<<(std::ostream &os, const Line &l) {
    os << "{" << l.A_ << "," << l.B_ << "," << l.C_ << "}";
    return os;
  }
};

}  // namespace planeGeometry

#endif
