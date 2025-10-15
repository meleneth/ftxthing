#include "animation.hpp"

void Animation::Advance(std::chrono::milliseconds dt) {
  if (finished)
    return;

  elapsed += dt;
  since_last_tick += dt;

  float t =
      std::min(1.0f, static_cast<float>(elapsed.count()) / duration.count());

  if (since_last_tick >= tick_interval) {
    since_last_tick = std::chrono::milliseconds(0);
    if (on_tick)
      on_tick(t);
  }

  if (t >= 1.0f) {
    if (looping) {
      elapsed = std::chrono::milliseconds(0);
      since_last_tick = std::chrono::milliseconds(0);
    } else {
      finished = true;
      if (on_complete)
        on_complete();
    }
  }
}
