#include <type_traits>
#include <concepts>

// 定义一个模板类Determinant，表示行列式
template <typename T, std::size_t N>
class Determinant
{
    static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::complex<typename T::value_type>>, "T must be a numeric type or a complex type with a numeric value type");

public:
    using value_type = T;
    static constexpr std::size_t order = N;

    // 构造函数
    Determinant()
    noexcept = default;
    Determinant(const Determinant &)
    noexcept = default;
    Determinant(Determinant &&)
    noexcept = default;
    ~Determinant() noexcept = default;
    Determinant &operator=(const Determinant &) noexcept = default;
    Determinant &operator=(Determinant &&) noexcept = default;

    // 获取行列式中指定元素的值
    template <std::size_t... Is>
        requires(sizeof...(Is) == N && ((Is >= 0 && Is < N) && ...))
    [[nodiscard]] constexpr value_type get_element() const noexcept;

    // 调换行列式中指定两行或两列
    template <std::size_t I, std::size_t J>
        requires((I >= 0 && I < N) && (J >= 0 && J < N))
    constexpr void swap_rows() noexcept;
    template <std::size_t I, std::size_t J>
        requires((I >= 0 && I < N) && (J >= 0 && J < N))
    constexpr void swap_columns() noexcept;

    // 行列式的基本性质
    [[nodiscard]] constexpr value_type determinant() const noexcept;
    [[nodiscard]] constexpr Determinant inverse() const noexcept;
    [[nodiscard]] constexpr Determinant transpose() const noexcept;

    // 行列式与数的乘除法以及加减运算
    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] constexpr Determinant operator*(const U &scalar) const noexcept;
    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] constexpr friend Determinant operator*(const U &scalar, const Determinant &m) noexcept
    {
        return m * scalar;
    }
    template <typename U>
        requires std::is_arithmetic_v<U>
    constexpr Determinant &operator*=(const U &scalar) noexcept;
    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] constexpr Determinant operator/(const U &scalar) const noexcept;
    template <typename U>
        requires std::is_arithmetic_v<U>
    constexpr Determinant &operator/=(const U &scalar) noexcept;
    [[nodiscard]] constexpr Determinant operator+(const Determinant &other) const noexcept;
    constexpr Determinant &operator+=(const Determinant &other) noexcept;
    [[nodiscard]] constexpr Determinant operator-(const Determinant &other) const noexcept;
    constexpr Determinant &operator-=(const Determinant &other) noexcept;

    // 行列式与向量、矩阵的乘法
    template <typename U>
        requires(std::is_arithmetic_v<U> || std::is_same_v<U, std::complex<typename U::value_type>>)
    [[nodiscard]] constexpr decltype(auto) operator*(const std::array<U, N> &v) const noexcept;
    template <typename U, std::size_t M>
        requires(std::is_same_v<U, Determinant<typename U::value_type, M>> && M == N)
    [[nodiscard]] constexpr decltype(auto) operator*(const U &m) const noexcept;

    // 比较运算符
    [[nodiscard]] bool operator==(const Determinant &other) const noexcept;
    [[nodiscard]] bool operator!=(const Determinant &other) const noexcept;
};

// 获取行列式中指定元素的值
template <typename T, std::size_t N>
template <std::size_t... Is>
    requires(sizeof...(Is) == N && ((Is >= 0 && Is < N) && ...))
[[nodiscard]] constexpr typename Determinant<T, N>::value_type Determinant<T, N>::get_element() const noexcept
{
    // 实现
}