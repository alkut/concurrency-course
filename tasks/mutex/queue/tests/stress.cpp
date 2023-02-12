#include "../queue_spinlock.hpp"

#include <twist/test/test.hpp>

#include <twist/test/race.hpp>
#include <twist/test/plate.hpp>

#include <wheels/test/util.hpp>

#include <chrono>

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////////////////////////

TEST_SUITE(Mutex) {
  void Test(size_t threads) {
    twist::test::Plate plate;  // Guarded by mutex
    QueueSpinLock spinlock;

    twist::test::Race race;

    for (size_t i = 0; i < threads; ++i) {
      race.Add([&]() {
        while (wheels::test::KeepRunning()) {
          QueueSpinLock::Guard guard(spinlock);
          {
            // Critical section
            plate.Access();
          }
        }
      });
    }

    race.Run();

    std::cout << "Critical sections: " << plate.AccessCount() << std::endl;
  }

  TWIST_TEST_TL(Stress1, 5s) {
    Test(2);
  }

  TWIST_TEST_TL(Stress2, 5s) {
    Test(5);
  }
}

////////////////////////////////////////////////////////////////////////////////

TEST_SUITE(MissedWakeup) {
  void Test(size_t threads) {
    QueueSpinLock spinlock;

    twist::test::Race race;

    for (size_t i = 0; i < threads; ++i) {
      race.Add([&]() {
        QueueSpinLock::Guard guard(spinlock);
        // Critical section
      });
    };

    race.Run();
  }

  TWIST_ITERATE_TEST(Stress1, 5s) {
    Test(2);
  }

  TWIST_ITERATE_TEST(Stress2, 5s) {
    Test(3);
  }
}

////////////////////////////////////////////////////////////////////////////////

RUN_ALL_TESTS()
