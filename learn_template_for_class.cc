/* 类模板 */
#include <iostream>
#include <algorithm>
template <typename T>
class learn_template_for_class
{
private:
    T /* data */;

public:
    learn_template_for_class(T /* args */) = delete;
};

int main(void)
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}