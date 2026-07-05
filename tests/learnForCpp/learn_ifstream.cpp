#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;
int main(void) {
  ifstream ifs("1.cpp", fstream::out);
  istream_iterator<string> is(ifs), f_eof;
  vector<string> str(is, f_eof);
  ifs.close();
  ofstream ofs("test.cpp", fstream::out);
  ostream_iterator<string> os(ofs);
  for_each(str.begin(), str.end(), [&os](string &s) { os = s; });
  return 0;
}