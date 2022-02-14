#include <memory>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main(void)
{
    shared_ptr<char> sp(new char('a'));
    unique_ptr<string> up(new string("abc"));
    sp = make_shared<char>('q');
    weak_ptr<char> wp(sp);
    auto up2 = wp.lock();
    return 0;
}