#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;
int main(void)
{
    set<string> sign;
    vector<string> result;
    string tmp;
    while (cin >> tmp)
    {
        if (sign.find(tmp) == sign.cend())
        {
            sign.insert(tmp);
            result.push_back(tmp);
        }
    }
    for (auto &i : result)
        cout << i << endl;
    return 0;
}