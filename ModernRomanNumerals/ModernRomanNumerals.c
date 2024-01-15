// Modern Roman numerals
//  I   V   X   L   C   D   M
//  1   5   10  50  100 500 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *toRomanNum(int num);
int main(void)
{
    char *str;
    for (size_t i = 1; i <= 100; ++i)
    {
        str = toRomanNum(i);
        printf("%ld:%s\n", i, str);
        free(str);
    }
    return 0;
}
char *toRomanNum(int num)
{
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
    if (num >= 900)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "CM");
        num -= 900;
    }
    else if (num >= 500)
    {
        Width += 1;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "D");
        num -= 500;

        addWidth = num / 100;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "C");
            }
            num -= addWidth * 100;
        }
    }
    else if (num >= 400)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "CD");
        num -= 400;
    }
    else
    {
        addWidth = num / 100;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "C");
            }
            num -= addWidth * 100;
        }
    }
    if (num >= 90)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "XC");
        num -= 90;
    }
    else if (num >= 50)
    {
        Width += 1;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "L");
        num -= 50;

        addWidth = num / 10;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "X");
            }
            num -= addWidth * 10;
        }
    }
    else if (num >= 40)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "XL");
        num -= 40;
    }
    else
    {
        addWidth = num / 10;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "X");
            }
            num -= addWidth * 10;
        }
    }
    if (num >= 9)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "IX");
        num -= 9;
    }
    else if (num >= 5)
    {
        Width += 1;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "V");
        num -= 5;

        addWidth = num / 1;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "I");
            }
            num -= addWidth * 1;
        }
    }
    else if (num >= 4)
    {
        Width += 2;
        tmp = (char *)realloc(tmp, Width);
        strcat(tmp, "IV");
        num -= 4;
    }
    else
    {
        addWidth = num / 1;
        if (addWidth > 0)
        {
            Width += addWidth;
            tmp = (char *)realloc(tmp, Width);
            for (size_t i = 0; i < addWidth; ++i)
            {
                strcat(tmp, "I");
            }
            num -= addWidth * 1;
        }
    }
    return tmp;
}