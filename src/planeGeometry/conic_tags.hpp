#ifndef PLANE_GEOMETRY_CONIC_TAGS_HPP
#define PLANE_GEOMETRY_CONIC_TAGS_HPP

namespace planeGeometry {

/**
 * @brief Tag type marking an ellipse (including circle as a special case).
 */
struct ellipse_tag {};

/**
 * @brief Tag type marking a parabola.
 */
struct parabola_tag {};

/**
 * @brief Tag type marking a hyperbola.
 */
struct hyperbola_tag {};

/**
 * @brief Tag type marking a degenerate conic (e.g. a pair of parallel
 *        lines or an empty locus).
 */
struct degenerate_tag {};

/**
 * @brief A unified, type-erased tag that can be constructed from any of
 *        the concrete conic tag types.
 *
 * `conic_tag` is the single runtime/compile-time token used for
 * tag-based dispatch across the conic system.
 */
struct conic_tag {
  /** @brief Implicitly construct from an ellipse tag. */
  constexpr conic_tag(ellipse_tag) noexcept {}
  /** @brief Implicitly construct from a parabola tag. */
  constexpr conic_tag(parabola_tag) noexcept {}
  /** @brief Implicitly construct from a hyperbola tag. */
  constexpr conic_tag(hyperbola_tag) noexcept {}
  /** @brief Implicitly construct from a degenerate tag. */
  constexpr conic_tag(degenerate_tag) noexcept {}
};

/**
 * @brief Primary template: maps a concrete tag to its own type.
 *
 * Specializations below provide the mapping for each tag. The primary
 * template is intentionally left undefined so that unsupported tag
 * types produce a clear compile error.
 *
 * @tparam Tag One of the concrete tag types (`ellipse_tag`, etc.).
 */
template <typename Tag>
struct conic_tag_of;

/** @brief `ellipse_tag -> ellipse_tag`. */
template <>
struct conic_tag_of<ellipse_tag> {
  using type = ellipse_tag;
};
/** @brief `parabola_tag -> parabola_tag`. */
template <>
struct conic_tag_of<parabola_tag> {
  using type = parabola_tag;
};
/** @brief `hyperbola_tag -> hyperbola_tag`. */
template <>
struct conic_tag_of<hyperbola_tag> {
  using type = hyperbola_tag;
};
/** @brief `degenerate_tag -> degenerate_tag`. */
template <>
struct conic_tag_of<degenerate_tag> {
  using type = degenerate_tag;
};

/**
 * @brief Factory function: returns a `conic_tag` constructed from the
 *        concrete tag passed as argument.
 * @tparam Tag A concrete tag type.
 * @param tag An instance of the concrete tag (unused, only its type matters).
 * @return A `conic_tag` value representing the same category.
 */
template <typename Tag>
inline constexpr conic_tag make_conic_tag(Tag) noexcept {
  return typename conic_tag_of<Tag>::type{};
}

}  // namespace planeGeometry

#endif
