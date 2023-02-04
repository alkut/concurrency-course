#pragma once

#include <exe/executors/executor.hpp>

#include <exe/executors/tp/fast/queues/global_queue.hpp>
#include <exe/executors/tp/fast/worker.hpp>
#include <exe/executors/tp/fast/coordinator.hpp>
#include <exe/executors/tp/fast/metrics.hpp>

// random_device
#include <twist/ed/stdlike/random.hpp>

#include <deque>

namespace exe::executors::tp::fast {

// Scalable work-stealing scheduler for short-lived tasks

class ThreadPool : public IExecutor {
  friend class Worker;

 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // IExecutor
  void Execute(TaskBase* task) override;

  void WaitIdle();

  void Stop();

  // After Stop
  PoolMetrics Metrics() const;

  static ThreadPool* Current();

 private:
  std::deque<Worker> workers_;
  Coordinator coordinator_;
  GlobalQueue global_tasks_;
  // ???
};

}  // namespace exe::executors::tp::fast
