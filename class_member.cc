#include <iostream>
#include <string>
template <typename T>
T ADD(T const &a, T const &b) { return a + b; }
int main(void)
{
    std::cout << ADD(std::string("ads"), std::string("fas"));
    return 0;
}