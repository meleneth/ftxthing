#pragma once
#include <chrono>
#include <functional>

class Animation {
public:
  void Advance(std::chrono::milliseconds dt);

  bool IsFinished() const { return finished; }
  bool Looping() const { return looping; }

  // Configurable fields
  std::chrono::milliseconds duration{0};
  std::chrono::milliseconds tick_interval{16};
  bool looping = false;

  std::function<void(float)> on_tick;     // receives 0.0 → 1.0
  std::function<void()> on_complete;

private:
  std::chrono::milliseconds elapsed{0};
  std::chrono::milliseconds since_last_tick{0};
  bool finished = false;
};
