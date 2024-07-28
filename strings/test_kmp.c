#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp.h"
void testKMP()
{
    char *pattern = "abc";
    char *text = "ababcabcabcabc";
    int next[3];

    assert(GetNext(3, pattern, next) == true);
    assert(KMP(3, pattern, 14, text, next) == 2);

    char *pattern2 = "abcd";
    char *text2 = "ababcabcabcabc";
    int next2[4];

    assert(GetNext(4, pattern2, next2) == true);
    assert(KMP(4, pattern2, 14, text2, next2) == -1);

    char *pattern3 = "abc";
    char *text3 = "abcabcabcabcabc";
    int next3[3];

    assert(GetNext(3, pattern3, next3) == true);
    assert(KMP(3, pattern3, 15, text3, next3) == 0);

    printf("All tests passed!\n");
}
void testKMPInFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }
    fseek(fp, -1000, SEEK_END);
    char *pattern = "startxref";
    int *next = calloc(strlen(pattern), sizeof(int));
    char buffer[1024];
    if (GetNext(3, pattern, next) && kmpInFile(fp, pattern, next))
    {
        fgets(buffer,1024, fp);
        puts(buffer);
        fgets(buffer,1024, fp);
        puts(buffer);
    }
    free((void *)next);
    fclose(fp);
    printf("All tests passed!\n");
}
int main(int argc, char const *argv[])
{
    puts(argv[1]);
    testKMPInFile(argv[1]);
    // char *pattern = "aabaaf";
    // //
    // size_t ln = strlen(pattern);
    // int *next = calloc(ln, sizeof(int));
    // if (next != NULL)
    // {
    //     GetNext(ln, pattern, next);
    //     for (int i = 0; i < ln; i++)
    //     {
    //         printf("%d ", next[i]);
    //     }
    //     free((void*)next);
    // }
    return 0;
}