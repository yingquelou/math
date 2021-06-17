#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#define __BETTER__
#define STR "abcabcac"
#define WIDTH 9
bool GetNext(unsigned length, char *arr, int *next);
int KMP(unsigned Lt, char *t, unsigned Ls, char *s, int *Next);
int main(void)
{
    char arr[] = "abaabcasdrac";
    int next[WIDTH], length = sizeof arr;
    GetNext(WIDTH, STR, next);
    int pos = KMP(WIDTH, STR, length - 1, arr, next);
    return 0;
}
//arr为模式串，length为模式串长度,next用于存放Next数组，
//注意须保证Next数组可存放的元素个数不小于模式串长度
bool GetNext(unsigned length, char *arr, int *next)
{
    //判断参数合法性，不合法返回false
    if ((length < 1) || !arr || !next)
        return false;
    int i = 1, j = 0;
    next[0] = -1;
    next[1] = 0;
    while (i < length)
    {
        if (j < 0 || arr[i] == arr[j])
        {
            ++i;
            ++j;
//不论__BETTER__是否定义，GetNext函数都能产生有效的Next数组，
//但定义__BETTER__后，GetNext函数产生的Next数组对KMP匹配来说更为高效
#ifdef __BETTER__
            if (arr[i] != arr[j])
#endif
                next[i] = j;
#ifdef __BETTER__
            else
                next[i] = next[j];
#endif
        }
        else
            j = next[j];
    }
    return true; //成功生成Nexts数组返回true
}
//t为模式串，Lt为模式串长度，s为主串，Ls为主串长度
int KMP(unsigned Lt, char *t, unsigned Ls, char *s, int *Next)
{
    //判断参数合法性，不合法返回-1
    if (!Lt || !Ls || (Lt > Ls) || !t || !s)
        return -1;
    int ti = 0, si = 0; //定义模式串和主串进行比较时的位置标志
    while (si < Ls && ti < Lt)
    {
        if (ti < 0 || t[ti] == s[si])
        {
            ti++;
            si++;
        }
        else
            ti = Next[ti];
    }
    if (ti >= Lt)
        return si - ti; //返回主串中第一次出现模式串的位置(以下标的方式返回)
    else
        return -2; //未找到返回-2
}