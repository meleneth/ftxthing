#include <entt/entt.hpp>

#include "entities/entities.hpp"
#include "screens/battle_screen.hpp"

int main() {
  entt::registry reg = build_demo_registry();

  BattleScreen(reg).Run();
  return 0;
}
