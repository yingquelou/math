#define _CRT_SECURE_NO_WARNINGS 1
#include "../../src/combinatorics/combinatorics.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int x = 24;
    do
    {
    angine:
        scanf("%d", &x);
        if (x > 0 && x <= 12)
        {
            printf("可能性为%lld\n", combination_sum(x));
        }
        else
        {
            printf("输入超出可计算范围（1到12），请重新输入：\n");
            goto angine;
        }
    } while (x > 0 && x <= 12);
    system("pause");
    return 0;
}
