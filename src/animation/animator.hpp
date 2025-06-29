#pragma once
#include <vector>
#include <mutex>
#include <chrono>

#include "animation.hpp"

class Animator {
public:
  void Add(Animation anim);
  void Update(std::chrono::milliseconds dt);

private:
  std::mutex mutex;
  std::vector<Animation> animations;
};
