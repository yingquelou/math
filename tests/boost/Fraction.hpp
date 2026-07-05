#ifndef FRACTION_H
#define FRACTION_H 1
#include <boost/operators.hpp>
#include <cstdlib>
#include <iostream>

template <typename T>
inline T gcd(T a, T b) {
  a = std::abs(a);
  b = std::abs(b);
  while (b != 0) {
    T t = b;
    b = a % b;
    a = t;
  }
  return a;
}

template <typename T>
class Fraction : boost::addable<Fraction<T>>,
                 boost::addable<Fraction<T>, T>,
                 boost::subtractable<Fraction<T>>,
                 boost::subtractable<Fraction<T>, T>,
                 boost::subtractable2_left<Fraction<T>, T>,
                 boost::multipliable<Fraction<T>>,
                 boost::multipliable<Fraction<T>, T>,
                 boost::dividable<Fraction<T>>,
                 boost::dividable<Fraction<T>, T>,
                 boost::dividable2_left<Fraction<T>, T> {
  static_assert(std::is_integral<T>::value, "Fraction requires integral type");

 public:
  Fraction() : numerator(0), denominator(1) {}

  explicit Fraction(T n, T d = 1) : numerator(n), denominator(d) { reduce(); }

  Fraction(const Fraction &other)
      : numerator(other.numerator), denominator(other.denominator) {}

  Fraction &operator+=(const Fraction &other) {
    numerator = numerator * other.denominator + denominator * other.numerator;
    denominator = denominator * other.denominator;
    reduce();
    return *this;
  }

  Fraction &operator+=(const T &other) { return *this += Fraction(other); }

  Fraction &operator-=(const Fraction &other) {
    numerator = numerator * other.denominator - denominator * other.numerator;
    denominator = denominator * other.denominator;
    reduce();
    return *this;
  }
  Fraction &operator-=(const T &other) { return *this -= Fraction(other); }
  Fraction &operator*=(const Fraction &other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }
  Fraction &operator*=(const T &other) { return *this *= Fraction(other); }

  Fraction &operator/=(const Fraction &other) {
    numerator *= other.denominator;
    denominator *= other.numerator;
    reduce();
    return *this;
  }
  Fraction &operator/=(const T &other) { return *this /= Fraction(other); }
  operator double() { return numerator * 1.0 / denominator; }
  template <typename U>
  friend std::ostream &operator<<(std::ostream &_os_, const Fraction<U> &f) {
    return _os_ << f.numerator << '/' << f.denominator;
  }

 private:
  T numerator;
  T denominator;

  void reduce() {
    T g = gcd(denominator, numerator);
    numerator /= g;
    denominator /= g;
    if (numerator * denominator >= 0) {
      numerator = std::abs(numerator);
    } else {
      numerator = -std::abs(numerator);
    }
    denominator = std::abs(denominator);
  }
};
#endif