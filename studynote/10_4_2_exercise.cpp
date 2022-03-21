#include <fstream>
#include <iostream>
#include <ctime>
#include <iterator>
#include <algorithm>
using namespace std;
int main(void)
{
    srand((unsigned)time(nullptr));
    ofstream file1("1.txt"), file2;
    for (unsigned i = 0; i < 100; ++i)
        file1 << (rand() % 100) << ' ';
    file1.close();
    ifstream file0("1.txt");
    file2.open("2.txt");
    file1.open("3.txt");
    ostream_iterator<int> is2(file2, " "), is3(file1, "\n");
    istream_iterator<int> is1(file0), f_eof;
    /* auto m = is1;
    copy_if(is1, f_eof, is2, [](int i){ return i % 2; });
    is1 = m;
    copy_if(is1, f_eof, is3, [](int i){ return !(i % 2); }); */
    while (is1 != f_eof)
    {
        if (*is1 % 2)
            *is2 = *is1;
        else
            *is3 = *is1;
        ++is1;
    }
    file1.close();
    file2.close();
    cout << "Nice to meet you!" << endl;
    return 0;
}