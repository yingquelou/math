#include <stdlib.h>
#include <string.h>
#include "kmp.h"

bool GetNext(unsigned length, const char *arr, int *next)
{
    if ((length < 2) || !arr || !next)
        return false;
    int i = 1, j = 0;
    next[0] = 0;
    while (i < (int)length)
    {
        if (arr[i] == arr[j])
        {
            next[i] = j + 1;
            i++;
            j++;
        }
        else if (j == 0)
        {
            next[i] = 0;
            i++;
        }
        else
            j = next[j - 1];
    }
    return true;
}

int KMP(unsigned Lp, const char p[], unsigned Ls, const char s[], const int Next[])
{
    if (!Lp || !Ls || (Lp > Ls) || !p || !s)
        return -1;
    int ti = 0, si = 0;
    while (si < (int)Ls && ti < (int)Lp)
    {
        if (ti == 0 || p[ti] == s[si])
        {
            ti++;
            si++;
        }
        else
            ti = Next[ti - 1];
    }
    return (ti >= (int)Lp) ? si - ti : -1;
}

bool kmpInFile(FILE *fp, const char *pattern, const int *next)
{
    if (!fp || !pattern || !next)
        return false;
    char ch;
    int i = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == pattern[i])
        {
            i++;
            if (pattern[i] == '\0')
            {
                fseek(fp, -strlen(pattern), SEEK_CUR);
                return true;
            }
        }
        else if (i > 0)
        {
            i = next[i - 1];
        }
    }
    return false;
}
