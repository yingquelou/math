#include <stdio.h>
#include <limits.h>

#define IntTypeMax(X) _Generic((X),     \
    char: CHAR_MAX,                     \
    unsigned char: UCHAR_MAX,           \
    short: SHRT_MAX,                    \
    unsigned short: USHRT_MAX,          \
    int: INT_MAX,                       \
    unsigned: UINT_MAX,                 \
    long: LONG_MAX,                     \
    unsigned long: ULONG_MAX,           \
    long long: LONG_LONG_MAX,           \
    unsigned long long: ULONG_LONG_MAX, \
    default: 0)
#define token(X) _Generic((X), \
    char: "%d",                \
    unsigned char: "%u",       \
    short: "%d",               \
    unsigned short: "%u",      \
    int: "%d",                 \
    unsigned: "%u",            \
    long: "%ld",               \
    unsigned long: "%u",       \
    long long: "%lld",         \
    unsigned long long: "%u",  \
    default: "%#x")
int main(int argc, char *argv[])
{
    // char num = 10;
    // unsigned char num = 10;
    // short num = 10;
    // unsigned short num = 10;
    // int num = 10;
    // unsigned num = 10;
    // long num = 10;
    // unsigned long num = 10;
    // long long num = 10;
    unsigned long long num = 10;
    printf("%x", IntTypeMax(num));
    return 0;
}