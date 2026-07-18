#include "RomanNumerals.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define __ctx ((struct __RomanContextInternal*)ctx)
struct __RomanContextInternal {
  size_t max;
  RomanString *str;
  size_t bsz;
};
static size_t __pow10(size_t exp);
static RomanString *__RomanFromNumber(const RomanContext *ctx, size_t num) {
  RomanString *str = RomanStringAlloc(NULL, __ctx->bsz);
  if (!str) {
    return NULL;
  }
  size_t pos = 0;
  while (num >= 1) {
    size_t Y = floor(log10(num));
    size_t offset = Y * 2;
    size_t n1 = __pow10(Y), opt;
    char c1 = ctx->str->data[offset];
    char c5 = ctx->str->data[offset + 1];
    char c10 = ctx->str->data[offset + 2];
    switch (opt = num / n1) {
    case 9:
      str->data[pos++] = c1;
      str->data[pos++] = c10;
      break;
    case 8:
      str->data[pos++] = c5;
      str->data[pos++] = c1;
    case 7:
      if (opt == 7)
        str->data[pos++] = c5;
      str->data[pos++] = c1;
    case 6:
      if (opt == 6)
        str->data[pos++] = c5;
      str->data[pos++] = c1;
      break;
    case 5:
    case 4:
      if (opt == 4)
        str->data[pos++] = c1;
      str->data[pos++] = c5;
      break;
    case 3:
      str->data[pos++] = c1;
    case 2:
      str->data[pos++] = c1;
    case 1:
      str->data[pos++] = c1;
      break;
    }
    num -= n1 * opt;
  }
  RomanString *tmp = realloc(str, sizeof(RomanString) + pos);
  if (tmp) {
    str = tmp;
    str->sz = pos;
  } else {
    RomanStringRelease(str);
    str = NULL;
  }
  return str;
}
static size_t __pow10(size_t exp) {
  size_t result = 1;
  for (size_t i = 0; i < exp; ++i) {
    result *= 10;
  }
  return result;
}

RomanContext *RomanContextAlloc(const char *roman, size_t sz) {
  RomanString *str = RomanStringAlloc(roman, sz);
  if (!str) {
    return NULL;
  }
  RomanContext *ctx = malloc(sizeof(struct __RomanContextInternal));
  if (!ctx) {
    RomanStringRelease(str);
    return NULL;
  }
  __ctx->str = str;
  size_t q = str->sz / 2;
  size_t r = str->sz % 2;
  size_t m = __pow10(q);
  __ctx->bsz = q * 4;
  if (r) {
    __ctx->max = 4 * m;
    __ctx->bsz += 3;
  } else {
    __ctx->max = m / 10 * 9;
  }
  return ctx;
}
void RomanContextRelease(RomanContext *ctx) {
  __ctx->max = 0;
  __ctx->bsz = 0;
  RomanStringRelease(__ctx->str);
  __ctx->str = NULL;
  free(ctx);
}
RomanString *RomanFromNumber(const RomanContext *ctx, size_t num) {
  if (num && num < __ctx->max) {
    return __RomanFromNumber(ctx, num);
  } else {
    fprintf(stderr,
            "Roman numerals defined by '%.*s' "
            "can represent decimal numbers in the open interval (0, %zd), "
            "so the decimal number '%zd' cannot be represented.\n",
            (int)__ctx->str->sz, __ctx->str->data, __ctx->max, num);
    return NULL;
  }
}
static size_t __char_value(const RomanContext *ctx, char c) {
  for (size_t i = 0; i < __ctx->str->sz; ++i) {
    if (__ctx->str->data[i] == c) {
      size_t power = i / 2;
      if (i % 2 == 0) {
        return __pow10(power);
      } else {
        return 5 * __pow10(power);
      }
    }
  }
  return 0;
}

static int __is_five_char(const RomanContext *ctx, char c) {
  for (size_t i = 0; i < __ctx->str->sz; ++i) {
    if (__ctx->str->data[i] == c) {
      return (i % 2 == 1);
    }
  }
  return 0;
}

static int __is_valid_subtraction(const RomanContext *ctx, char small, char large) {
  size_t small_idx = (size_t)-1, large_idx = (size_t)-1;
  for (size_t i = 0; i < __ctx->str->sz; ++i) {
    if (__ctx->str->data[i] == small) {
      small_idx = i;
    }
    if (__ctx->str->data[i] == large) {
      large_idx = i;
    }
  }
  if (small_idx == (size_t)-1 || large_idx == (size_t)-1) {
    return 0;
  }
  if (small_idx % 2 != 0) {
    return 0;
  }
  return (large_idx == small_idx + 1 || large_idx == small_idx + 2);
}

size_t NumberFromRoman(const RomanContext *ctx, const char *roman, size_t sz) {
  if (!roman) {
    fprintf(stderr, "Roman numeral string is NULL.\n");
    return 0;
  }
  if (!sz || sz == (size_t)-1) {
    sz = strlen(roman);
  }
  if (!sz) {
    fprintf(stderr, "Roman numeral string is empty.\n");
    return 0;
  }
  size_t result = 0;
  size_t prev_value = 0;
  char prev_char = '\0';
  size_t repeat_count = 0;
  for (size_t i = 0; i < sz; ++i) {
    size_t value = __char_value(ctx, roman[i]);
    if (value == 0) {
      fprintf(stderr, "Invalid character '%c' in Roman numeral string.\n", roman[i]);
      return 0;
    }
    if (roman[i] == prev_char) {
      repeat_count++;
    } else {
      repeat_count = 1;
    }
    if (__is_five_char(ctx, roman[i]) && repeat_count > 1) {
      fprintf(stderr, "Invalid Roman numeral: '%c' cannot be repeated.\n", roman[i]);
      return 0;
    }
    if (!__is_five_char(ctx, roman[i]) && repeat_count > 3) {
      fprintf(stderr, "Invalid Roman numeral: '%c' cannot be repeated more than 3 times.\n", roman[i]);
      return 0;
    }
    if (value > prev_value && prev_value != 0) {
      if (!__is_valid_subtraction(ctx, prev_char, roman[i])) {
        fprintf(stderr, "Invalid Roman numeral: '%c' cannot be subtracted by '%c'.\n",
                roman[i], prev_char);
        return 0;
      }
      result += value - 2 * prev_value;
    } else {
      result += value;
    }
    prev_value = value;
    prev_char = roman[i];
  }
  if (result >= __ctx->max) {
    fprintf(stderr,
            "Roman numeral '%.*s' represents value %zd, "
            "which exceeds the maximum representable value %zd.\n",
            (int)sz, roman, result, __ctx->max - 1);
    return 0;
  }
  return result;
}
RomanString *RomanStringAlloc(const char *roman, size_t sz) {
  if (!sz || sz == (size_t)-1) {
    if (roman)
      sz = strlen(roman);
    else
      sz = 0;
  }

  RomanString *str = NULL;
  if (sz) {
    str = malloc(sizeof(RomanString) + sz);
    if (str) {
      str->sz = sz;
      if (roman) {
        memcpy(str->data, roman, sz);
      } else {
        memset(str->data, 0, sz);
      }
    }
  }
  return str;
}
void RomanStringRelease(RomanString *str) {
  if (str) {
    str->sz = 0;
    free(str);
  }
}