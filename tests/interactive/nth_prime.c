#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/prime/prime.h"

int main()
{
    int n = 1;
    while (n)
    {
        printf("请输入n，求第n个素数，n为0时退出程序\n");
        printf("请输入n（输入0退出程序）：\n");
        scanf("%d", &n);

        if (n > 0)
            printf("第%d个素数是：%lld\n", n, nth_prime(n));
    }
    return 0;
}
