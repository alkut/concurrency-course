#pragma once

#include <tl/expected.hpp>

#include <system_error>

namespace exe {

// https://github.com/TartanLlama/expected

template <typename T>
using Result = tl::expected<T, std::error_code>;

}  // namespace exe
