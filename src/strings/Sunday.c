#include "sunday.h"

int Sunday(const char *s, int Ls, const char *p, int Lp)
{
    if (Ls <= 0 || Lp <= 0 || Ls < Lp)
        return -1;
    for (int si = 0; si <= Ls - Lp; ++si)
    {
        int pi = 0;
        while (pi < Lp && s[si + pi] == p[pi])
            ++pi;
        if (pi == Lp)
            return si;
    }
    return -1;
}
