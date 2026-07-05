#ifndef STRINGS_KMP_H
#define STRINGS_KMP_H

#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

bool GetNext(unsigned length, const char *arr, int *next);

int KMP(unsigned Lp, const char p[], unsigned Ls, const char s[],
        const int Next[]);

bool kmpInFile(FILE *fp, const char *pattern, const int *next);

#ifdef __cplusplus
}
#endif

#endif
