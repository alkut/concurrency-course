#pragma once

#include <exe/executors/executor.hpp>
#include <exe/futures/types/future.hpp>

#include <type_traits>

namespace exe::futures {

namespace traits {

template <typename F>
using SubmitT = typename std::invoke_result_t<F>::value_type;

}  // namespace traits

template <typename F>
Future<traits::SubmitT<F>> Submit(executors::IExecutor& /*exe*/,
                                       F /*fun*/) {
  std::abort();  // Not implemented
}

}  // namespace exe::futures
