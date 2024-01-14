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
void example(void)
{

    std::random_device rd;  // 将用于获得随机数引擎的种子
    std::mt19937 gen(rd()); // 以 rd() 播种的标准 mersenne_twister_engine
    std::uniform_real_distribution<> dis(-1, 8.9);
    for (int n = 0; n < 10; ++n)
    {
        // 用 dis 变换 gen 生成的随机 unsigned int 为 [1, 2) 中的 double
        std::cout << dis(gen) << ' '; // 每次调用 dis(gen) 都生成新的随机 double
    }
}