#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/prime/prime.h"

int main()
{
    int n1, n2, i, flag;

    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);
    printf("%d 和 %d 之间的素数: ", n1, n2);

    for (i = n1 + 1; i < n2; ++i)
    {
        // 判断是否为素数
        flag = checkPrimeNumber(i);

        if (flag == 1)
            printf("%d ", i);
    }
    return 0;
}