#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include <cstdlib>
#include <cstring>
using std::string;
#include <initializer_list>
#include <stdexcept>
using std::error_code;
using std::initializer_list;
int Sum(const initializer_list<int> &list);
int main()
{
    cout << Sum({1, 3, 2, 100}) << endl;
    return 0;
}
int Sum(const initializer_list<int> &list)
{
    int Sum = 0;
    for (const auto &x : list)
        Sum += x;
    return Sum;
}
using zu = string (&)[10];
typedef string (&zu2)[10];
zu fun();
zu2 fun();
string (&fun())[10];