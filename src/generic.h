#pragma once

#include <stddef.h>
#include <stdint.h>

#include "immintrin.h"

#define packed_struct struct __attribute__((__packed__))
#define packed_union union __attribute__((__packed__))

[[noreturn]] void Panic(const char* s);

#ifndef assert
void __assert(const char* expr_str, const char* file, int line);
#define assert(expr) \
  ((void)((expr) || (__assert(#expr, __FILE__, __LINE__), 0)))
#endif

constexpr uint64_t kPageSizeExponent = 12;
constexpr uint64_t kPageSize = 1 << kPageSizeExponent;
constexpr uint64_t kPageAddrMask = kPageSize - 1;
inline uint64_t ByteSizeToPageSize(uint64_t byte_size) {
  return (byte_size + kPageSize - 1) >> kPageSizeExponent;
}

#ifdef LIUMOS_LOADER
#include "loader_support.h"
#else
#include <new>
#endif
