/*===============================================================*
 *       Copyright (c) Fux Games,Inc. All Rights Reserved.       *
 *===============================================================*/

#pragma once
#include "rt_types.h"
// common error
#define FUX_SUCC(x) ((x) >= 0)
#define FUX_FAIL(x) ((x) < 0)

// bit
#define FUX_BIT(num) (1ULL << (num))
#define FUX_NUMBITS(type) (sizeof(type) * 8)

// Defining common SI unit macros.
//
// The mebibyte is a multiple of the unit byte for digital information.
// Technically a megabyte (MB) is a power of ten, while a mebibyte (MiB) is a
// power of two, appropriate for binary machines. Many Linux distributions use
// the unit, but it is not widely acknowledged within the industry or media.
// Reference: https://en.wikipedia.org/wiki/Mebibyte
//
// Examples:
// 	auto size1 = FUX_KILOBYTE(16);
// 	auto size2 = FUX_MEGABYTE(128);
// 	auto size3 = FUX_MEBIBYTE(8);
// 	auto size4 = FUX_GIBIBYTE(8);

// define byte for completeness
#define FUX_BYTE(x) (x)

// Decimal SI units
#define FUX_KILOBYTE(x) (size_t(x) * 1000)
#define FUX_MEGABYTE(x) (size_t(x) * 1000 * 1000)
#define FUX_GIGABYTE(x) (size_t(x) * 1000 * 1000 * 1000)
#define FUX_TERABYTE(x) (size_t(x) * 1000 * 1000 * 1000 * 1000)
#define FUX_PETABYTE(x) (size_t(x) * 1000 * 1000 * 1000 * 1000 * 1000)
#define FUX_EXABYTE(x) (size_t(x) * 1000 * 1000 * 1000 * 1000 * 1000 * 1000)

// Binary SI units
#define FUX_KIBIBYTE(x) (size_t(x) * 1024)
#define FUX_MEBIBYTE(x) (size_t(x) * 1024 * 1024)
#define FUX_GIBIBYTE(x) (size_t(x) * 1024 * 1024 * 1024)
#define FUX_TEBIBYTE(x) (size_t(x) * 1024 * 1024 * 1024 * 1024)
#define FUX_PEBIBYTE(x) (size_t(x) * 1024 * 1024 * 1024 * 1024 * 1024)
#define FUX_EXBIBYTE(x) (size_t(x) * 1024 * 1024 * 1024 * 1024 * 1024 * 1024)

// get size of
#define FUX_SIZE_KB(x) (((x) + 1023) / 1024)
#define FUX_SIZE_MB(x) (((SIZE_KB(x)) + 1023) / 1024)
#define FUX_SIZE_GB(x) (((SIZE_MB(x)) + 1023) / 1024)

// max value of
#define FUX_MAX_TYPE(x) ((((1 << ((sizeof(x) - 1) * 8 - 1)) - 1) << 8) | 255)
#define FUX_MIN_TYPE(x) (-MAX_TYPE(x) - 1)
#define FUX_MAX_UTYPE(x) ((((1U << ((sizeof(x) - 1) * 8)) - 1) << 8) | 255U)
#define FUX_MIN_UTYPE(x) 0

// common defines
#define FUX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define FUX_MAX3(x, y, z)                                                      \
  ((x) > (y) ? ((x) > (z) ? (x) : (z)) : ((y) > (z) ? (y) : (z)))

#define FUX_IS_SIGNED(type) (((type)(-1)) < 0)
#define FUX_IS_UNSIGNED(type) ((type)(-1) > 0)

#ifdef __cplusplus

namespace utils {

template <class T> FUX_INL bool is_pointer(T) { return false; }

template <class T> FUX_INL bool is_pointer(T *) { return true; }

template <class T> FUX_INL T max(T x, T y) { return x > y ? x : y; }

template <class T> FUX_INL T min(T x, T y) { return x < y ? x : y; }

template <class T> FUX_INL T clamp(T v, T max_, T min_) {
  return max(min(v, max_), min_);
}

template <typename T> FUX_INL T sqr(T x) { return x * x; }

template <typename T> FUX_INL T cube(T x) { return x * x * x; }

template <typename T> FUX_INL void mem_clr(T *p, usize len) {
  for (usize i = 0; i < len; ++i) {
    *(p + i) = 0;
  }
}

template <typename T> FUX_INL T *mem_fill(T *p, usize len, T n) {
  for (usize i = 0; i < len; ++i) {
    *(p + i) = n;
  }
  return p;
}

} // namespace utils
#endif
