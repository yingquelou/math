#include <RomanNumerals.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_roman_string_lifecycle(void) {
  RomanString *str = RomanStringAlloc("IVX", 3);
  assert(str != NULL);
  assert(str->sz == 3);
  assert(memcmp(str->data, "IVX", 3) == 0);
  RomanStringRelease(str);

  str = RomanStringAlloc(NULL, 0);
  assert(str == NULL);

  str = RomanStringAlloc("test", -1);
  assert(str != NULL);
  assert(str->sz == 4);
  assert(memcmp(str->data, "test", 4) == 0);
  RomanStringRelease(str);

  str = RomanStringAlloc(NULL, 5);
  assert(str != NULL);
  assert(str->sz == 5);
  RomanStringRelease(str);
}

static void test_roman_context(void) {
  RomanContext *ctx = RomanContextAlloc("IVXLCDM", 7);
  assert(ctx != NULL);
  assert(ctx->max == 4000);
  RomanContextRelease(ctx);

  ctx = RomanContextAlloc("IVXLCD", 6);
  assert(ctx != NULL);
  assert(ctx->max == 900);
  RomanContextRelease(ctx);

  ctx = RomanContextAlloc("IV", 2);
  assert(ctx != NULL);
  assert(ctx->max == 9);
  RomanContextRelease(ctx);

  ctx = RomanContextAlloc("IVX", 3);
  assert(ctx != NULL);
  assert(ctx->max == 40);
  RomanContextRelease(ctx);
}

static void test_custom_character_set(void) {
  RomanContext *ctx = RomanContextAlloc("ABabCDcd", 8);
  assert(ctx != NULL);
  assert(ctx->max == 9000);

  RomanString *str = RomanFromNumber(ctx, 1);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(str->data[0] == 'A');
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 4);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(str->data[0] == 'A');
  assert(str->data[1] == 'B');
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 5);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(str->data[0] == 'B');
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 9);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(str->data[0] == 'A');
  assert(str->data[1] == 'a');
  RomanStringRelease(str);

  assert(NumberFromRoman(ctx, "A", 1) == 1);
  assert(NumberFromRoman(ctx, "AB", 2) == 4);
  assert(NumberFromRoman(ctx, "B", 1) == 5);
  assert(NumberFromRoman(ctx, "Aa", 2) == 9);
  assert(NumberFromRoman(ctx, "a", 1) == 10);

  for (size_t n = 1; n < 100; ++n) {
    str = RomanFromNumber(ctx, n);
    assert(str != NULL);
    size_t result = NumberFromRoman(ctx, str->data, str->sz);
    assert(result == n);
    RomanStringRelease(str);
  }

  RomanContextRelease(ctx);
}

static void test_roman_from_number(void) {
  RomanContext *ctx = RomanContextAlloc("IVXLCDM", 7);
  assert(ctx != NULL);

  RomanString *str;

  str = RomanFromNumber(ctx, 1);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(memcmp(str->data, "I", 1) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 3);
  assert(str != NULL);
  assert(str->sz == 3);
  assert(memcmp(str->data, "III", 3) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 4);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "IV", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 5);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(memcmp(str->data, "V", 1) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 9);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "IX", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 10);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(memcmp(str->data, "X", 1) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 40);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "XL", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 90);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "XC", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 400);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "CD", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 900);
  assert(str != NULL);
  assert(str->sz == 2);
  assert(memcmp(str->data, "CM", 2) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 1000);
  assert(str != NULL);
  assert(str->sz == 1);
  assert(memcmp(str->data, "M", 1) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 3999);
  assert(str != NULL);
  assert(str->sz == 9);
  assert(memcmp(str->data, "MMMCMXCIX", 9) == 0);
  RomanStringRelease(str);

  str = RomanFromNumber(ctx, 42);
  assert(str != NULL);
  assert(str->sz == 4);
  assert(memcmp(str->data, "XLII", 4) == 0);
  RomanStringRelease(str);

  RomanContextRelease(ctx);
}

