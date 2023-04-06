#pragma once

#include <exe/result/types/result.hpp>

#include <cstdlib>

namespace exe::futures {

template <typename T>
struct [[nodiscard]] Future {
  using ValueType = T;

  // ???
};

}  // namespace exe::futures
