#include "gcd.h"

int gcd_recursive(int n1, int n2)
{
    if (n2 != 0)
        return gcd_recursive(n2, n1 % n2);
    else
        return n1;
}

int gcd_subtraction(int n1, int n2)
{
    while (n1 != n2)
    {
        if (n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    return n2;
}

int gcd_loop(int a, int b)
{
    if (a > 0 && b > 0)
    {
        int i = a < b ? a : b;
        for (; i > 0; i--)
        {
            if (a % i == 0 && b % i == 0)
                break;
        }
        return i;
    }
    else
        return 0;
}