static void test_number_from_roman(void) {
  RomanContext *ctx = RomanContextAlloc("IVXLCDM", 7);
  assert(ctx != NULL);

  assert(NumberFromRoman(ctx, "I", 1) == 1);
  assert(NumberFromRoman(ctx, "III", 3) == 3);
  assert(NumberFromRoman(ctx, "IV", 2) == 4);
  assert(NumberFromRoman(ctx, "V", 1) == 5);
  assert(NumberFromRoman(ctx, "IX", 2) == 9);
  assert(NumberFromRoman(ctx, "X", 1) == 10);
  assert(NumberFromRoman(ctx, "XL", 2) == 40);
  assert(NumberFromRoman(ctx, "XC", 2) == 90);
  assert(NumberFromRoman(ctx, "CD", 2) == 400);
  assert(NumberFromRoman(ctx, "CM", 2) == 900);
  assert(NumberFromRoman(ctx, "M", 1) == 1000);
  assert(NumberFromRoman(ctx, "MMMCMXCIX", 9) == 3999);
  assert(NumberFromRoman(ctx, "XLII", 4) == 42);
  assert(NumberFromRoman(ctx, "MCMLIV", 6) == 1954);

  RomanContextRelease(ctx);
}

static void test_roundtrip(void) {
  RomanContext *ctx = RomanContextAlloc("IVXLCDM", 7);
  assert(ctx != NULL);

  for (size_t n = 1; n < ctx->max; ++n) {
    RomanString *str = RomanFromNumber(ctx, n);
    assert(str != NULL);
    size_t result = NumberFromRoman(ctx, str->data, str->sz);
    assert(result == n);
    RomanStringRelease(str);
  }

  RomanContextRelease(ctx);
}

static void test_error_cases(void) {
  RomanContext *ctx = RomanContextAlloc("IVXLCDM", 7);
  assert(ctx != NULL);

  assert(RomanFromNumber(ctx, 0) == NULL);
  assert(RomanFromNumber(ctx, 4000) == NULL);

  assert(NumberFromRoman(ctx, NULL, 0) == 0);
  assert(NumberFromRoman(ctx, "", 0) == 0);
  assert(NumberFromRoman(ctx, "IVXLCDM", 7) == 0);
  assert(NumberFromRoman(ctx, "A", 1) == 0);
  assert(NumberFromRoman(ctx, "IA", 2) == 0);

  assert(NumberFromRoman(ctx, "IIII", 4) == 0);
  assert(NumberFromRoman(ctx, "VV", 2) == 0);
  assert(NumberFromRoman(ctx, "LL", 2) == 0);
  assert(NumberFromRoman(ctx, "DD", 2) == 0);
  assert(NumberFromRoman(ctx, "IC", 2) == 0);
  assert(NumberFromRoman(ctx, "IL", 2) == 0);
  assert(NumberFromRoman(ctx, "IM", 2) == 0);
  assert(NumberFromRoman(ctx, "XD", 2) == 0);
  assert(NumberFromRoman(ctx, "XM", 2) == 0);
  assert(NumberFromRoman(ctx, "CCCC", 4) == 0);
  assert(NumberFromRoman(ctx, "MMMM", 4) == 0);

  RomanContextRelease(ctx);
}

int main(int argc, char const *argv[], char const *env[]) {
  (void)argc;
  (void)argv;
  (void)env;

  test_roman_string_lifecycle();
  printf("RomanString lifecycle tests passed\n");

  test_roman_context();
  printf("RomanContext tests passed\n");

  test_custom_character_set();
  printf("Custom character set tests passed\n");

  test_roman_from_number();
  printf("RomanFromNumber tests passed\n");

  test_number_from_roman();
  printf("NumberFromRoman tests passed\n");

  test_roundtrip();
  printf("Roundtrip tests passed\n");

  test_error_cases();
  printf("Error cases tests passed\n");

  printf("\nAll tests passed!\n");
  return 0;
}
