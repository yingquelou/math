#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../src/determinant/determinant.h"

#define n 4

int main(void)
{
    srand((unsigned)time(NULL));
    Dtype a[n][n];
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            a[i][j] = rand() % 10;
            if (j != n - 1)
                printf("%.0lf,", a[i][j]);
            else
                printf("%.0lf", a[i][j]);
        }
        puts("");
    }
    Dtype tmp = determinant_2_3(n, a);
    printf("%.0lf\n", tmp);
    return 0;
}

