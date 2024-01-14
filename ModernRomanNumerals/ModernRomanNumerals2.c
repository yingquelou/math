// Modern Roman numerals
//  I   V   X   L   C   D   M
//  1   5   10  50  100 500 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char *toRomanNum(int num);
int main(void)
{
    FILE *pf;
    if (pf = fopen("log.txt", "w"))
    {
        char *str;
        for (int i = 1; i < 4000; ++i)
        {
            str = toRomanNum(i);
            fprintf(pf, "%4d:%s\n", i, str);
            free(str);
            str = NULL;
        }
        fclose(pf);
    }
    else
        exit(EXIT_FAILURE);
    return 0;
}
static char *fun(int *pnum, int *pWidth, char *tmp,
                 int Condition,
                 char *s1, char *s2, char *s3,
                 char *s4, char *s5)
{
    int num = *pnum, Width = *pWidth, addWidth;
    if (num >= 9 * Condition)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, s1);
        num -= 9 * Condition;
    }
    else if (num >= 5 * Condition)
    {
        Width += 1;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, s2);
        num -= 5 * Condition;

        addWidth = num / Condition;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, s3);
            }
            num -= addWidth * Condition;
        }
    }
    else if (num >= 4 * Condition)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, s4);
        num -= 4 * Condition;
    }
    else
    {
        addWidth = num / Condition;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, s5);
            }
            num -= addWidth * Condition;
        }
    }
    *pnum = num;
    *pWidth = Width;
    return tmp;
}
static char *toRomanNum(int num)
{
    if (num < 1 || num > 3999)
    {
        fprintf(stderr, "Out of representable range!");
        return NULL;
    }
    char *tmp = (char *)malloc(sizeof(char));
    *tmp = '\0';
    int Width = 1, addWidth;
    if (num >= 1000)
    {
        addWidth = num / 1000;
        Width += addWidth;
        tmp = (char *)realloc(tmp, Width);
        for (size_t i = 0; i < addWidth; ++i)
        {
            strcat(tmp, "M");
        }
        num %= 1000;
    }
    tmp = fun(&num, &Width, tmp, 100, "CM", "D", "C", "CD", "C");
    tmp = fun(&num, &Width, tmp, 10, "XC", "L", "X", "XL", "X");
    tmp = fun(&num, &Width, tmp, 1, "IX", "V", "I", "IV", "I");
    return tmp;
}