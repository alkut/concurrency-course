#pragma once

#include <wheels/core/unit.hpp>

namespace exe::futures {

inline Future<wheels::Unit> Just() {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
