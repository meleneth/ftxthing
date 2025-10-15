#pragma once
#include <string>

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
