#ifndef TENSOR_HPP
#define TENSOR_HPP 1

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace tensor {

namespace detail {

template <typename T>
struct is_numeric
    : std::integral_constant<bool, std::is_integral<T>::value ||
                                       std::is_floating_point<T>::value> {};

template <typename T, typename = void>
struct has_unary_plus : std::false_type {};

template <typename T>
struct has_unary_plus<T, decltype(void(+std::declval<T>()))> : std::true_type {
};

template <typename T, typename = void>
struct has_unary_minus : std::false_type {};

template <typename T>
struct has_unary_minus<T, decltype(void(-std::declval<T>()))> : std::true_type {
};

template <typename T, typename = void> struct has_add : std::false_type {};

template <typename T>
struct has_add<T,
               decltype(void(std::declval<T &>() += std::declval<const T &>()))>
    : std::true_type {};

template <typename T, typename = void> struct has_sub : std::false_type {};

template <typename T>
struct has_sub<T,
               decltype(void(std::declval<T &>() -= std::declval<const T &>()))>
    : std::true_type {};

template <typename T, typename = void> struct has_mul : std::false_type {};

template <typename T>
struct has_mul<T,
               decltype(void(std::declval<T &>() *= std::declval<const T &>()))>
    : std::true_type {};

template <typename T, typename = void> struct has_div : std::false_type {};

template <typename T>
struct has_div<T,
               decltype(void(std::declval<T &>() /= std::declval<const T &>()))>
    : std::true_type {};

} // namespace detail

// ---------------------------------------------------------------------------
//  TENSOR_FRIEND_BRIDGE(Derived)
//
//  Expands to the minimal set of friend / member operator declarations that
//  forward arithmetic expressions to the SFINAE-guarded helpers inherited
//  from TensorBase<Derived, T, N>.  Using a macro keeps the three derived
//  classes (Tensor<T,N>, Tensor<T,1>, Tensor<T,2>) in sync while still
//  producing genuine ADL-visible free functions (required because some
//  operators, e.g. `s * a` and `a + b`, must be callable without an
//  explicit class qualifier).
//
//  Generated overloads:
//    unary  : operator+(a), operator-(a)
//    binary : operator+(a, b), operator-(a, b)        (element-wise)
//    scalar : operator*(a, s), operator*(s, a)        (element-wise scaling)
//    scalar : operator/(a, s)                          (element-wise scaling)
//    assign : operator+=, operator-=, operator*=, operator/=
// ---------------------------------------------------------------------------

#define TENSOR_FRIEND_BRIDGE(Derived)                                          \
  template <typename U = T, typename = typename std::enable_if<                \
                                detail::has_unary_plus<U>::value>::type>       \
  friend Derived operator+(const Derived &a) {                                 \
    return a.op_unary_plus();                                                  \
  }                                                                            \
                                                                               \
  template <typename U = T, typename = typename std::enable_if<                \
                                detail::has_unary_minus<U>::value>::type>      \
  friend Derived operator-(const Derived &a) {                                 \
    return a.op_unary_minus();                                                 \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_add<U>::value>::type>      \
  friend Derived operator+(const Derived &a, const Derived &b) {               \
    return a.op_add(b);                                                        \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_sub<U>::value>::type>      \
  friend Derived operator-(const Derived &a, const Derived &b) {               \
    return a.op_sub(b);                                                        \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_mul<U>::value>::type>      \
  friend Derived operator*(const Derived &a, const T &s) {                     \
    return a.op_mul(s);                                                        \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_mul<U>::value>::type>      \
  friend Derived operator*(const T &s, const Derived &a) {                     \
    return a.op_mul(s);                                                        \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_div<U>::value>::type>      \
  friend Derived operator/(const Derived &a, const T &s) {                     \
    return a.op_div(s);                                                        \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_add<U>::value>::type>      \
  Derived &operator+=(const Derived &rhs) {                                    \
    return this->op_add_assign(rhs);                                           \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_sub<U>::value>::type>      \
  Derived &operator-=(const Derived &rhs) {                                    \
    return this->op_sub_assign(rhs);                                           \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_mul<U>::value>::type>      \
  Derived &operator*=(const T &s) {                                            \
    return this->op_mul_assign(s);                                             \
  }                                                                            \
                                                                               \
  template <typename U = T,                                                    \
            typename =                                                         \
                typename std::enable_if<detail::has_div<U>::value>::type>      \
  Derived &operator/=(const T &s) {                                            \
    return this->op_div_assign(s);                                             \
  }

// Forward declarations
template <typename T, std::size_t N> class Tensor;

template <typename T> class Tensor<T, 1>;

template <typename T> class Tensor<T, 2>;

// ============================================================================
//  CRTP base: TensorBase<Derived, T, N>
//
//  Holds all shared state and shared implementation used by the primary
//  template (N-D), the vector specialization (N=1) and the matrix
//  specialization (N=2): storage, constructor family, shape/size/dim/ndim,
//  operator[], fill, reshape, operator==/!=, check_compatible and the
//  full set of SFINAE-guarded arithmetic helpers.
//
//  Derived classes expose the arithmetic via a minimal "friend bridge"
//  pattern so that ADL continues to work on unqualified calls like
//  `a + b`, `a * s`, `s * a` and `a / s`.
// ============================================================================

/**
 * @brief CRTP 基类，提供所有派生 Tensor 类型共享的基础功能
 *
 * TensorBase<Derived, T, N> 以 CRTP 模式派生，为 Tensor<T,N>、
 * Tensor<T,1>、Tensor<T,2> 提供统一的 shape/size 接口、元素访问、
 * fill/reshape、基础算术运算等。
 *
 * @tparam Derived 派生类自身类型（CRTP 派生参数）
 * @tparam T       元素类型，须满足 detail::is_numeric（整数或浮点）
 * @tparam N       张量秩（维度数），N > 0
 */
template <typename Derived, typename T, std::size_t N> class TensorBase {
  static_assert(N > 0, "Tensor rank must be greater than 0");

public:
  using value_type = T;
  using size_type = std::size_t;
  using shape_type = std::array<size_type, N>;

protected:
  shape_type shape_;    ///< Shape (dimensions).
  std::vector<T> data_; ///< Flat storage (row-major).

public:
  // ------------------------------------------------------------------
  //  Constructors (shared across all derived classes).
  // ------------------------------------------------------------------

  TensorBase() { shape_.fill(0); }

  explicit TensorBase(const shape_type &shape) : shape_(shape) {
    data_.resize(compute_size(shape_), T());
  }

  TensorBase(const shape_type &shape, const T &value) : shape_(shape) {
    data_.resize(compute_size(shape_), value);
  }

  TensorBase(const shape_type &shape, std::initializer_list<T> list)
      : shape_(shape) {
    const size_type total = compute_size(shape_);
    if (list.size() != total) {
      throw std::invalid_argument(
          "Tensor: initializer_list size does not match shape");
    }
    data_.assign(list.begin(), list.end());
  }

  TensorBase(const TensorBase &) = default;
  TensorBase(TensorBase &&) noexcept = default;
  TensorBase &operator=(const TensorBase &) = default;
  TensorBase &operator=(TensorBase &&) noexcept = default;
  ~TensorBase() = default;

  // ------------------------------------------------------------------
  //  Shape / size / dim / ndim
  // ------------------------------------------------------------------

  /** @brief 获取形状数组（各维度长度） */
  const shape_type &shape() const noexcept { return shape_; }

  /** @brief 获取元素总数（所有维度长度的乘积） */
  size_type size() const noexcept { return data_.size(); }

  /**
   * @brief 获取指定维度的长度
   * @param i 维度索引（0 基）
   * @return 第 i 维长度
   * @throws std::out_of_range 当 i >= N
   */
  size_type dim(std::size_t i) const {
    if (i >= N) {
      throw std::out_of_range("Tensor::dim: dimension index out of range");
    }
    return shape_[i];
  }

  /** @brief 获取张量秩（维度数 N） */
  constexpr std::size_t ndim() const noexcept { return N; }

  // ------------------------------------------------------------------
  //  Element access (flat)
  // ------------------------------------------------------------------

  /** @brief 按扁平化索引访问元素（只读） */
  const T &operator[](size_type i) const { return data_[i]; }
  /** @brief 按扁平化索引访问元素（可写） */
  T &operator[](size_type i) { return data_[i]; }

  // ------------------------------------------------------------------
  //  Fill / reshape
  // ------------------------------------------------------------------

  /** @brief 将所有元素填充为 value */
  void fill(const T &value) { std::fill(data_.begin(), data_.end(), value); }

  void reshape(const shape_type &new_shape) {
    const size_type new_total = compute_size(new_shape);
    if (new_total != data_.size()) {
      throw std::invalid_argument("Tensor::reshape: size mismatch");
    }
    shape_ = new_shape;
  }

  // ------------------------------------------------------------------
  //  Static factories
  // ------------------------------------------------------------------

  static Derived zeros(const shape_type &shape) { return Derived(shape, T()); }

  static Derived ones(const shape_type &shape) { return Derived(shape, T(1)); }

  // ------------------------------------------------------------------
  //  Element-wise equality
  // ------------------------------------------------------------------

  bool operator==(const Derived &rhs) const {
    return shape_ == rhs.shape_ && data_ == rhs.data_;
  }

  bool operator!=(const Derived &rhs) const { return !(*this == rhs); }

  bool allclose(const Derived &rhs, T tol = T(1e-9)) const {
    if (shape_ != rhs.shape_)
      return false;
    for (std::size_t i = 0; i < data_.size(); ++i) {
      using std::abs;
      if (abs(data_[i] - rhs.data_[i]) > tol)
        return false;
    }
    return true;
  }

  // ------------------------------------------------------------------
  //  SFINAE arithmetic helpers (implemented once, inherited by all
  //  derived classes; exposed via friend bridges in Derived).
  // ------------------------------------------------------------------

  template <typename U = T, typename = typename std::enable_if<
                                detail::has_unary_plus<U>::value>::type>
  Derived op_unary_plus() const {
    Derived r(this->shape_);
    std::transform(this->data_.begin(), this->data_.end(), r.data_.begin(),
                   [](const T &v) -> T { return +v; });
    return r;
  }

  template <typename U = T, typename = typename std::enable_if<
                                detail::has_unary_minus<U>::value>::type>
  Derived op_unary_minus() const {
    Derived r(this->shape_);
    std::transform(this->data_.begin(), this->data_.end(), r.data_.begin(),
                   [](const T &v) -> T { return -v; });
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value>::type>
  Derived &op_add_assign(const Derived &rhs) {
    check_compatible(*static_cast<const Derived *>(this), rhs);
    for (std::size_t i = 0; i < this->data_.size(); ++i)
      this->data_[i] += rhs.data_[i];
    return *static_cast<Derived *>(this);
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_sub<U>::value>::type>
  Derived &op_sub_assign(const Derived &rhs) {
    check_compatible(*static_cast<const Derived *>(this), rhs);
    for (std::size_t i = 0; i < this->data_.size(); ++i)
      this->data_[i] -= rhs.data_[i];
    return *static_cast<Derived *>(this);
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_mul<U>::value>::type>
  Derived &op_mul_assign(const T &s) {
    for (auto &v : this->data_)
      v *= s;
    return *static_cast<Derived *>(this);
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_div<U>::value>::type>
  Derived &op_div_assign(const T &s) {
    for (auto &v : this->data_)
      v /= s;
    return *static_cast<Derived *>(this);
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value>::type>
  Derived op_add(const Derived &rhs) const {
    Derived r(*static_cast<const Derived *>(this));
    r.op_add_assign(rhs);
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_sub<U>::value>::type>
  Derived op_sub(const Derived &rhs) const {
    Derived r(*static_cast<const Derived *>(this));
    r.op_sub_assign(rhs);
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_mul<U>::value>::type>
  Derived op_mul(const T &s) const {
    Derived r(*static_cast<const Derived *>(this));
    r.op_mul_assign(s);
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_div<U>::value>::type>
  Derived op_div(const T &s) const {
    Derived r(*static_cast<const Derived *>(this));
    r.op_div_assign(s);
    return r;
  }

protected:
  static size_type compute_size(const shape_type &s) {
    size_type total = 1;
    for (auto d : s)
      total *= d;
    return total;
  }

  static void check_compatible(const Derived &a, const Derived &b) {
    if (a.shape_ != b.shape_) {
      throw std::invalid_argument("Tensor: tensors have incompatible shapes");
    }
  }
};

// ============================================================================
//  Primary template: general N-dimensional tensor
// ============================================================================

/**
 * @brief 主模板：N 维张量
 *
 * Tensor<T, N> 表示秩为 N 的通用张量。通过 CRTP 继承 TensorBase
 * 的通用接口，并提供按维度索引的元素访问、逐元素算术运算等。
 *
 * @tparam T 元素类型（整数或浮点）
 * @tparam N 张量秩，N >= 1
 */
template <typename T, std::size_t N>
class Tensor : public TensorBase<Tensor<T, N>, T, N> {
  friend class TensorBase<Tensor<T, N>, T, N>;

public:
  using base_type = TensorBase<Tensor<T, N>, T, N>;
  using value_type = typename base_type::value_type;
  using size_type = typename base_type::size_type;
  using shape_type = typename base_type::shape_type;
  using self_type = Tensor<T, N>;

  using base_type::dim;
  using base_type::ndim;
  using base_type::shape;
  using base_type::size;
  using base_type::operator[];
  using base_type::fill;
  using base_type::reshape;
  using base_type::operator==;
  using base_type::operator!=;
  using base_type::allclose;
  using base_type::ones;
  using base_type::zeros;

protected:
  using base_type::data_;
  using base_type::shape_;

public:
  Tensor() {}
  explicit Tensor(const shape_type &shape) : base_type(shape) {}
  Tensor(const shape_type &shape, const T &value) : base_type(shape, value) {}
  Tensor(const shape_type &shape, std::initializer_list<T> list)
      : base_type(shape, list) {}

  Tensor(const self_type &) = default;
  Tensor(self_type &&) noexcept = default;
  self_type &operator=(const self_type &) = default;
  self_type &operator=(self_type &&) noexcept = default;
  ~Tensor() = default;

  // ------------------------------------------------------------------
  //  Multi-index access
  // ------------------------------------------------------------------

  template <typename... Indices>
  typename std::enable_if<sizeof...(Indices) == N, const T &>::type
  operator()(Indices... idx) const {
    return data_[offset(static_cast<size_type>(idx)...)];
  }

  template <typename... Indices>
  typename std::enable_if<sizeof...(Indices) == N, T &>::type
  operator()(Indices... idx) {
    return data_[offset(static_cast<size_type>(idx)...)];
  }

  // ------------------------------------------------------------------
  //  Friend bridges (one-line ADL forwarding to TensorBase helpers).
  //  Generated by TENSOR_FRIEND_BRIDGE(self_type); see file top for the
  //  list of operators it produces.
  // ------------------------------------------------------------------

  TENSOR_FRIEND_BRIDGE(self_type)

  friend std::ostream &operator<<(std::ostream &os, const self_type &t) {
    t.print_recursive(os, 0);
    return os;
  }

protected:
  template <typename... Indices> size_type offset(Indices... idx) const {
    static_assert(sizeof...(Indices) == N,
                  "Tensor::offset: wrong number of indices");
    size_type indices[N] = {static_cast<size_type>(idx)...};
    size_type stride = 1;
    size_type off = 0;
    for (std::size_t i = N; i-- > 0;) {
      if (indices[i] >= shape_[i]) {
        throw std::out_of_range("Tensor::operator(): index out of range");
      }
      off += indices[i] * stride;
      stride *= shape_[i];
    }
    return off;
  }

  void print_recursive(std::ostream &os, std::size_t depth) const {
    print_recursive_impl(os, depth, 0);
  }

  void print_recursive_impl(std::ostream &os, std::size_t depth,
                            size_type offset) const {
    if (depth == N - 1) {
      os << "[";
      for (size_type i = 0; i < shape_[depth]; ++i) {
        if (i)
          os << ", ";
        os << data_[offset + i];
      }
      os << "]";
    } else {
      os << "[";
      for (size_type i = 0; i < shape_[depth]; ++i) {
        if (i)
          os << ", ";
        const size_type stride = stride_from(depth + 1);
        print_recursive_impl(os, depth + 1, offset + i * stride);
      }
      os << "]";
    }
  }

  size_type stride_from(std::size_t d) const {
    size_type s = 1;
    for (std::size_t i = d; i < N; ++i)
      s *= shape_[i];
    return s;
  }
};

// ============================================================================
//  Full specialization: Tensor<T, 1>  (vector)
// ============================================================================

/**
 * @brief 全特化：Tensor<T, 1>（向量）
 *
 * Tensor<T, 1> 为一维向量，除继承 TensorBase 的通用接口外，
 * 额外提供向量点积、叉积、模、归一化、夹角等向量特有运算。
 *
 * @tparam T 元素类型（整数或浮点）
 */
template <typename T>
class Tensor<T, 1> : public TensorBase<Tensor<T, 1>, T, 1> {
  friend class TensorBase<Tensor<T, 1>, T, 1>;

public:
  using base_type = TensorBase<Tensor<T, 1>, T, 1>;
  using value_type = typename base_type::value_type;
  using size_type = typename base_type::size_type;
  using shape_type = typename base_type::shape_type;
  using self_type = Tensor<T, 1>;

  using base_type::dim;
  using base_type::ndim;
  using base_type::shape;
  using base_type::size;
  using base_type::operator[];
  using base_type::fill;
  using base_type::reshape;
  using base_type::operator==;
  using base_type::operator!=;
  using base_type::allclose;

protected:
  using base_type::data_;
  using base_type::shape_;

public:
  // ------------------------------------------------------------------
  //  Constructors (delegating to TensorBase)
  // ------------------------------------------------------------------

  Tensor() { shape_[0] = 0; }
  explicit Tensor(size_type n) : base_type(shape_type{{n}}) {}
  Tensor(size_type n, const T &value) : base_type(shape_type{{n}}, value) {}
  Tensor(std::initializer_list<T> list)
      : base_type(shape_type{{list.size()}}, list) {}
  Tensor(const shape_type &shape) : base_type(shape) {}
  Tensor(const shape_type &shape, const T &value) : base_type(shape, value) {}

  Tensor(const self_type &) = default;
  Tensor(self_type &&) noexcept = default;
  self_type &operator=(const self_type &) = default;
  self_type &operator=(self_type &&) noexcept = default;
  ~Tensor() = default;

  // ------------------------------------------------------------------
  //  operator() (bounds-checked)
  // ------------------------------------------------------------------

  const T &operator()(size_type i) const {
    if (i >= data_.size()) {
      throw std::out_of_range("Vector::operator(): index out of range");
    }
    return data_[i];
  }

  T &operator()(size_type i) {
    if (i >= data_.size()) {
      throw std::out_of_range("Vector::operator(): index out of range");
    }
    return data_[i];
  }

  // ------------------------------------------------------------------
  //  Static factories (vector-specific signatures)
  // ------------------------------------------------------------------

  using base_type::ones;
  using base_type::zeros;

  static self_type zeros(const shape_type &shape) {
    return base_type::zeros(shape);
  }
  static self_type ones(const shape_type &shape) {
    return base_type::ones(shape);
  }
  static self_type zeros(size_type n) { return self_type(n, T()); }
  static self_type ones(size_type n) { return self_type(n, T(1)); }

  // ------------------------------------------------------------------
  //  Friend bridges (one-line ADL forwarding to TensorBase helpers).
  //  Generated by TENSOR_FRIEND_BRIDGE(self_type); see file top for the
  //  list of operators it produces.
  // ------------------------------------------------------------------

  TENSOR_FRIEND_BRIDGE(self_type)

  friend std::ostream &operator<<(std::ostream &os, const self_type &v) {
    os << "[";
    for (std::size_t i = 0; i < v.data_.size(); ++i) {
      if (i)
        os << ", ";
      os << v.data_[i];
    }
    os << "]";
    return os;
  }

  // ------------------------------------------------------------------
  //  Vector-specific operations
  // ------------------------------------------------------------------

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value &&
                                               detail::has_mul<U>::value>::type>
  T dot(const self_type &other) const {
    this->check_compatible(*this, other);
    T sum = T();
    for (std::size_t i = 0; i < data_.size(); ++i)
      sum += data_[i] * other.data_[i];
    return sum;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_sub<U>::value &&
                                               detail::has_mul<U>::value>::type>
  self_type cross(const self_type &other) const {
    if (data_.size() != 3 || other.data_.size() != 3) {
      throw std::invalid_argument("Vector::cross: cross product requires "
                                  "3D vectors");
    }
    return self_type({data_[1] * other.data_[2] - data_[2] * other.data_[1],
                      data_[2] * other.data_[0] - data_[0] * other.data_[2],
                      data_[0] * other.data_[1] - data_[1] * other.data_[0]});
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value &&
                                               detail::has_mul<U>::value>::type>
  T norm() const {
    using std::sqrt;
    T sum = T();
    for (auto v : data_)
      sum += v * v;
    return sqrt(sum);
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value &&
                                               detail::has_mul<U>::value &&
                                               detail::has_div<U>::value>::type>
  self_type normalize() const {
    const T n = norm();
    if (n == T()) {
      throw std::invalid_argument("Vector::normalize: zero vector");
    }
    self_type r(*this);
    for (auto &v : r.data_)
      v /= n;
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_mul<U>::value>::type>
  self_type cwiseProduct(const self_type &other) const {
    this->check_compatible(*this, other);
    self_type r(shape_[0]);
    for (std::size_t i = 0; i < data_.size(); ++i)
      r.data_[i] = data_[i] * other.data_[i];
    return r;
  }
};

// ============================================================================
//  Full specialization: Tensor<T, 2>  (matrix)
// ============================================================================

/**
 * @brief 全特化：Tensor<T, 2>（矩阵）
 *
 * Tensor<T, 2> 为二维矩阵，除继承 TensorBase 的通用接口外，
 * 额外提供矩阵乘法、转置、迹、行列式、逆、秩、行阶梯形、
 * 行最简形、H 拼接、单位矩阵、随机矩阵等矩阵特有运算。
 *
 * @tparam T 元素类型（整数或浮点）
 */
template <typename T>
class Tensor<T, 2> : public TensorBase<Tensor<T, 2>, T, 2> {
  friend class TensorBase<Tensor<T, 2>, T, 2>;

public:
  using base_type = TensorBase<Tensor<T, 2>, T, 2>;
  using value_type = typename base_type::value_type;
  using size_type = typename base_type::size_type;
  using shape_type = typename base_type::shape_type;
  using self_type = Tensor<T, 2>;
  using vector_type = Tensor<T, 1>;

  using base_type::dim;
  using base_type::ndim;
  using base_type::shape;
  using base_type::size;
  using base_type::operator[];
  using base_type::fill;
  using base_type::reshape;
  using base_type::operator==;
  using base_type::operator!=;
  using base_type::allclose;

protected:
  using base_type::data_;
  using base_type::shape_;

public:
  // ------------------------------------------------------------------
  //  Constructors (delegating to TensorBase)
  // ------------------------------------------------------------------

  Tensor() {
    shape_[0] = 0;
    shape_[1] = 0;
  }
  Tensor(size_type rows, size_type cols)
      : base_type(shape_type{{rows, cols}}) {}
  Tensor(size_type rows, size_type cols, const T &value)
      : base_type(shape_type{{rows, cols}}, value) {}
  Tensor(const shape_type &shape) : base_type(shape) {}
  Tensor(const shape_type &shape, const T &value) : base_type(shape, value) {}
  Tensor(std::initializer_list<std::initializer_list<T>> list) {
    shape_[0] = list.size();
    shape_[1] = list.size() ? list.begin()->size() : 0;
    data_.reserve(shape_[0] * shape_[1]);
    for (const auto &row : list) {
      if (row.size() != shape_[1]) {
        throw std::invalid_argument(
            "Matrix: initializer_list rows must have equal length");
      }
      data_.insert(data_.end(), row.begin(), row.end());
    }
  }

  Tensor(const self_type &) = default;
  Tensor(self_type &&) noexcept = default;
  self_type &operator=(const self_type &) = default;
  self_type &operator=(self_type &&) noexcept = default;
  ~Tensor() = default;

  // ------------------------------------------------------------------
  //  operator() (bounds-checked)
  // ------------------------------------------------------------------

  const T &operator()(size_type i, size_type j) const {
    if (i >= shape_[0] || j >= shape_[1]) {
      throw std::out_of_range("Matrix::operator(): index out of range");
    }
    return data_[i * shape_[1] + j];
  }

  T &operator()(size_type i, size_type j) {
    if (i >= shape_[0] || j >= shape_[1]) {
      throw std::out_of_range("Matrix::operator(): index out of range");
    }
    return data_[i * shape_[1] + j];
  }

  // ------------------------------------------------------------------
  //  Dimensions helpers
  // ------------------------------------------------------------------

  size_type rows() const noexcept { return shape_[0]; }
  size_type cols() const noexcept { return shape_[1]; }

  vector_type row(size_type i) const {
    if (i >= shape_[0]) {
      throw std::out_of_range("Matrix::row: index out of range");
    }
    vector_type v(shape_[1]);
    for (size_type j = 0; j < shape_[1]; ++j)
      v[j] = (*this)(i, j);
    return v;
  }

  vector_type col(size_type j) const {
    if (j >= shape_[1]) {
      throw std::out_of_range("Matrix::col: index out of range");
    }
    vector_type v(shape_[0]);
    for (size_type i = 0; i < shape_[0]; ++i)
      v[i] = (*this)(i, j);
    return v;
  }

  // ------------------------------------------------------------------
  //  Static factories (matrix-specific signatures)
  // ------------------------------------------------------------------

  using base_type::ones;
  using base_type::zeros;

  static self_type zeros(const shape_type &shape) {
    return base_type::zeros(shape);
  }
  static self_type ones(const shape_type &shape) {
    return base_type::ones(shape);
  }
  static self_type zeros(size_type r, size_type c) {
    return self_type(r, c, T());
  }
  static self_type ones(size_type r, size_type c) {
    return self_type(r, c, T(1));
  }
  static self_type eye(size_type n) {
    self_type r(n, n, T());
    for (size_type i = 0; i < n; ++i) {
      r(i, i) = T(1);
    }
    return r;
  }

  // ------------------------------------------------------------------
  //  Friend bridges (one-line ADL forwarding to TensorBase helpers).
  //  Generated by TENSOR_FRIEND_BRIDGE(self_type); see file top for the
  //  list of operators it produces.
  //
  //  Semantics for Tensor<T,2> (matrix):
  //    a + b   element-wise matrix addition
  //    a - b   element-wise matrix subtraction
  //    a * b   matrix-matrix multiplication (member operator* below)
  //    a * v   matrix-vector multiplication (member operator* below,
  //            returns a Tensor<T,1> vector)
  //    a * s   element-wise scaling by scalar s
  //    s * a   element-wise scaling by scalar s (commutative)
  //    a / s   element-wise scaling by scalar 1/s
  //    a += b  element-wise matrix addition assignment
  //    a -= b  element-wise matrix subtraction assignment
  //    a *= s  element-wise scalar scaling assignment
  //    a /= s  element-wise scalar scaling assignment
  //  ------------------------------------------------------------------

  TENSOR_FRIEND_BRIDGE(self_type)

  friend std::ostream &operator<<(std::ostream &os, const self_type &m) {
    os << "[";
    for (size_type i = 0; i < m.shape_[0]; ++i) {
      if (i)
        os << ", ";
      os << "[";
      for (size_type j = 0; j < m.shape_[1]; ++j) {
        if (j)
          os << ", ";
        os << m(i, j);
      }
      os << "]";
    }
    os << "]";
    return os;
  }

  // ------------------------------------------------------------------
  //  Matrix-vector / matrix-matrix multiplication (genuine product)
  //
  //  These overloads deliberately share the symbol `operator*` with the
  //  scalar-scaling overloads produced by TENSOR_FRIEND_BRIDGE above;
  //  the dispatch is resolved by argument type:
  //
  //    a * b   (Matrix * Matrix)   -> matrix-matrix product (this section)
  //    a * v   (Matrix * Vector)   -> matrix-vector product (this section)
  //    a * s   (Matrix * Scalar)   -> element-wise scaling  (friend bridge)
  //    s * a   (Scalar * Matrix)   -> element-wise scaling  (friend bridge)
  //  ------------------------------------------------------------------

  // Matrix * Vector: produces the linear combination r[i] = sum_j a(i,j) *
  // v[j].
  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value &&
                                               detail::has_mul<U>::value>::type>
  vector_type operator*(const vector_type &v) const {
    if (shape_[1] != v.size()) {
      throw std::invalid_argument("Matrix * Vector: incompatible dimensions");
    }
    vector_type r(shape_[0]);
    for (size_type i = 0; i < shape_[0]; ++i) {
      T s = T();
      for (size_type j = 0; j < shape_[1]; ++j)
        s += (*this)(i, j) * v[j];
      r[i] = s;
    }
    return r;
  }

  // Matrix * Matrix: standard (row,col)-product r(i,j) = sum_k a(i,k) * b(k,j).
  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value &&
                                               detail::has_mul<U>::value>::type>
  self_type operator*(const self_type &other) const {
    if (shape_[1] != other.shape_[0]) {
      throw std::invalid_argument("Matrix * Matrix: incompatible dimensions");
    }
    self_type r(shape_[0], other.shape_[1], T());
    for (size_type i = 0; i < shape_[0]; ++i) {
      for (size_type k = 0; k < shape_[1]; ++k) {
        const T aik = (*this)(i, k);
        for (size_type j = 0; j < other.shape_[1]; ++j) {
          r(i, j) += aik * other(k, j);
        }
      }
    }
    return r;
  }

  // ------------------------------------------------------------------
  //  Matrix-specific algorithms
  // ------------------------------------------------------------------

  self_type transpose() const {
    self_type r(shape_[1], shape_[0]);
    for (size_type i = 0; i < shape_[0]; ++i)
      for (size_type j = 0; j < shape_[1]; ++j)
        r(j, i) = (*this)(i, j);
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_add<U>::value>::type>
  T trace() const {
    if (shape_[0] != shape_[1]) {
      throw std::invalid_argument("Matrix::trace: non-square matrix");
    }
    T s = T();
    for (size_type i = 0; i < shape_[0]; ++i)
      s += (*this)(i, i);
    return s;
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_sub<U>::value &&
                detail::has_mul<U>::value && detail::has_div<U>::value>::type>
  T determinant() const {
    if (shape_[0] != shape_[1]) {
      throw std::invalid_argument("Matrix::determinant: non-square matrix");
    }
    const size_type n = shape_[0];
    if (n == 0)
      return T();
    if (n == 1)
      return data_[0];
    if (n == 2) {
      return data_[0] * data_[3] - data_[1] * data_[2];
    }
    if (n == 3) {
      return sarrus_3x3();
    }
    return gauss_determinant();
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_sub<U>::value &&
                detail::has_mul<U>::value && detail::has_div<U>::value>::type>
  self_type inverse() const {
    if (shape_[0] != shape_[1]) {
      throw std::invalid_argument("Matrix::inverse: non-square matrix");
    }
    const size_type n = shape_[0];

    self_type aug(n, 2 * n, T());
    for (size_type i = 0; i < n; ++i) {
      for (size_type j = 0; j < n; ++j)
        aug(i, j) = (*this)(i, j);
      aug(i, n + i) = T(1);
    }

    for (size_type c = 0; c < n; ++c) {
      size_type pivot = c;
      for (size_type r = c + 1; r < n; ++r) {
        if (std::abs(aug(r, c)) > std::abs(aug(pivot, c)))
          pivot = r;
      }
      if (std::abs(aug(pivot, c)) < T(1e-12)) {
        throw std::invalid_argument("Matrix::inverse: singular matrix");
      }
      if (pivot != c) {
        for (size_type j = 0; j < 2 * n; ++j) {
          std::swap(aug(c, j), aug(pivot, j));
        }
      }
      const T p = aug(c, c);
      for (size_type j = 0; j < 2 * n; ++j)
        aug(c, j) /= p;
      for (size_type r = 0; r < n; ++r) {
        if (r == c)
          continue;
        const T factor = aug(r, c);
        for (size_type j = 0; j < 2 * n; ++j)
          aug(r, j) -= factor * aug(c, j);
      }
    }

    self_type inv(n, n);
    for (size_type i = 0; i < n; ++i)
      for (size_type j = 0; j < n; ++j)
        inv(i, j) = aug(i, n + j);
    return inv;
  }

  // ------------------------------------------------------------------
  //  Row operations
  // ------------------------------------------------------------------

  self_type &swap_rows(size_type i, size_type j) {
    if (i >= shape_[0] || j >= shape_[0]) {
      throw std::out_of_range("Matrix::swap_rows: row index out of range");
    }
    if (i == j)
      return *this;
    for (size_type k = 0; k < shape_[1]; ++k) {
      std::swap(data_[i * shape_[1] + k], data_[j * shape_[1] + k]);
    }
    return *this;
  }

  self_type swap_rows(size_type i, size_type j) const {
    self_type c(*this);
    return c.swap_rows(i, j);
  }

  self_type &scale_row(size_type i, T k) {
    if (i >= shape_[0]) {
      throw std::out_of_range("Matrix::scale_row: row index out of range");
    }
    for (size_type j = 0; j < shape_[1]; ++j) {
      data_[i * shape_[1] + j] *= k;
    }
    return *this;
  }

  self_type scale_row(size_type i, T k) const {
    self_type c(*this);
    return c.scale_row(i, k);
  }

  self_type &add_row_to(size_type src, T k, size_type dst) {
    if (src >= shape_[0] || dst >= shape_[0]) {
      throw std::out_of_range("Matrix::add_row_to: row index out of range");
    }
    for (size_type j = 0; j < shape_[1]; ++j) {
      data_[dst * shape_[1] + j] += k * data_[src * shape_[1] + j];
    }
    return *this;
  }

  self_type add_row_to(size_type src, T k, size_type dst) const {
    self_type c(*this);
    return c.add_row_to(src, k, dst);
  }

  self_type &to_row_echelon_form(T tol = T(1e-12)) {
    const size_type r = shape_[0];
    const size_type c = shape_[1];
    size_type row = 0;
    for (size_type col = 0; col < c && row < r; ++col) {
      size_type pivot = row;
      for (size_type k = row; k < r; ++k) {
        if (std::abs((*this)(k, col)) > std::abs((*this)(pivot, col))) {
          pivot = k;
        }
      }
      if (std::abs((*this)(pivot, col)) < tol)
        continue;
      if (pivot != row)
        swap_rows(row, pivot);
      for (size_type k = row + 1; k < r; ++k) {
        const T f = (*this)(k, col) / (*this)(row, col);
        for (size_type j = col; j < c; ++j) {
          (*this)(k, j) -= f * (*this)(row, j);
        }
      }
      ++row;
    }
    return *this;
  }

  self_type row_echelon_form() const {
    self_type c(*this);
    return c.to_row_echelon_form();
  }

  self_type &to_reduced_row_echelon_form(T tol = T(1e-12)) {
    const size_type r = shape_[0];
    const size_type c = shape_[1];
    size_type row = 0;
    for (size_type col = 0; col < c && row < r; ++col) {
      size_type pivot = row;
      for (size_type k = row; k < r; ++k) {
        if (std::abs((*this)(k, col)) > std::abs((*this)(pivot, col))) {
          pivot = k;
        }
      }
      if (std::abs((*this)(pivot, col)) < tol)
        continue;
      if (pivot != row)
        swap_rows(row, pivot);
      const T p = (*this)(row, col);
      for (size_type j = 0; j < c; ++j)
        (*this)(row, j) /= p;
      for (size_type k = 0; k < r; ++k) {
        if (k == row)
          continue;
        const T f = (*this)(k, col);
        for (size_type j = 0; j < c; ++j) {
          (*this)(k, j) -= f * (*this)(row, j);
        }
      }
      ++row;
    }
    return *this;
  }

  self_type &to_rref() { return to_reduced_row_echelon_form(); }

  self_type reduced_row_echelon_form() const {
    self_type c(*this);
    return c.to_reduced_row_echelon_form();
  }

  self_type &standard_form() { return to_reduced_row_echelon_form(); }

  self_type standard_form() const { return reduced_row_echelon_form(); }

  // ------------------------------------------------------------------
  //  Concatenation
  // ------------------------------------------------------------------

  self_type &hconcat(const self_type &other) {
    if (shape_[0] != other.shape_[0]) {
      throw std::invalid_argument("Matrix::hconcat: row counts must match");
    }
    const size_type new_cols = shape_[1] + other.shape_[1];
    std::vector<T> nd;
    nd.reserve(shape_[0] * new_cols);
    for (size_type i = 0; i < shape_[0]; ++i) {
      for (size_type j = 0; j < shape_[1]; ++j)
        nd.push_back((*this)(i, j));
      for (size_type j = 0; j < other.shape_[1]; ++j)
        nd.push_back(other(i, j));
    }
    data_.swap(nd);
    shape_[1] = new_cols;
    return *this;
  }

  self_type hconcat(const self_type &other) const {
    self_type c(*this);
    return c.hconcat(other);
  }

  self_type &operator&=(const self_type &other) { return hconcat(other); }

  friend self_type operator&(const self_type &a, const self_type &b) {
    self_type c(a);
    return c.hconcat(b);
  }

  // ------------------------------------------------------------------
  //  Utilities
  // ------------------------------------------------------------------

  bool same_shape(const self_type &other) const noexcept {
    return shape_[0] == other.shape_[0] && shape_[1] == other.shape_[1];
  }

  template <typename U = T,
            typename = typename std::enable_if<detail::has_mul<U>::value>::type>
  self_type cwiseProduct(const self_type &other) const {
    if (!same_shape(other)) {
      throw std::invalid_argument(
          "Matrix::cwiseProduct: shape mismatch");
    }
    self_type r(shape_[0], shape_[1]);
    for (std::size_t i = 0; i < data_.size(); ++i)
      r.data_[i] = data_[i] * other.data_[i];
    return r;
  }

  std::string toString() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
  }

  std::string toLaTeX() const {
    std::ostringstream oss;
    oss << "\\begin{bmatrix}";
    for (size_type i = 0; i < shape_[0]; ++i) {
      if (i)
        oss << " \\\\\n";
      for (size_type j = 0; j < shape_[1]; ++j) {
        if (j)
          oss << " & ";
        oss << (*this)(i, j);
      }
    }
    oss << "\\end{bmatrix}";
    return oss.str();
  }

  template <typename U = T>
  static typename std::enable_if<std::is_integral<U>::value, self_type>::type
  random(size_type rows, size_type cols, T lo, T hi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    self_type r(rows, cols);
    using int_type =
        typename std::conditional<std::is_same<T, bool>::value, int, T>::type;
    std::uniform_int_distribution<int_type> dist(static_cast<int_type>(lo),
                                                 static_cast<int_type>(hi));
    for (auto &v : r.data_) {
      v = static_cast<T>(dist(gen));
    }
    return r;
  }

  template <typename U = T>
  static
      typename std::enable_if<std::is_floating_point<U>::value, self_type>::type
      random(size_type rows, size_type cols, T lo, T hi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    self_type r(rows, cols);
    std::uniform_real_distribution<T> dist(lo, hi);
    for (auto &v : r.data_) {
      v = dist(gen);
    }
    return r;
  }

  explicit operator bool() const noexcept {
    return shape_[0] > 0 && shape_[1] > 0;
  }

  bool is_valid() const noexcept { return shape_[0] > 0 && shape_[1] > 0; }

  self_type leftCompatibleIdentity() const { return eye(shape_[0]); }

  self_type rightCompatibleIdentity() const { return eye(shape_[1]); }

  self_type pow(std::size_t k) const {
    if (shape_[0] != shape_[1]) {
      throw std::invalid_argument("Matrix::pow: non-square matrix");
    }
    const size_type n = shape_[0];
    self_type result = eye(n);
    self_type base(*this);
    while (k > 0) {
      if ((k & 1u) == 1u) {
        result = result * base;
      }
      k >>= 1;
      if (k > 0) {
        base = base * base;
      }
    }
    return result;
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_sub<U>::value &&
                detail::has_mul<U>::value && detail::has_div<U>::value>::type>
  size_type rank(T tol = T(1e-12)) const {
    self_type m(*this);
    m.to_row_echelon_form(tol);
    const size_type r = m.shape_[0];
    const size_type c = m.shape_[1];
    size_type result = 0;
    for (size_type i = 0; i < r; ++i) {
      bool nonzero = false;
      for (size_type j = 0; j < c; ++j) {
        if (std::abs(m(i, j)) > tol) {
          nonzero = true;
          break;
        }
      }
      if (nonzero)
        ++result;
    }
    return result;
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_mul<U>::value>::type>
  static self_type outer_product(const vector_type &u, const vector_type &v) {
    const size_type m = u.size();
    const size_type n = v.size();
    self_type r(m, n, T());
    for (size_type i = 0; i < m; ++i)
      for (size_type j = 0; j < n; ++j)
        r(i, j) = u[i] * v[j];
    return r;
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_mul<U>::value>::type>
  T frobenius_norm() const {
    T sum = T();
    for (auto v : data_)
      sum += v * v;
    using std::sqrt;
    return sqrt(sum);
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_sub<U>::value && detail::has_mul<U>::value>::type>
  bool is_symmetric(T tol = T(1e-12)) const {
    if (shape_[0] != shape_[1]) return false;
    for (size_type i = 0; i < shape_[0]; ++i)
      for (size_type j = i + 1; j < shape_[1]; ++j)
        if (std::abs((*this)(i, j) - (*this)(j, i)) > tol) return false;
    return true;
  }

  template <typename U = T,
            typename = typename std::enable_if<
                detail::has_add<U>::value && detail::has_mul<U>::value>::type>
  bool is_orthogonal(T tol = T(1e-12)) const {
    if (shape_[0] != shape_[1]) return false;
    const self_type prod = (*this) * this->transpose();
    const self_type id = eye(shape_[0]);
    for (size_type i = 0; i < shape_[0]; ++i)
      for (size_type j = 0; j < shape_[1]; ++j)
        if (std::abs(prod(i, j) - id(i, j)) > tol) return false;
    return true;
  }

private:
  T sarrus_3x3() const {
    const T &a = data_[0];
    const T &b = data_[1];
    const T &c = data_[2];
    const T &d = data_[3];
    const T &e = data_[4];
    const T &f = data_[5];
    const T &g = data_[6];
    const T &h = data_[7];
    const T &i = data_[8];
    return (a * e * i + b * f * g + c * d * h) -
           (c * e * g + b * d * i + a * f * h);
  }

  T gauss_determinant() const {
    const size_type n = shape_[0];
    self_type m(*this);
    T sign = T(1);
    constexpr T tol = T(1e-12);
    for (size_type c = 0; c < n; ++c) {
      size_type pivot = c;
      for (size_type r = c + 1; r < n; ++r) {
        if (std::abs(m(r, c)) > std::abs(m(pivot, c)))
          pivot = r;
      }
      if (std::abs(m(pivot, c)) < tol)
        return T();
      if (pivot != c) {
        m.swap_rows(c, pivot);
        sign = -sign;
      }
      for (size_type r = c + 1; r < n; ++r) {
        const T factor = m(r, c) / m(c, c);
        for (size_type j = c + 1; j < n; ++j)
          m(r, j) -= factor * m(c, j);
      }
    }
    T det = sign;
    for (size_type i = 0; i < n; ++i)
      det *= m(i, i);
    return det;
  }
};

} // namespace tensor
#endif