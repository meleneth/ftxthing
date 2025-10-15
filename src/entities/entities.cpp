// game_state.hpp
#include "entities.hpp"
#include "entity_types.hpp"

entt::registry build_demo_registry() {
  entt::registry reg;

  // Create a player
  auto player = reg.create();
  reg.emplace<Name>(player, "Hero");
  reg.emplace<Health>(player, 40, 50);
  reg.emplace<PlayerControlled>(player);
  reg.emplace<Level>(player, 1);

  // Create a couple enemies
  auto goblin = reg.create();
  reg.emplace<Name>(goblin, "Goblin");
  reg.emplace<Health>(goblin, 20, 20);
  reg.emplace<EnemyControlled>(goblin);
  reg.emplace<Level>(goblin, 2);

  auto orc = reg.create();
  reg.emplace<Name>(orc, "Orc");
  reg.emplace<Health>(orc, 55, 60);
  reg.emplace<EnemyControlled>(orc);
  reg.emplace<Level>(orc, 4);

  return reg;
}
