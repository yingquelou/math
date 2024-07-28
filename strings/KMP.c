#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "kmp.h"
/**
 * \brief 创建Next数组
 * \note 须保证Next数组可存放的元素个数不小于模式串长度
 * \param length 模式串长度
 * \param arr 模式串
 * \param next 用于存放Next数组
 * \return  反应是否成功生成Next数组
 * \date by yingquelou at 2024-06-16 07:17:20
 */
bool GetNext(unsigned length, const char *arr, int *next)
{
    // 判断参数合法性，不合法返回false
    if ((length < 2) || !arr || !next)
        return false;
    int i = 1, j = 0;
    next[0] = 0;
    while (i < length)
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
    return true; // 成功生成Nexts数组返回true
}
/**
 * \brief
 *
 * \param Lp 模式串长度
 * \param p 模式串
 * \param Ls 主串
 * \param s 主串长度
 * \param Next
 * \return
 * \date by yingquelou at 2024-06-16 09:18:22
 */
int KMP(unsigned Lp, const char p[], unsigned Ls, const char s[], const int Next[])
{
    // 判断参数合法性，不合法返回-1
    if (!Lp || !Ls || (Lp > Ls) || !p || !s)
        return -1;
    int ti = 0, si = 0; // 定义模式串和主串进行比较时的位置标志
    while (si < Ls && ti < Lp)
    {
        if (ti < 0 || p[ti] == s[si])
        {
            ti++;
            si++;
        }
        else
            ti = Next[ti];
    }
    return (ti >= Lp) ? si - ti : -1;
}
bool kmpInFile(FILE *fp, const char *pattern, const int *next)
{
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
        else
            i = next[i];
    }
    return -1;
}