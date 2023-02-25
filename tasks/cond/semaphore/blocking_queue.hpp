#pragma once

#include "tagged_semaphore.hpp"

#include <deque>

// Bounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

template <typename T>
class BlockingQueue {
 public:
  explicit BlockingQueue(size_t /*capacity*/) {
  }

  void Put(T /*value*/) {
    // Not implemented
  }

  T Take() {
    return T{};  // Not implemented
  }

 private:
  // Tags
  struct SomeTag {};

 private:
  // Buffer
};
