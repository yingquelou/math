#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(void)
{

    return 0;
}
int kmp(char t[], char s[], int next[])
{
    int i = 0, j = 0,
        Lt = strlen(t),
        Ls = strlen(s);
    if (Lt > Ls)
        return -1;
    while (i <= Ls && j <= Lt)
    {
        if (j == 0 || t[i] == s[j])
        {
            ++i;
            if (j > 0)
                ++j;
        }
        else
            j = next[j];
    }
    if (j > Lt)
        return i - j - 1;
    return -1;
}