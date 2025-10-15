#include "animator.hpp"

void Animator::Add(Animation anim) {
  std::lock_guard lock(mutex);
  animations.push_back(std::move(anim));
}

void Animator::Update(std::chrono::milliseconds dt) {
  std::lock_guard lock(mutex);
  for (auto &anim : animations)
    anim.Advance(dt);

  animations.erase(
      std::remove_if(animations.begin(), animations.end(),
                     [](const Animation &a) { return a.IsFinished(); }),
      animations.end());
}
