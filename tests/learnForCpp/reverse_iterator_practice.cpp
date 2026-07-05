#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main(void) {
  vector<char> strL{'a', 's', 'd', 'd', 'f', 'g'};
  for (auto first = strL.crbegin(); first != strL.crend(); ++first)
    cout << *first;
  return 0;
}