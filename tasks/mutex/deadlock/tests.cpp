#include "sims.hpp"

#include <wheels/test/framework.hpp>

#include <tf/rt/scheduler.hpp>

#include <wheels/system/quick_exit.hpp>

// Deadlock with one fiber and one mutex

TEST_SUITE(DeadLock) {
  TEST(SimOneFiber, wheels::test::TestOptions().ForceFork()) {
    tf::rt::Scheduler scheduler;

    scheduler.SetDeadlockHandler([] {
      std::cout << "DeadLock detected" << std::endl;
      wheels::QuickExit(0);  // World is broken, leave it ASAP
    });

    scheduler.Run([] {
      OneFiberDeadLock();
    });
  }

  TEST(SimTwoFibers, wheels::test::TestOptions().ForceFork()) {
    tf::rt::Scheduler scheduler;

    scheduler.SetDeadlockHandler([] {
      std::cout << "DeadLock detected" << std::endl;
      wheels::QuickExit(0);  // World is broken, leave it ASAP
    });

    scheduler.Run([] {
      TwoFibersDeadLock();
    });
  }
}

RUN_ALL_TESTS()
