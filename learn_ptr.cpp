#include <memory>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main(void)
{
    shared_ptr<char> sp(new char('a'));
    shared_ptr<char> sp2(sp);
    unique_ptr<string> up(new string("abc"));
    sp = make_shared<char>('q');
    weak_ptr<char> wp(sp);
    auto up2 = wp.lock();
    auto tf = sp2.use_count();
    allocator<char> pc;
    auto pch = pc.allocate(3);
    return 0;
}