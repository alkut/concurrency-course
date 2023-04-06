#pragma once

#include <exe/futures/types/error.hpp>

#include <tl/expected.hpp>

namespace exe::futures {

// https://github.com/TartanLlama/expected

template <typename T>
using Result = tl::expected<T, Error>;

}  // namespace exe::futures
