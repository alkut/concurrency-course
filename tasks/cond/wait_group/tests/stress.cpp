#include "../wait_group.hpp"

#include <twist/test/with/wheels/stress.hpp>

#include <twist/test/race.hpp>
#include <twist/test/random.hpp>

void StressTest() {
  WaitGroup wg;

  size_t workers = 1 + twist::test::Random(4);
  size_t waiters = 1 + twist::test::Random(4);

  std::atomic<size_t> work{0};

  twist::test::Race race;

  wg.Add(workers);

  for (size_t i = 0; i < waiters; ++i) {
    race.Add([&] {
      wg.Wait();
      ASSERT_EQ(work.load(), workers);
    });
  }

  for (size_t i = 1; i <= workers; ++i) {
    race.Add([&] {
      ++work;
      wg.Done();
    });
  }

  race.Run();
}

TEST_SUITE(WaitGroup) {
  TWIST_TEST_REPEAT(Stress, 5s) {
    StressTest();
  }
}

RUN_ALL_TESTS();
