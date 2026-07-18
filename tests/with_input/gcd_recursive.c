#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "../../src/gcd/gcd.h"

int main()
{
    int n1, n2;
    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);

    printf("%d 和 %d 的最大公约数为 %d", n1, n2, gcd_recursive(n1, n2));
    return 0;
}