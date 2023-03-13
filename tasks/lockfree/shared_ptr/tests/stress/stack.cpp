#include "../../lock_free_stack.hpp"

#include <twist/test/with/wheels/stress.hpp>

#include <twist/rt/layer/fault/adversary/adversary.hpp>

#include <twist/test/race.hpp>
#include <twist/test/lockfree.hpp>
#include <twist/test/random.hpp>

#include <twist/test/budget.hpp>

#include <limits>
#include <vector>

//////////////////////////////////////////////////////////////////////

template <typename T>
struct Counted {
  static std::atomic<size_t> object_count;
  static size_t obj_count_limit;

  Counted() {
    IncrementCount();
  }

  Counted(const Counted& /*that*/) {
    IncrementCount();
  }

  Counted(Counted&& /*that*/) {
    IncrementCount();
  }

  Counted& operator=(const Counted& that) = default;
  Counted& operator=(Counted&& that) = default;

  ~Counted() {
    DecrementCount();
  }

  static void SetLiveLimit(size_t count) {
    obj_count_limit = count;
  }

  static size_t LiveObjectCount() {
    return object_count.load();
  }

 private:
  static void IncrementCount() {
    ASSERT_TRUE_M(
        object_count.fetch_add(1) + 1 < obj_count_limit,
        "Too many alive test objects: " << object_count.load());
  }

  static void DecrementCount() {
    object_count.fetch_sub(1);
  }
};

template <typename T>
std::atomic<size_t> Counted<T>::object_count = 0;

template <typename T>
size_t Counted<T>::obj_count_limit = std::numeric_limits<size_t>::max();

//////////////////////////////////////////////////////////////////////

struct TestObject: public Counted<TestObject> {
  size_t value;

  explicit TestObject(size_t _value)
      : value(_value) {
  }

  static TestObject Make() {
    static const size_t kValueRange = 1000007;

    return TestObject{twist::test::Random(kValueRange)};
  }
};

//////////////////////////////////////////////////////////////////////

void StressTest(size_t threads, size_t batch_size_limit) {
  twist::test::ReportProgressFor<LockFreeStack<TestObject>> stack;

  std::atomic<size_t> ops{0};
  std::atomic<size_t> pushed{0};
  std::atomic<size_t> popped{0};

  TestObject::SetLiveLimit(1024);

  twist::test::Race race;

  for (size_t i = 0; i < threads; ++i) {
    race.Add([&]() {
      twist::test::EnablePark guard;

      while (twist::test::KeepRunning()) {
        size_t batch_size = twist::test::Random(1, batch_size_limit);

        // Push

        for (size_t j = 0; j < batch_size; ++j) {
          auto obj = TestObject::Make();

          stack->Push(obj);
          pushed.fetch_add(obj.value);
          ++ops;
        }

        // Pop

        for (size_t j = 0; j < batch_size; ++j) {
          auto obj = stack->TryPop();
          ASSERT_TRUE(obj);
          popped.fetch_add(obj->value);
        }
      }
    });
  }

  race.Run();

  std::cout << "Operations #: " << ops.load() << std::endl;
  std::cout << "Pushed: " << pushed.load() << std::endl;
  std::cout << "Popped: " << popped.load() << std::endl;

  ASSERT_EQ(pushed.load(), popped.load());

  // Stack is empty
  ASSERT_FALSE(stack->TryPop());

  std::cout << "Live objects = " << TestObject::LiveObjectCount() << std::endl;
  // Memory leak
  ASSERT_TRUE(TestObject::LiveObjectCount() == 0);
}

TEST_SUITE(LockFreeStack) {
  TWIST_TEST(Stress1, 5s) {
    StressTest(/*threads=*/2, /*batch_size_limit=*/2);
  }

  TWIST_TEST(Stress2, 5s) {
    StressTest(/*threads=*/5, /*batch_size_limit=*/1);
  }

  TWIST_TEST(Stress3, 5s) {
    StressTest(/*threads=*/5, /*batch_size_limit=*/3);
  }

  TWIST_TEST(Stress4, 5s) {
    StressTest(/*threads=*/5, /*batch_size_limit=*/5);
  }
}

