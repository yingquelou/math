#ifndef FRACTION_H
#define FRACTION_H 1
#include <concepts>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
class Fraction {
public:
    // 构造函数
    Fraction() : numerator(0), denominator(1) {}
    Fraction(T n, T d = 1);
    
    // 拷贝构造函数
    Fraction(const Fraction& other) : numerator(other.numerator), denominator(other.denominator) {}

    // 运算符重载
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;

    template <Arithmetic U>
    friend Fraction<U> operator+(const U& lhs, const Fraction<U>& rhs);
    template <Arithmetic U>
    friend Fraction<U> operator-(const U& lhs, const Fraction<U>& rhs);
    template <Arithmetic U>
    friend Fraction<U> operator*(const U& lhs, const Fraction<U>& rhs);
    template <Arithmetic U>
    friend Fraction<U> operator/(const U& lhs, const Fraction<U>& rhs);

template<typename U> 
friend std::ostream&operator<<(std::ostream&_os_,const Fraction<U> &f){
   return _os_<<f.numerator<<'/'<<f.denominator;
}
private:
    // 成员变量
    T numerator;       // 分子
    T denominator;     // 分母

    // 减小分数，使分子和分母互质
    void reduce();
};

template<Arithmetic T> 
Fraction<T>::Fraction(T n, T d){
     numerator=n;denominator=d; 
        reduce();
    }

// 运算符重载
template <Arithmetic T>
Fraction<T> Fraction<T>::operator+(const Fraction<T>& other) const {
    T n = numerator * other.denominator + denominator * other.numerator;
    T d = denominator * other.denominator;
    return Fraction(n, d);
}

template <Arithmetic T>
Fraction<T> Fraction<T>::operator-(const Fraction<T>& other) const {
    T n = numerator * other.denominator - denominator * other.numerator;
    T d = denominator * other.denominator;
    return Fraction(n, d);
}

template <Arithmetic T>
Fraction<T> Fraction<T>::operator*(const Fraction<T>& other) const {
    T n = numerator * other.numerator;
    T d = denominator * other.denominator;
    return Fraction(n, d);
}

template <Arithmetic T>
Fraction<T> Fraction<T>::operator/(const Fraction<T>& other) const {
    T n = numerator * other.denominator;
    T d = denominator * other.numerator;
    return Fraction(n, d);
}

// 与基本算术类型进行混合运算
template <Arithmetic T>
Fraction<T> operator+(const T& lhs, const Fraction<T>& rhs) {
    return rhs + lhs;
}

template <Arithmetic T>
Fraction<T> operator-(const T& lhs, const Fraction<T>& rhs) {
    return -rhs + lhs;
}

template <Arithmetic T>
Fraction<T> operator*(const T& lhs, const Fraction<T>& rhs) {
    return rhs * lhs;
}

template <Arithmetic T>
Fraction<T> operator/(const T& lhs, const Fraction<T>& rhs) {
    T n = lhs * rhs.denominator;
    T d = rhs.numerator;
    return Fraction<T>(n, d);
}
#include<numeric>
template<Arithmetic T> void Fraction<T>::reduce(){
    if (std::is_integral_v<T>){
        auto&&g= std::gcd(denominator,numerator);
        numerator/=g;
        denominator/=g;
        }
    if (denominator*numerator<0)
        numerator=-std::abs(numerator);
        else
        numerator=std::abs(numerator);
        denominator=std::abs(denominator);
}

#endif
