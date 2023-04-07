#pragma once

#include <exe/futures/types/future.hpp>

#include <wheels/core/unit.hpp>

namespace exe::futures {

// Acknowledgement
using Ack = Future<wheels::Unit>;

}  // namespace exe::futures
