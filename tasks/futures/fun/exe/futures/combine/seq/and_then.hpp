#pragma once

#include <exe/futures/syntax/pipe.hpp>

namespace exe::futures {

namespace pipe {

template <typename F>
struct [[nodiscard]] AndThen {
  F fun;

  explicit AndThen(F f)
      : fun(std::move(f)) {
  }

  // Result<U>
  template <typename T>
  using R = std::invoke_result_t<F, T>;

  template <typename T>
  using U = typename R<T>::value_type;

  template <typename T>
  Future<U<T>> Pipe(Future<T>) {
    std::abort();  // Not implemented
  }
};

}  // namespace pipe

// Future<T> -> Result<U>(T) -> Future<U>

template <typename F>
auto AndThen(F fun) {
  return pipe::AndThen{std::move(fun)};
}

}  // namespace exe::futures
