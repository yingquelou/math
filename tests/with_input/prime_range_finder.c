#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/prime/prime.h"

int main()
{
    int n = 1, m = 1000;
    scanf("%d %d", &n, &m);
    find_primes_in_range(n, m);
    return 0;
}