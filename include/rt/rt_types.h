/*===============================================================*
 *       Copyright (c) Fux Games,Inc. All Rights Reserved.       *
  *= ============================================================= = */

#pragma once

#include "rt_base.h"

// base types
typedef unsigned char byte; // 8 bits
typedef unsigned short word;
typedef unsigned long dword;

typedef unsigned int uint;

typedef int8_t int8, i8;
typedef uint8_t uint8, u8;

typedef int16_t int16, i16;
typedef uint16_t uint16, u16;

typedef int32_t int32, i32;
typedef uint32_t uint32, u32;

typedef int64_t int64, i64;
typedef uint64_t uint64, u64;

typedef float f32;
typedef double f64;

typedef unsigned int usize;
typedef int isize;

// The C/C++ standard guarantees the size of an unsigned type is the same as the
// signed type. The exact size in bytes of several types is guaranteed here.
assert_sizeof(bool, 1);
assert_sizeof(char, 1);
assert_sizeof(short, 2);
assert_sizeof(int, 4);
assert_sizeof(float, 4);
assert_sizeof(byte, 1);
assert_sizeof(i8, 1);
assert_sizeof(u8, 1);
assert_sizeof(i16, 2);
assert_sizeof(u16, 2);
assert_sizeof(i32, 4);
assert_sizeof(u32, 4);
assert_sizeof(i64, 8);
assert_sizeof(u64, 8);

assert_sizeof(f32, 4);
assert_sizeof(f64, 8);

#ifndef time_t
typedef int64 time_t; // Signed because -1 means "File not found" and we don't
                      // want that to compare > than any other time
#endif
