#include <iostream>
#include <random>
int main(void)
{
    std::default_random_engine e;
    std::uniform_int_distribution<> a(1, 3);
    std::cout << a(e) << std::endl;
    std::uniform_real_distribution<> b(1, 3);
    std::cout << b(e) << std::endl;
    std::normal_distribution<> c(1, 3);
    std::cout << c(e) << std::endl;
    return 0;
}