#include <exe/executors/thread_pool.hpp>

#include <exe/futures/make/contract.hpp>
#include <exe/futures/make/submit.hpp>
#include <exe/futures/make/value.hpp>
#include <exe/futures/make/fail.hpp>
#include <exe/futures/make/just.hpp>

#include <exe/futures/combine/seq/map.hpp>
#include <exe/futures/combine/seq/and_then.hpp>
#include <exe/futures/combine/seq/or_else.hpp>
#include <exe/futures/combine/seq/via.hpp>
#include <exe/futures/combine/seq/flatten.hpp>

#include <exe/futures/combine/par/all.hpp>
#include <exe/futures/combine/par/first.hpp>

#include <exe/futures/terminate/get.hpp>
#include <exe/futures/terminate/forget.hpp>

#include <exe/futures/syntax/both.hpp>
#include <exe/futures/syntax/or.hpp>

#include <exe/result/make/ok.hpp>
#include <exe/result/make/err.hpp>

#include <wheels/core/panic.hpp>

#include <fmt/core.h>
#include <fmt/std.h>

using namespace exe;

Error Timeout() {
  return std::make_error_code(std::errc::timed_out);
}

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

    auto r = futures::Fail<int>(/*with=*/Timeout()) | futures::Get();

    if (!r) {
      fmt::println("Fail -> {}", *r);
    }
  }

  {
    // Submit

    auto r = futures::Submit(pool,
                             []() {
                               fmt::println("Running on thread pool");
                               return result::Ok(7);
                             }) |
             futures::Get();

    fmt::println("Submit -> {}", *r);
  }

  {
    // Submit + Map

    auto r = futures::Submit(pool,
                             []() {
                               fmt::println("Running on thread pool");
                               return result::Ok(1);
                             }) |
             futures::Map([](int v) {
               return v + 1;
             }) |
             futures::Get();

    fmt::println("Submit.Map -> {}", r.value());
  }

  {
    // Forget

    futures::Just() | futures::Map([](wheels::Unit) {
      fmt::println("Just");
    }) | futures::Forget();
  }

  {
    auto r = futures::Value(1) | futures::Map([](int v) {
               fmt::println("Inline")
                   // Inline
                   return v +
                   1;
             }) |
             futures::Via(pool) | futures::Map(int v) {
      fmt::println("ThreadPool") return v + 1;
    }) | futures::Get();

    fmt::println("Value.Map.Via.Map -> {}", *r);
  }

  {
    // AndThen / OrElse

    auto r = futures::Value(1) | futures::AndThen([](int) -> Result<int> {
               return result::Err(Timeout());
             }) |
             futures::AndThen([](int) -> Result<int> {
               wheels::Panic("Should be skipped");
               return -1;
             }) |
             futures::OrElse([](std::error_code) -> Result<int> {
               return 42;  // Fallback
             }) |
             futures::Get();

    fmt::println("AndThen.OrElse -> {}", *r);
  }

  {
    // Flatten

    futures::Future<int> f =
        futures::Submit(pool,
                        [&] {
                          return result::Ok(futures::Submit(pool, [] {
                            return result::Ok(7);
                          }));
                        }) |
        futures::Flatten();

    auto r = std::move(f) | futures::Get();

    fmt::println("Flatten -> {}", *r);
  }

  {
    // FirstOf

    auto f = futures::Submit(pool, [] {
      return result::Ok(1);
    });

    auto g = futures::Submit(pool, [] {
      return result::Ok(2);
    });

    auto r = (std::move(f) or std::move(g)) | futures::Get();

    fmt::println("FirstOf -> {}", *r);
  }

  pool.WaitIdle();
  pool.Stop();

  return 0;
}
