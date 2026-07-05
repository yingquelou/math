#include "kmp.h"
#include "kmp2.h"
#include "sunday.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_kmp_in_file_edge_cases(void) {
  assert(kmpInFile(NULL, "abc", NULL) == false);
  assert(kmpInFile(stdout, NULL, NULL) == false);

  FILE *fp = tmpfile();
  assert(fp != NULL);
  fputs("hello world, hello kmp, hello universe", fp);
  rewind(fp);

  int next_small[16] = {0};
  assert(GetNext(5, "hello", next_small) == true);
  rewind(fp);
  assert(kmpInFile(fp, "hello", next_small) == true);

  int next_miss[16] = {0};
  assert(GetNext(4, "xyzq", next_miss) == true);
  rewind(fp);
  assert(kmpInFile(fp, "xyzq", next_miss) == false);

  fclose(fp);
}

static void test_kmp_in_file_single_char(void) {
  FILE *fp = tmpfile();
  assert(fp != NULL);
  fputs("ab", fp);
  rewind(fp);

  int next_one[2] = {0};
  assert(GetNext(1, "a", next_one) == true);
  rewind(fp);
  assert(kmpInFile(fp, "a", next_one) == true);

  int next_no[2] = {0};
  assert(GetNext(1, "z", next_no) == true);
  rewind(fp);
  assert(kmpInFile(fp, "z", next_no) == false);

  fclose(fp);
}

static void test_kmp_basic(void) {
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
}

static void test_kmp_kmp2_variant(void) {
  char t1[] = "aabaaf";
  char s1[] = "aabaacaabaaf";
  int Ls1 = (int)strlen(s1);
  int Lt1 = (int)strlen(t1);
  int *next_arr = (int *)calloc((size_t)Lt1, sizeof(int));
  assert(next_arr != NULL);
  assert(GetNext((unsigned)Lt1, t1, next_arr) == true);
  int pos = KMP((unsigned)Lt1, t1, (unsigned)Ls1, s1, next_arr);
  assert(pos >= 0);
  free(next_arr);
}

static void test_kmp_edge_cases(void) {
  assert(GetNext(0, "abc", NULL) == false);
  assert(GetNext(1, "a", NULL) == false);
  assert(GetNext(3, NULL, NULL) == false);
  assert(KMP(0, "abc", 10, "ababc", NULL) == -1);
  assert(KMP(3, "abc", 2, "ab", NULL) == -1);
}

static void test_sunday_basic(void) {
  char source1[] = "Hello, world!";
  char pattern1[] = "world";
  assert(Sunday(source1, (int)strlen(source1), pattern1,
                (int)strlen(pattern1)) == 7);

  char source2[] = "GitHub Copilot";
  char pattern2[] = "Copilot";
  assert(Sunday(source2, (int)strlen(source2), pattern2,
                (int)strlen(pattern2)) == 7);

  char source3[] = "abcabcabc";
  char pattern3[] = "abc";
  assert(Sunday(source3, (int)strlen(source3), pattern3,
                (int)strlen(pattern3)) == 0);

  char source4[] = "abcabcabc";
  char pattern4[] = "def";
  assert(Sunday(source4, (int)strlen(source4), pattern4,
                (int)strlen(pattern4)) == -1);
}

static void test_kmp2_basic(void) {
  const char *t = "aabxaabxcaabxaabxay";
  int nextval[20];
  int expected[20] = {0, 0, 1, 2, 0, 1, 2, 3, 4, 5,
                      0, 1, 2, 3, 0, 1, 2, 3, 4, 5};
  unsigned i;
  assert(GetNextVal(20, t, nextval) == true);
  for (i = 0; i < 20; ++i)
    assert(nextval[i] == expected[i]);

  const char *t2 = "aabaaf";
  int nextval2[6];
  int expected2[6] = {0, 0, 1, 0, 1, 2};
  assert(GetNextVal(6, t2, nextval2) == true);
  for (i = 0; i < 6; ++i)
    assert(nextval2[i] == expected2[i]);
}

static void test_kmp2_edge_cases(void) {
  int dummy[4];
  assert(GetNextVal(0, "abc", dummy) == false);
  assert(GetNextVal(1, "a", dummy) == false);
  assert(GetNextVal(3, NULL, dummy) == false);
  assert(GetNextVal(3, "abc", NULL) == false);

  const char *t = "aaaa";
  int nextval[4];
  int expected[4] = {0, 0, 0, 0};
  unsigned i;
  assert(GetNextVal(4, t, nextval) == true);
  for (i = 0; i < 4; ++i)
    assert(nextval[i] == expected[i]);
}

int main(void) {
  printf("Running string matching tests...\n");

  printf("  KMP in-file edge cases...\n");
  test_kmp_in_file_edge_cases();
  printf("  KMP in-file single-char...\n");
  test_kmp_in_file_single_char();
  printf("  KMP basic tests...\n");
  test_kmp_basic();
  printf("  KMP variant tests...\n");
  test_kmp_kmp2_variant();
  printf("  KMP edge cases...\n");
  test_kmp_edge_cases();
  printf("  Sunday basic tests...\n");
  test_sunday_basic();
  printf("  KMP2 basic (nextval) tests...\n");
  test_kmp2_basic();
  printf("  KMP2 edge (nextval) tests...\n");
  test_kmp2_edge_cases();

  printf("All string matching tests passed!\n");
  return 0;
}
