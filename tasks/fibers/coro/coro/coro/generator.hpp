#pragma once

#include <exe/coroutine/impl.hpp>
#include <sure/stack.hpp>

#include <optional>

namespace exe::coroutine::generators {

template <typename T>
class Generator {
 public:
  explicit Generator(Routine /*routine*/) {
    // Not implemented
  }

  // Pull
  std::optional<T> Receive() {
    return std::nullopt;  // Not implemented
  }

  static void Send(T /*value*/) {
    // Not implemented
  }

 private:
  // Intentionally naive and inefficient
  static sure::Stack AllocateStack() {
    static const size_t kStackPages = 16;  // 16 * 4KB = 64KB
    return sure::Stack::AllocatePages(kStackPages);
  }

 private:
  /// ???
};

// Shortcut
template <typename T>
void Send(T value) {
  Generator<T>::Send(std::move(value));
}

}  // namespace exe::coroutine::generators
