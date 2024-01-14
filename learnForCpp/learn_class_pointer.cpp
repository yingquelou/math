#include <iostream>
enum
{
    zero,
    one,
    two,
    three
};
class INT
{
    friend

        std::ostream &
        operator<<(std::ostream &, const INT &);

private:
    int i;

public:
    INT() : i(0) {}
    // INT(const INT &t) : i(i) {}
    INT(const int &t) : i(i) {}
    ~INT() {}

public:
    INT operator+(const INT &j) const { return i + j.i; }
    INT operator-(const INT &j) const { return i - j.i; }
    INT operator*(const INT &j) const { return i * j.i; }
    INT operator/(const INT &j) const { return i / j.i; }

public:
    using list = INT(const INT &j) const;
};
std::ostream &operator<<(std::ostream &out, const INT &A)
{
    out << A.i;
    return out;
}
int main(void)
{
    INT::list INT::*li[] = {
        &INT::operator+,
        &INT::operator-,
        &INT::operator*,
        &INT::operator/ };
    INT y;
    std::cout << (y.*li[0])(8) << std::endl;
    return 0;
}