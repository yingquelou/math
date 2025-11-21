typedef struct
{
    int length;
    char *ch;
} SString;
void get_nextval(SString T, int nextval[])
{ //求模式串T的next函数修正值并存入数组nextval
    int i = 1, j = 0;
    nextval[1] = 0;
    while (i < T.length)
    {
        if (j == 0 || T.ch[i] == T.ch[j])
        {
            ++i;
            ++j;
            if (T.ch[i] != T.ch[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];
        }
        else
            j = nextval[j];
    }
}