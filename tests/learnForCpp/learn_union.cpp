#include <iostream>
union learn_union
{
public:
    char c;
    short s;
    int i;
    long l;
    long long ll;
};
int main(void)
{
    learn_union tmp;
    tmp.c = 1;
    std::cout << tmp.ll << std::endl;
    return 0;
}