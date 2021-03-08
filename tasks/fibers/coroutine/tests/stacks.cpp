#include <mtf/fibers/api.hpp>

#include <wheels/test/test_framework.hpp>

using mtf::fibers::Spawn;

TEST_SUITE(Stacks) {

#if !__has_feature(thread_sanitizer)

  void Recurse(size_t left) {
    mtf::fibers::Yield();

    if (left > 0) {
      Recurse(left - 1);
    }
  }

  SIMPLE_TEST(Recurse) {
    mtf::tp::StaticThreadPool scheduler{4};

    for (size_t i = 0; i < 128; ++i) {
      Spawn([]() { Recurse(128); }, scheduler);
    }

    scheduler.Join();
  }

  TEST(Pool, wheels::test::TestOptions().TimeLimit(5s).AdaptTLToSanitizer()) {
    mtf::tp::StaticThreadPool scheduler{1};

    static const size_t kFibers = 1'000'000;

    std::atomic<size_t> counter{0};

    auto spawner = [&]() {
      for (size_t i = 0; i < kFibers; ++i) {
        Spawn([&]() {
          ++counter;
        });
      }
    };

    Spawn(spawner, scheduler);

    scheduler.Join();

    ASSERT_EQ(counter, kFibers)
  }

#endif
}
