#pragma once

#include <exe/executors/executor.hpp>
#include <exe/futures/types/future.hpp>

#include <type_traits>

namespace exe::futures {

template <typename F>
Future<std::invoke_result_t<F>> Submit(executors::IExecutor& /*exe*/,
                                       F /*fun*/) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
