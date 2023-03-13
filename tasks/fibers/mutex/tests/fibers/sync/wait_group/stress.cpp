#include <twist/test/with/wheels/stress.hpp>

#include <twist/test/repeat.hpp>
#include <twist/test/random.hpp>

#include <exe/executors/thread_pool.hpp>
#include <exe/fibers/sched/go.hpp>
#include <exe/fibers/sync/wait_group.hpp>

#include <atomic>
#include <chrono>

using namespace exe;
using namespace std::chrono_literals;

//////////////////////////////////////////////////////////////////////

void StressTest() {
  executors::ThreadPool scheduler{/*threads=*/4};
  scheduler.Start();

  for (twist::test::Repeat repeat; repeat.Test(); ) {
    size_t workers = 1 + twist::test::Random(3);
    size_t waiters = 1 + twist::test::Random(3);

    fibers::WaitGroup wg;

    std::atomic<size_t> work{0};
    std::atomic<size_t> acks{0};

    wg.Add(workers);

    // Waiters

    for (size_t i = 0; i < waiters; ++i) {
      fibers::Go(scheduler, [&] {
        wg.Wait();
        ASSERT_EQ(work.load(), workers);
        acks.fetch_add(1);
      });
    }

    // Workers

    for (size_t j = 0; j < workers; ++j) {
      fibers::Go(scheduler, [&] {
        work.fetch_add(1);
        wg.Done();
      });
    }

    scheduler.WaitIdle();

    ASSERT_EQ(acks.load(), waiters);
  }

  scheduler.Stop();
}

//////////////////////////////////////////////////////////////////////

TEST_SUITE(WaitGroup) {
  TWIST_TEST(Stress, 5s) {
    StressTest();
  }
}

RUN_ALL_TESTS()
