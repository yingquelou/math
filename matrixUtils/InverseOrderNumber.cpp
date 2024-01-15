#include <iostream>
#include <string>
auto &os = std::cout;
/**
 * \brief 求解排列的逆序数
 * 
 * \param str 排列
 * \return  
 * \date by yingquelou at 2023-04-30 14:15:47
 */
size_t InverseOrderNumber(const char str[])
{
    std::string s(str);
    size_t pos = 0;
    auto &&ret = std::stoull(s, &pos);
    auto &&len = s.length();
    if (pos == len)
    {
        pos = 0;
        for (size_t i = 0; i < len - 1; i++)
            for (size_t j = i + 1; j < len; j++)
                if (s[j] < s[i])
                    ++pos;
        return pos;
    }
    else
        return UINT64_MAX;
}
int main(int argc, char *argv[])
{
    if (argc > 1)
        for (size_t i = 1; i < argc; i++)
        {
            auto &&num = InverseOrderNumber(argv[i]);
            if (num == UINT64_MAX)
                os << argv[i] << "\tIt isn't a number!\n";
            else
                os << argv[i] << '\t' << num << '\n';
        }
    return 0;
}