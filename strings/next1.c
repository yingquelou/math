//#define __DEBUG__
#include <string.h>
void GetNext(char T[], int next[]);
#include <stdio.h>
int main(void)
{
    char *arr = "safsgfasawwq";
    int length = strlen(arr),
        next[12];
    GetNext(arr, next);
    return 0;
}
void GetNext(char *p, int next[])
{
    int pLen = strlen(p);
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < pLen - 1)
    {
        //p[k]表示前缀，p[j]表示后缀
        if (k == -1 || p[j] == p[k])
        {
            ++k;
            ++j;
            next[j] = k;
        }
        else
        {
            k = next[k];
        }
    }
}