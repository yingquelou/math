#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
class name
{
private:
    std::multimap<std::wstring, std::wstring,std::greater<std::wstring>> table;

public:
    name();
    ~name();
};