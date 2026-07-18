#include "prime.h"
#include <stdio.h>

int is_prime(int n)
{
    if (n < 2)
        return 0;
    
    int i = 0;
    for (i = 2; i < n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int checkPrimeNumber(int n)
{
    int j, flag = 1;

    for (j = 2; j <= n / 2; ++j)
    {
        if (n % j == 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

long long nth_prime(int n)
{
    if (n <= 0)
        return 0;
    
    int i = 2, count = 0;
    for (; count < n; ++i)
    {
        int j = 2, flag = 1;
        for (; j * j <= i; j++)
        {
            if (i % j == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1)
            count++;
    }
    return i - 1;
}

void find_primes_in_range(int n, int m)
{
    for (; n < m; n++)
    {
        int i = 1;
        int c = 0;
        for (i = 2; i < n; i++)
        {
            if ((n % i) == 0)
                c++;
        }
        if (c == 0)
            printf("%d\n", n);
    }
}
