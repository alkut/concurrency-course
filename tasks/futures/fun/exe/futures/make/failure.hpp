#pragma once

#include <exe/futures/types/error.hpp>
#include <exe/futures/types/future.hpp>

namespace exe::futures {

template <typename T>
Future<T> Failure(Error) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
