#pragma once
#include <string>

namespace fairlanes {

struct Name {
  std::string value;
};
struct Health {
  int current, max;
};
struct PlayerControlled {};
struct EnemyControlled {};
struct Level {
  int value;
};

} // namespace fairlanes