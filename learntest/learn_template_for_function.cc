/* 函数模板 */
#include <iostream>
#include <string>
// Ty是某种类型,作为模板的参数,简称类型模板参数
template <typename T>
// 与template <class T>等价
/* static∧(constexpr∨inline) */
T ADD(T const &a, T const &b)
{
    return a + b;
}
// val是某种类型的对象(引用),亦可作为模板的参数,简称非类型模板参数
template <const int val>
/*  static∧(constexpr∨inline)
 *  模板也可以用某种类型的对象作为参数,但其必须是常量表达式
 */
int func2()
{
    return val;
}
using std::string;
template <typename T>
void print(std::ostream &out,const T&s)
{
    out << s;
}
template <typename T,typename... Ty>
void print(std::ostream &out,const T&s,const Ty&... Arg)
{
    out << s;
    print(out, Arg...);
}
int main(void)
{
    print(std::cout, "asfa", 12, "qw", 'd',string("adfas"));
    // std::cout << ADD(string("adsas"), string("asas"));
    return 0;
}