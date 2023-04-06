#pragma once

#include <exe/futures/types/future.hpp>

namespace exe::futures {

template <typename T>
Future<T> FirstOf(Future<T>, Future<T>) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
