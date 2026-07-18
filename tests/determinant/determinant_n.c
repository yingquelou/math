#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../src/determinant/determinant.h"

#define n 80
#define y 0

int main(void)
{
    srand((unsigned)time(NULL));
    Dtype a[n][n];
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            a[i][j] = rand() % 100 + 1;
            if (j != n - 1)
                printf("%.*lf,", y, a[i][j]);
            else
                printf("%.*lf", y, a[i][j]);
        }
        puts("");
    }
    puts("");
    for (size_t i = 0; i < n; ++i)
    {
        printf("[");
        for (size_t j = 0; j < n; ++j)
        {
            if (j != n - 1)
                printf("%.*lf,", y, a[i][j]);
            else
                printf("%.*lf", y, a[i][j]);
        }
        printf("]");
        if (i != n - 1)
            puts(",");
    }
    puts("");
    Dtype tmp = n_determinant(n, a);
    printf("%e\n", tmp);
    return 0;
}

