#pragma once

#include <exe/result/types/result.hpp>

namespace exe::result {

/*
 * Usage:
 *
 * futures::Value(1) | futures::AndThen([](int v) -> Result<int> {
 *   return result::Error(Timeout());
 * })
 *
 */

inline auto Error(std::error_code ec) {
  return tl::unexpected(ec);
}

}  // namespace exe::result
