#pragma once

#include <exe/result/types/result.hpp>

namespace exe::result {

/*
 * Usage:
 *
 * futures::Just() | futures::AndThen([](Unit) -> Result<int> {
 *   return result::Err(Timeout());
 * });
 *
 */

inline auto Err(std::error_code ec) {
  return tl::unexpected(ec);
}

}  // namespace exe::result
