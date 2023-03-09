#include <exe/executors/thread_pool.hpp>
#include <exe/executors/strand.hpp>
#include <exe/executors/submit.hpp>

#include <twist/test/with/wheels/stress.hpp>

#include <twist/test/budget.hpp>

#include <thread>

using namespace exe::executors;
using namespace std::chrono_literals;

/////////////////////////////////////////////////////////////////////

class OnePassBarrier {
 public:
  explicit OnePassBarrier(size_t threads) : total_(threads) {
  }

  void Pass() {
    arrived_.fetch_add(1);
    while (arrived_.load() < total_) {
      std::this_thread::yield();
    }
  }

 private:
  const size_t total_{0};
  std::atomic<size_t> arrived_{0};
};

void MaybeAnomaly() {
  ThreadPool pool{1};

  while (twist::test::KeepRunning()) {
    Strand strand(pool);
    OnePassBarrier barrier{2};

    size_t done = 0;

    Submit(strand, [&done, &barrier] {
      ++done;
      barrier.Pass();
    });

    barrier.Pass();

    Submit(strand, [&done] {
      ++done;
    });

    pool.WaitIdle();

    ASSERT_EQ(done, 2);
  }

  pool.Stop();
}

//////////////////////////////////////////////////////////////////////

TEST_SUITE(MemoryOrders) {
  TWIST_TEST(MaybeAnomaly, 5s) {
    MaybeAnomaly();
  }
}

RUN_ALL_TESTS()
