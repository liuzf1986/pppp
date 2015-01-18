#pragma once

#include <stdio.h>
#include <stdlib.h>

template <int N>
class SimpleAllocator {
 public:
  static inline void* alloc(size_t size) {
    return ::malloc(N * size);
  }

  static inline void release(void* ptr) {
    return ::free(ptr);
  }
};



















