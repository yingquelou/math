#ifndef MODERNROMANNUMERALS_H
#define MODERNROMANNUMERALS_H 1
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdio.h>
typedef struct RomanString {
  size_t sz;
  char data[0];
} RomanString;
RomanString *RomanStringAlloc(const char *roman, size_t sz);
void RomanStringRelease(RomanString *str);
typedef struct RomanContext {
  /// The largest positive integer that can currently be represented by the Roman meta-character sequence is max-1.
  size_t max;
  /// Roman meta-character sequence
  RomanString *str;
} RomanContext;
/// Modern Roman numerals
/// |I|V|X|L|C|D|M|...|
/// |-|-|-|-|-|-|-|-|
/// |1|5|10|50|100|500|1000|...|
RomanContext *RomanContextAlloc(const char *roman, size_t sz);
void RomanContextRelease(RomanContext *ctx);
/// The return value's memory is allocated on the heap.
/// Please release it yourself when it is no longer needed.
RomanString *RomanFromNumber(const RomanContext *ctx, size_t num);
/// Converts a Roman numeral string to a decimal number.
/// Returns 0 on error (invalid input or out of range).
size_t NumberFromRoman(const RomanContext *ctx, const char *roman, size_t sz);
#ifdef __cplusplus
}
#endif
#endif