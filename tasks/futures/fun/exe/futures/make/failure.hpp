#pragma once

#include <exe/futures/types/future.hpp>

#include <system_error>

namespace exe::futures {

template <typename T>
Future<T> Error(std::error_code) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
