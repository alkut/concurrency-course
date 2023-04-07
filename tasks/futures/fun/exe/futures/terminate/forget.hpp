#pragma once

#include <exe/futures/syntax/pipe.hpp>

namespace exe::futures {

namespace pipe {

struct [[nodiscard]] Forget {
  template <typename T>
  void Pipe(Future<T>) {
    std::abort();  // Not implemented
  }
};

}  // namespace pipe

inline auto Forget() {
  return pipe::Forget{};
}

}  // namespace exe::futures
