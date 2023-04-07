#pragma once

#include <exe/futures/types/future.hpp>

#include <exe/result/types/error.hpp>

namespace exe::futures {

template <typename T>
Future<T> Fail(Error) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
