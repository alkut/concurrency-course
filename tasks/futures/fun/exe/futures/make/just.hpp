#pragma once

#include <exe/futures/types/ack.hpp>

namespace exe::futures {

inline Ack Just() {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
