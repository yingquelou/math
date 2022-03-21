#include "name.h"
name::name()
{
    std::ifstream ifs("name.csv");
    std::istream_iterator<std::string> ii(ifs), fend;
    for (; ii != fend; ++ii)
    {
        auto pos =ii->find(",",0);
        std::string left(*ii, 0,pos), right(*ii, pos + 1);
        table.emplace(left, right);
    }
}
name::~name()
{
}