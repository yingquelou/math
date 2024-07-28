#include <stdio.h>
#include <string.h>
#include <assert.h>
int Sunday(char *s, int Ls, char *t, int Lt);
int main(void)
{
    char source1[] = "Hello, world!";
    char pattern1[] = "world";
    assert(Sunday(source1, strlen(source1), pattern1, strlen(pattern1)) == 7);

    char source2[] = "GitHub Copilot";
    char pattern2[] = "Copilot";
    assert(Sunday(source2, strlen(source2), pattern2, strlen(pattern2)) == 7);

    char source3[] = "abcabcabc";
    char pattern3[] = "abc";
    assert(Sunday(source3, strlen(source3), pattern3, strlen(pattern3)) == 0);

    char source4[] = "abcabcabc";
    char pattern4[] = "def";
    assert(Sunday(source4, strlen(source4), pattern4, strlen(pattern4)) == -1);

    return 0;
}
/**
 * 使用Sunday算法在源字符串中搜索模式字符串的第一个匹配位置。
 *
 * @param s 源字符串。
 * @param Ls 源字符串的长度。
 * @param p 模式字符串。
 * @param Lp 模式字符串的长度。
 * @return 模式字符串在源字符串中的第一个匹配位置的索引，如果未找到则返回-1。
 */
int Sunday(char *s, int Ls, char *p, int Lp)
{
    int si = 0, pi = 0;
    if (Ls <= 0 || Lp <= 0 || Ls < Lp)
        return -1;
    while (si < Ls && pi < Lp)
    {
        if (s[si] == p[pi])
        {
            ++si;
            ++pi;
        }
        else
        {
            int i = Lp - 1;
            while (i >= 0 && p[i] != s[si])
                --i;
            si += Lp - i;
            pi = 0;
        }
    }
    return pi == Lp ? si - Lp : -1;
}