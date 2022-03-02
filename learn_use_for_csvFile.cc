#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
int main(void)
{
    std::ofstream of("class.csv", std::fstream::out);
    std::ostream_iterator<std::string> ofi(of);
    for (char i = '0'; i < '2'; ++i)
    {
        for (char j = '0'; j < '2'; ++j)
        {
            for (char k = '0'; k < '2'; ++k)
            {
                for (char l = '0'; l < '2'; ++l)
                    if ((j + k + l - 3 * '0') == 1)
                        ofi = std::string("") + i + ',' + j + ',' + k + ',' + l + '\n';
            }
        }
    }
    of.close();
    return 0;
}