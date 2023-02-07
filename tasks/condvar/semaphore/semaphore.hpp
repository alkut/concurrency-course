#pragma once

#include <twist/ed/stdlike/mutex.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>

// std::lock_guard, std::unique_lock
#include <mutex>
#include <cstdint>

// A Counting semaphore

// Semaphores are often used to restrict the number of threads
// than can access some (physical or logical) resource

class Semaphore {
 public:
  // Creates a Semaphore with the given number of permits
  explicit Semaphore(size_t /*initial*/) {
    // Not implemented
  }

  // Acquires a permit from this semaphore,
  // blocking until one is available
  void Acquire() {
    // Not implemented
  }

  // Releases a permit, returning it to the semaphore
  void Release() {
    // Not implemented
  }

 private:
  // Permits
};
