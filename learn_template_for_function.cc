/* 函数模板 */
#include <iostream>
// Ty是某种类型,作为模板的参数,简称类型模板参数
template <typename Ty>
// 与template <class Ty>等价
/* static∧(constexpr∨inline) */
Ty func1(Ty a)
{
    /* code */
}
// val是某种类型的对象,亦可作为模板的参数,简称非类型模板参数
template <int val>
/*  static∧(constexpr∨inline)
 *  模板也可以用某种类型的对象作为参数,但其必须是常量表达式
 */
int func2(int a)
{
    /* code */
}
int main(void)
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}