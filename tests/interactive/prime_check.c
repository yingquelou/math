#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/prime/prime.h"

int main()
{
    // 测试字符数组
    char arr[] = "a";
    printf("%d\n", arr[0]);

    do
    {
        long long k = 0;
        scanf("%lld", &k);
        switch (is_prime(k))
        {
        case 1:
            printf("%lld是质数\n", k);
            break;
        case 0:
            printf("%lld不是质数\n", k);
            break;
        }
    } while (1);
}
