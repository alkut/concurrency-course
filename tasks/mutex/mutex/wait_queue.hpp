#pragma once

#include <twist/stdlike/atomic.hpp>

namespace solutions {

class WaitQueue {
 public:
  void Park() {
    // Direct futex syscall
    queue_.FutexWait(0);
  }

  void WakeOne() {
    queue_.FutexWakeOne();
  }

  void WakeAll() {
    queue_.FutexWakeAll();
  }

 private:
  twist::stdlike::atomic<uint32_t> queue_{0};
};

}  // namespace solutions
