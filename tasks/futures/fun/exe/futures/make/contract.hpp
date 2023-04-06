#pragma once

#include <exe/result/types/result.hpp>
#include <exe/futures/types/future.hpp>

#include <tuple>

namespace exe::futures {

template <typename T>
class Promise {
 public:
  void Set(Result<T>) {
    // Not implemented
  }

  void SetValue(T) {
    // Not implemented
  }

  void SetError(std::error_code) {
    // Not implemented
  }
};

template <typename T>
std::tuple<Future<T>, Promise<T>> Contract() {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
