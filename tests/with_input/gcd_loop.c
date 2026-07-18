#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/gcd/gcd.h"

int main()
{
    int a, b;
    printf("输入两个正整数：");
    scanf("%d %d", &a, &b);
    int c = gcd_loop(a, b);
    if (c != 0)
        printf("%d和%d的最大公约数是%d", a, b, c);
    return 0;
}