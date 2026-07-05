#include "kmp2.h"

bool GetNextVal(unsigned length, const char *arr, int *nextval) {
  if ((length < 2) || !arr || !nextval)
    return false;

  unsigned i = 1;
  int j = 0;
  nextval[0] = 0;

  while (i < length) {
    if (j == 0 || arr[i] == arr[j]) {
      ++i;
      ++j;
      if (i < length) {
        if (arr[i] != arr[j])
          nextval[i] = j;
        else
          nextval[i] = nextval[j];
      }
    } else {
      j = nextval[j];
    }
  }
  return true;
}
