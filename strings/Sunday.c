#include <stdio.h>
#include <string.h>
int Sunday(char *s, int Ls, char *t, int Lt);
int main(void)
{ //             11100100 10111101 10100000
    //unicode 你 100111101100000 (4f60)
    char t[] = u8"你",
         s[] = u8"你好";
    printf("%d\n", Sunday(s, strlen(s), t, strlen(t)));
    return 0;
}
int Sunday(char *s, int Ls, char *t, int Lt)
{
    int si = 0, ti = 0;
    if (Ls <= 0 || Lt <= 0 || Ls < Lt)
        return -1;
    while (si < Ls)
    {
        while (ti < Lt && t[ti] == s[si])
        {
            ti++;
            si++;
        }
        if (ti == Lt)
            return si - ti;
        else
        {
            si += Lt - ti;
            ti = 0;
            while (ti < Lt && s[si] != t[ti])
                ti++;
            if (ti == Lt)
            {
                si++;
                ti = 0;
            }
            else
            {
                ti++;
                si++;
            }
        }
    }
    if (si >= Ls)
        return -1;
}