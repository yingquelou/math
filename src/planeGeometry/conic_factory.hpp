#ifndef PLANE_GEOMETRY_CONIC_FACTORY_HPP
#define PLANE_GEOMETRY_CONIC_FACTORY_HPP

#include <cmath>
#include <cstdlib>
#include <new>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define PLANE_GEOMETRY_HAS_STD_VARIANT 1
#include <variant>
#else
#define PLANE_GEOMETRY_HAS_STD_VARIANT 0
#endif

#include "Circle.hpp"
#include "Ellipse.hpp"
#include "Hyperbola.hpp"
#include "Parabola.hpp"

namespace planeGeometry {

/**
 * @brief Namespace for internal implementation details of the conic
 *        factory. Not part of the public API.
 */
namespace detail {

/**
 * @brief Type-erased storage for any of the supported conic types.
 *
 * Uses manual type erasure (aligned storage + a kind tag) so that the
 * same public `conic_variant` interface can be provided on pre-C++17
 * compilers that lack `std::variant`.
 *
 * @tparam T Numeric type used by the stored conic.
 */
template <typename T> struct conic_storage {
  /** @brief The set of concrete conic kinds that can be stored. */
  enum class kind { monostate_k, circle_k, ellipse_k, parabola_k, hyperbola_k };

  /** @brief The currently stored kind. */
  kind which;
  /** @brief Raw storage for the concrete conic object.
   *
   *  Sized to hold the largest of the supported conic types, whose
   *  size is determined at compile time via `sizeof` over each type.
   */
  static const size_t kStorageSize =
      (sizeof(Circle<T>) > sizeof(Ellipse<T>) ? sizeof(Circle<T>)
                                              : sizeof(Ellipse<T>));
  static const size_t kStorageAlign =
      (std::alignment_of<Circle<T>>::value >
               std::alignment_of<Ellipse<T>>::value
           ? std::alignment_of<Circle<T>>::value
           : std::alignment_of<Ellipse<T>>::value);
  typedef typename std::aligned_storage<kStorageSize, kStorageAlign>::type
      storage_type;
  storage_type storage;

  /** @brief Default constructor: constructs a `monostate`. */
  conic_storage() noexcept : which(kind::monostate_k) {}

  /** @brief Destructor: destroys the currently stored object (if any). */
  ~conic_storage() { destroy(); }

  /** @brief Copy constructor. */
  conic_storage(const conic_storage &other) : which(kind::monostate_k) {
    copy_from(other);
  }
  /** @brief Move constructor. */
  conic_storage(conic_storage &&other) noexcept : which(kind::monostate_k) {
    move_from(std::move(other));
  }

  /** @brief Copy assignment. */
  conic_storage &operator=(const conic_storage &other) {
    if (this != &other) {
      destroy();
      copy_from(other);
    }
    return *this;
  }
  /** @brief Move assignment. */
  conic_storage &operator=(conic_storage &&other) noexcept {
    if (this != &other) {
      destroy();
      move_from(std::move(other));
    }
    return *this;
  }

  /** @brief Destroy the currently stored object, if any. */
  void destroy() {
    switch (which) {
    case kind::circle_k:
      reinterpret_cast<Circle<T> *>(&storage)->~Circle<T>();
      break;
    case kind::ellipse_k:
      reinterpret_cast<Ellipse<T> *>(&storage)->~Ellipse<T>();
      break;
    case kind::parabola_k:
      reinterpret_cast<Parabola<T> *>(&storage)->~Parabola<T>();
      break;
    case kind::hyperbola_k:
      reinterpret_cast<Hyperbola<T> *>(&storage)->~Hyperbola<T>();
      break;
    case kind::monostate_k:
      break;
    }
    which = kind::monostate_k;
  }

  /**
   * @brief Construct a concrete conic in place.
   * @tparam U Type of the conic to construct (normally deduced from @p u).
   * @param u  Conic instance to construct from (forwarding reference).
   */
  template <typename U> void construct(U &&u) {
    typedef typename std::remove_reference<U>::type V;
    ::new (&storage) V(std::forward<U>(u));
    which = kind_for<V>();
  }

  /**
   * @brief Return the `kind` enum value corresponding to the concrete
   *        conic type `V`.
   * @tparam U Concrete conic type.
   * @return The matching `kind` value (or `monostate_k` if not a conic).
   */
  template <typename U> static kind kind_for() {
    typedef
        typename std::remove_cv<typename std::remove_reference<U>::type>::type
            plain;
    if (std::is_same<plain, Circle<T>>::value)
      return kind::circle_k;
    if (std::is_same<plain, Ellipse<T>>::value)
      return kind::ellipse_k;
    if (std::is_same<plain, Parabola<T>>::value)
      return kind::parabola_k;
    if (std::is_same<plain, Hyperbola<T>>::value)
      return kind::hyperbola_k;
    return kind::monostate_k;
  }

  /**
   * @brief Apply a visitor to the currently stored conic.
   *
   * The visitor must be callable with `const Circle<T>&`,
   * `const Ellipse<T>&`, `const Parabola<T>&`, `const Hyperbola<T>&`,
   * and with no arguments (for the `monostate` case).
   *
   * @tparam Visitor Visitor type.
   * @param visitor Visitor instance (forwarding reference).
   * @return The result of the visitor call; the exact return type is
   *         given by `Visitor::result_t`.
   */
  template <typename Visitor>
  typename Visitor::result_t apply(Visitor &&visitor) const {
    switch (which) {
    case kind::circle_k:
      return std::forward<Visitor>(visitor)(
          *reinterpret_cast<const Circle<T> *>(&storage));
    case kind::ellipse_k:
      return std::forward<Visitor>(visitor)(
          *reinterpret_cast<const Ellipse<T> *>(&storage));
    case kind::parabola_k:
      return std::forward<Visitor>(visitor)(
          *reinterpret_cast<const Parabola<T> *>(&storage));
    case kind::hyperbola_k:
      return std::forward<Visitor>(visitor)(
          *reinterpret_cast<const Hyperbola<T> *>(&storage));
    case kind::monostate_k:
      return std::forward<Visitor>(visitor)();
    }
    return std::forward<Visitor>(visitor)();
  }

private:
  /** @brief Copy-construct the stored object from @p other. */
  void copy_from(const conic_storage &other) {
    switch (other.which) {
    case kind::circle_k:
      construct(*reinterpret_cast<const Circle<T> *>(&other.storage));
      break;
    case kind::ellipse_k:
      construct(*reinterpret_cast<const Ellipse<T> *>(&other.storage));
      break;
    case kind::parabola_k:
      construct(*reinterpret_cast<const Parabola<T> *>(&other.storage));
      break;
    case kind::hyperbola_k:
      construct(*reinterpret_cast<const Hyperbola<T> *>(&other.storage));
      break;
    case kind::monostate_k:
      which = kind::monostate_k;
      break;
    }
  }

  /** @brief Move-construct the stored object from @p other. */
  void move_from(conic_storage &&other) {
    switch (other.which) {
    case kind::circle_k:
      construct(std::move(*reinterpret_cast<Circle<T> *>(&other.storage)));
      break;
    case kind::ellipse_k:
      construct(std::move(*reinterpret_cast<Ellipse<T> *>(&other.storage)));
      break;
    case kind::parabola_k:
      construct(std::move(*reinterpret_cast<Parabola<T> *>(&other.storage)));
      break;
    case kind::hyperbola_k:
      construct(std::move(*reinterpret_cast<Hyperbola<T> *>(&other.storage)));
      break;
    case kind::monostate_k:
      which = kind::monostate_k;
      break;
    }
  }
};

} // namespace detail

#if PLANE_GEOMETRY_HAS_STD_VARIANT

/**
 * @brief `conic_variant` is simply `std::variant<monostate, Circle<T>,
 *        Ellipse<T>, Parabola<T>, Hyperbola<T>>` when the compiler
 *        provides `<variant>` (C++17 or newer).
 *
 * @tparam T Numeric type.
 */
template <typename T>
using conic_variant = std::variant<std::monostate, Circle<T>, Ellipse<T>,
                                   Parabola<T>, Hyperbola<T>>;

#else

/**
 * @brief A type-erased discriminated union of the four concrete conic
 *        types plus an empty-state option, used on pre-C++17 compilers.
 *
 * Provides a similar interface to `std::variant`, with `which()` and
 * `visit()` as the main entry points.
 *
 * @tparam T Numeric type.
 */
template <typename T> class conic_variant {
public:
  /** @brief Construct an empty (monostate) variant. */
  conic_variant() noexcept {
    storage_.which = detail::conic_storage<T>::kind::monostate_k;
  }

  /** @brief Construct from a Circle. */
  conic_variant(const Circle<T> &c) { storage_.construct(c); }
  /** @brief Construct from a Circle (rvalue). */
  conic_variant(Circle<T> &&c) { storage_.construct(std::move(c)); }
  /** @brief Construct from an Ellipse. */
  conic_variant(const Ellipse<T> &e) { storage_.construct(e); }
  /** @brief Construct from an Ellipse (rvalue). */
  conic_variant(Ellipse<T> &&e) { storage_.construct(std::move(e)); }
  /** @brief Construct from a Parabola. */
  conic_variant(const Parabola<T> &p) { storage_.construct(p); }
  /** @brief Construct from a Parabola (rvalue). */
  conic_variant(Parabola<T> &&p) { storage_.construct(std::move(p)); }
  /** @brief Construct from a Hyperbola. */
  conic_variant(const Hyperbola<T> &h) { storage_.construct(h); }
  /** @brief Construct from a Hyperbola (rvalue). */
  conic_variant(Hyperbola<T> &&h) { storage_.construct(std::move(h)); }

  conic_variant(const conic_variant &) = default;
  conic_variant(conic_variant &&) noexcept = default;
  conic_variant &operator=(const conic_variant &) = default;
  conic_variant &operator=(conic_variant &&) noexcept = default;

  /**
   * @brief Assign a new concrete conic.
   * @tparam U Type of the conic (normally deduced).
   * @param u  Conic to assign.
   * @return Reference to `*this`.
   */
  template <typename U> conic_variant &operator=(U &&u) {
    storage_.construct(std::forward<U>(u));
    return *this;
  }

  /** @brief Enumeration of the possible variant states. */
  enum class index_t {
    monostate = 0,
    circle = 1,
    ellipse = 2,
    parabola = 3,
    hyperbola = 4
  };

  /**
   * @brief Return the current variant state as an `index_t` enum.
   * @return The current state.
   */
  index_t which() const noexcept {
    return static_cast<index_t>(storage_.which);
  }

  /**
   * @brief Apply a const visitor.
   * @tparam Visitor Visitor type.
   * @param visitor  Visitor instance (forwarding reference).
   * @return The visitor's result.
   */
  template <typename Visitor>
  typename Visitor::result_t visit(Visitor &&visitor) const {
    return storage_.apply(std::forward<Visitor>(visitor));
  }

  /**
   * @brief Apply a mutable visitor.
   * @tparam Visitor Visitor type.
   * @param visitor  Visitor instance (forwarding reference).
   * @return The visitor's result.
   */
  template <typename Visitor>
  typename Visitor::result_t visit(Visitor &&visitor) {
    return storage_.apply(std::forward<Visitor>(visitor));
  }

private:
  /** @brief The type-erased storage. */
  detail::conic_storage<T> storage_;
};

#endif

/**
 * @brief Factory that classifies a general quadratic equation by its
 *        discriminant and returns the matching concrete conic type.
 *
 * Given the general-form coefficients
 * @math{
 *   F(x,y) = a x^2 + b y^2 + c x y + d x + e y + f
 * }
 * the discriminant is @f$ \Delta = c^2 - 4 a b @f$ and the classification is:
 *   @li @f$ \Delta = 0 @f$: parabola (or degenerate parallel lines).
 *   @li @f$ \Delta < 0 @f$: ellipse (or circle when @f$ a \approx b @f$
 *       and @f$ c \approx 0 @f$).
 *   @li @f$ \Delta > 0 @f$: hyperbola.
 *
 * @tparam T Numeric type (double, float, etc.).
 * @param a Coefficient @f$ a @f$.
 * @param b Coefficient @f$ b @f$.
 * @param c Coefficient @f$ c @f$.
 * @param d Coefficient @f$ d @f$.
 * @param e Coefficient @f$ e @f$.
 * @param f Constant term @f$ f @f$.
 * @return A `conic_variant<T>` holding the concrete conic type.
 * @throws std::invalid_argument if coefficients are degenerate (e.g. a
 *         parabola that turns out to be parallel lines).
 */
template <typename T>
conic_variant<T> make_conic(T a, T b, T c, T d, T e, T f) {
  const double A = static_cast<double>(a);
  const double B = static_cast<double>(b);
  const double C = static_cast<double>(c);
  const double delta = C * C - 4.0 * A * B;
  const double tol = 1e-9;

  if (std::abs(delta) < tol) {
    return conic_variant<T>(Parabola<T>::from_general(a, b, c, d, e, f));
  } else if (delta < 0.0) {
    if (std::abs(A - B) < tol * 10.0 && std::abs(C) < tol * 10.0) {
      return conic_variant<T>(Circle<T>::from_general(a, b, c, d, e, f));
    }
    return conic_variant<T>(Ellipse<T>::from_general(a, b, c, d, e, f));
  } else {
    return conic_variant<T>(Hyperbola<T>::from_general(a, b, c, d, e, f));
  }
}

#if PLANE_GEOMETRY_HAS_STD_VARIANT

/**
 * @brief Visit a `conic_variant` with a callable.
 *
 * Dispatch is performed via `std::visit`; no runtime enum switch is
 * performed by the factory layer itself — type resolution is handled by
 * the variant machinery.
 *
 * @tparam T        Numeric type.
 * @tparam Visitor  Visitor callable type.
 * @param v         The variant to visit.
 * @param visitor   Callable to dispatch.
 * @return The result of the visitor call.
 */
template <typename T, typename Visitor>
auto visit_conic(const conic_variant<T> &v, Visitor &&visitor)
    -> decltype(std::visit(std::forward<Visitor>(visitor), v)) {
  return std::visit(std::forward<Visitor>(visitor), v);
}

#else

/** @brief Visit a `conic_variant` (const overload). */
template <typename T, typename Visitor>
auto visit_conic(const conic_variant<T> &v, Visitor &&visitor)
    -> decltype(v.visit(std::forward<Visitor>(visitor))) {
  return v.visit(std::forward<Visitor>(visitor));
}

/** @brief Visit a `conic_variant` (mutable overload). */
template <typename T, typename Visitor>
auto visit_conic(conic_variant<T> &v, Visitor &&visitor)
    -> decltype(v.visit(std::forward<Visitor>(visitor))) {
  return v.visit(std::forward<Visitor>(visitor));
}

/** @brief Visit a `conic_variant` (rvalue overload). */
template <typename T, typename Visitor>
auto visit_conic(conic_variant<T> &&v, Visitor &&visitor)
    -> decltype(v.visit(std::forward<Visitor>(visitor))) {
  return v.visit(std::forward<Visitor>(visitor));
}

#endif

} // namespace planeGeometry

#endif
