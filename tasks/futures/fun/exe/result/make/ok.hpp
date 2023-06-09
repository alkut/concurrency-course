#pragma once

#include <exe/result/types/result.hpp>

namespace exe::result {

/*
 * Usage:
 *
 * futures::Value(1) | futures::AndThen([](int v) {
 *   return result::Ok(v + 1);
 * })
 *
 */

template <typename T>
Result<T> Ok(T value) {
  return {std::move(value)};
}

}  // namespace exe::result
