#pragma once

namespace exe::fibers {

// https://gobyexample.com/waitgroups

class WaitGroup {
 public:
  void Add(size_t /*count*/) {
    // Not implemented
  }

  void Done() {
    // Not implemented
  }

  // One-shot
  void Wait() {
    // Not implemented
  }

 private:
  // ???
};

}  // namespace exe::fibers
