#include "determinant.h"
#include <stdio.h>

Dtype determinant_2_3(int Order, Dtype a[Order][Order])
{
    if (Order < 2)
    {
        puts("Order is too small!");
        return 0;
    }
    if (Order == 2)
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    if (Order > 3)
    {
        puts("Order is too big!");
        return 0;
    }
    Dtype Sum = 0, Product = 1;
    int i = 0, j = 0, k = 0;
    // 主对角线系
    for (; k < Order; ++k)
    {
        for (i = 0, j = k, Product = 1; i < Order; ++i, ++j)
        {
            j %= Order;
            Product *= a[i][j];
        }
        Sum += Product;
    }
    // 副对角线系
    for (k = 0; k < Order; ++k)
    {
        for (i = 0, j = k, Product = 1; i < Order; ++i, --j)
        {
            j = (j + Order) % Order;
            Product *= a[i][j];
        }
        Sum -= Product;
    }
    return Sum;
}

Dtype n_determinant(int Order, Dtype a[Order][Order])
{
    int i = 0, j = 0, l = 0, p = 0;
    Dtype k = 0;
    int sign = 1;
    
    for (; j < Order - 1; ++j)
    {
        p = j;
        for (i = j + 1; i < Order; ++i)
        {
            if (a[i][j] != 0)
            {
                p = i;
                break;
            }
        }
        
        if (a[p][j] == 0)
            return 0;
        
        if (p != j)
        {
            for (l = j; l < Order; ++l)
            {
                Dtype temp = a[j][l];
                a[j][l] = a[p][l];
                a[p][l] = temp;
            }
            sign = -sign;
        }
        
        for (i = j + 1; i < Order; ++i)
        {
            k = -a[i][j] / a[j][j];
            if (k)
            {
                a[i][j] = 0;
                for (l = j + 1; l < Order; ++l)
                {
                    a[i][l] += k * a[j][l];
                }
            }
        }
    }
    Dtype Product = sign;
    for (l = 0; l < Order; ++l)
    {
        Product *= a[l][l];
    }
    return Product;
}

