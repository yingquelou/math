#include <iostream>
#include <type_traits> // 包含std::enable_if_t和std::is_arithmetic_v

// 定义Fraction类
template<typename T>
class Fraction {

private:
    T numerator; // 分子
    T denominator; // 分母

public:
    // 构造函数
    Fraction(T num = 0, T den = 1) : numerator(num), denominator(den) {}

    // 获取分子和分母
    T getNumerator() const { return numerator; }
    T getDenominator() const { return denominator; }

    // 赋值运算符重载
    Fraction& operator=(const Fraction& other) {
        if (this != &other) {
            numerator = other.numerator;
            denominator = other.denominator;
        }
        return *this;
    }

    // 加法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator+(const Fraction<T>& other) const {
        T num = numerator * other.denominator + other.numerator * denominator;
        T den = denominator * other.denominator;
        return Fraction<U>(num, den);
    }

    // 减法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator-(const Fraction<T>& other) const {
        T num = numerator * other.denominator - other.numerator * denominator;
        T den = denominator * other.denominator;
        return Fraction<U>(num, den);
    }

    // 乘法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator*(const Fraction<T>& other) const {
        T num = numerator * other.numerator;
        T den = denominator * other.denominator;
        return Fraction<U>(num, den);
    }

    // 除法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator/(const Fraction<T>& other) const {
        T num = numerator * other.denominator;
        T den = denominator * other.numerator;
        return Fraction<U>(num, den);
    }

    // 赋值加法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator+=(const Fraction<T>& other) {
        *this = *this + other;
        return *this;
    }

    // 赋值减法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator-=(const Fraction<T>& other) {
        *this = *this - other;
        return *this;
    }

    // 赋值乘法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator*=(const Fraction<T>& other) {
        *this = *this * other;
        return *this;
    }

    // 赋值除法运算符重载
    template<typename U = T>
    Fraction<std::enable_if_t<std::is_arithmetic_v<U>, U>> operator/=(const Fraction<T>& other) {
        *this = *this / other;
        return *this;
    }

    // 相等运算符重载
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    // 不等运算符重载
    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    // 小于运算符重载
    bool operator<(const Fraction& other) const {
        T num1 = numerator * other.denominator;
        T num2 = other.numerator * denominator;
        return num1 < num2;
    }

    // 小于等于运算符重载
    bool operator<=(const Fraction& other) const {
        return (*this < other) || (*this == other