#include <iostream>
union learn_union
{
    learn_union();
    ~learn_union();

public:
    char c;
    short s;
    int i;
    std::iostream a;

    long l;
    long long ll;

public:
    bool add();
};
int main(void)
{
    learn_union tmp;
    tmp.c = 1;
    std::cout << tmp.ll << std::endl;
    return 0;
}