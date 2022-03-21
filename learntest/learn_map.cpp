#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;
int main(void)
{
    map<string, size_t> words_count;
    words_count.insert({{"adas", 2}, {"wqeqw", 3}, {"adqw", 3}});
    multimap<string, int> mm(words_count.cbegin(), words_count.cend());
    map<string, vector<string>> flist;
    string firstname, lastname;
    while (cin >> lastname >> firstname)
    {
        flist[lastname].push_back(firstname);
    }
    return 0;
}