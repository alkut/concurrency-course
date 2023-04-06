#include <exe/executors/thread_pool.hpp>

#include <exe/futures/make/contract.hpp>
#include <exe/futures/make/submit.hpp>
#include <exe/futures/make/value.hpp>
#include <exe/futures/make/failure.hpp>
#include <exe/futures/make/just.hpp>

#include <exe/futures/combine/seq/and_then.hpp>
#include <exe/futures/combine/seq/or_else.hpp>
#include <exe/futures/combine/seq/map.hpp>
#include <exe/futures/combine/seq/via.hpp>
#include <exe/futures/combine/seq/flatten.hpp>
#include <exe/futures/combine/seq/visit.hpp>

#include <exe/futures/combine/par/all.hpp>
#include <exe/futures/combine/par/first.hpp>

#include <exe/futures/terminate/get.hpp>
#include <exe/futures/terminate/forget.hpp>

#include <exe/futures/syntax/both.hpp>
#include <exe/futures/syntax/or.hpp>

#include <wheels/core/panic.hpp>

#include <fmt/core.h>
#include <fmt/std.h>

using namespace exe;

int main() {
  executors::ThreadPool pool{4};
  pool.Start();

  {
    // Contract

    auto [f, p] = futures::Contract<int>();

    std::move(p).SetValue(7);
    auto r = std::move(f) | futures::Get();

    fmt::println("Contract -> {}", r.value());
  }

  {
    // Value

    auto r = futures::Value(42) | futures::Get();

    fmt::println("Value -> {}", *r);
  }

  {
    // Failure

    auto timeout = std::make_error_code(std::errc::timed_out);

    auto r = futures::Failure<int>(timeout) | futures::Get();

    if (!r) {
      fmt::println("Failure -> {}", *r);
    }
  }

  {
    // Submit

    auto r = futures::Submit(pool,
                             []() -> int {
                               fmt::println("Running on thread pool");
                               return 7;
                             }) |
             futures::Get();

    fmt::println("Submit -> {}", *r);
  }

  {
    // Submit + Map

    auto r = futures::Submit(pool,
                             []() -> int {
                               fmt::println("Running on thread pool");
                               return 1;
                             }) |
             futures::Map([](int v) {
               return v + 1;
             }) |
             futures::Get();

    fmt::println("Submit -> {}", r.value());
  }

  {
    // AndThen / OrElse

    auto r = futures::Value(1) |
             futures::AndThen([](int) -> futures::Result<int> {
               auto timeout = std::make_error_code(std::errc::timed_out);
               throw timeout;
             }) |
             futures::AndThen([](int) -> futures::Result<int> {
               wheels::Panic("Should be skipped");
             }) |
             futures::OrElse([](futures::Error) -> futures::Result<int> {
               return 42;  // Fallback
             }) |
             futures::Get();

    fmt::println("Pipeline -> {}", *r);
  }

  {
    futures::Future<int> g = futures::Submit(pool,
                                             [&] {
                                               return futures::Submit(pool, [] {
                                                 return 42;
                                               });
                                             }) |
                             futures::Flatten();

    auto r = std::move(g) | futures::Get();

    fmt::println("Flatten -> {}", *r);
  }

  {
    // FirstOf

    auto f = futures::Submit(pool, [] {
      return 1;
    });
    auto g = futures::Submit(pool, [] {
      return 2;
    });

    auto r = (std::move(f) or std::move(g)) | futures::Get();

    fmt::println("FirstOf -> {}", *r);
  }

  pool.WaitIdle();
  pool.Stop();

  return 0;
}
