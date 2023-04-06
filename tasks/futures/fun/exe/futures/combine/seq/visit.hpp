#pragma once

#include <exe/futures/syntax/pipe.hpp>

namespace exe::futures {

namespace pipe {

template <typename F>
struct [[nodiscard]] Visit {
  F fun;

  explicit Visit(F f)
      : fun(std::move(f)) {
  }

  template <typename T>
  Future<T> Pipe(Future<T>) {
    std::abort();  // Not implemented
  }
};

}  // namespace pipe

// Future<T> -> T(const T&) -> Future<U>

template <typename F>
auto Visit(F fun) {
  return pipe::Map{std::move(fun)};
}

}  // namespace exe::futures
