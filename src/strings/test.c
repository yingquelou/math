#include <stdio.h>
int main(int argc, char const *argv[])
{
    for (int i = 1; argv[i]; ++i)
    {
        puts(argv[i]);
    }
    return 0;
}
