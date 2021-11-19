#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
#include <vector>
using std::vector;
static int Serch(const vector<string> &s, const string &t);
//统计输入的一系列串中不同串的数量
int main(void)
{
    //定义串容器
    vector<string> s;
    //定义计数容器
    vector<unsigned> count;
    string t;
    int pos;
    while (cin >> t)
    {
        if ((pos = Serch(s, t)) < 0)
        {
            if (!t.empty())
            {
                s.push_back(t);
                count.push_back(1);
            }
            else
            {
                cerr << "The t is empty.it's not decent!" << endl;
                return 0;
            }
        }
        else
            ++count[pos];
    }
    for (size_t i = 0; i < s.size(); ++i)
        cout << s[i] << " is " << count[i] << "." << endl;
    return 0;
}
//在串容器中查找某串,找到了返回相应位置(以下标形式),
//空容器或客串不用找,返回-1;找不到返回-2
static int Serch(const vector<string> &s, const string &t)
{
    if (s.empty() || t.empty())
        return -1; //空容器或客串不用找,返回-1
    int pos = 0;
    for (const auto &i : s)
        if (i != t)
            ++pos;
        else
            return pos;
    return -2;//找不到,返回-2
}