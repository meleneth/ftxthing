#pragma once
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "entities/party.hpp"
#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

namespace fairlanes {
class GrandCentral {
public:
  GrandCentral(const AppConfig &cfg);
  void main_loop();

private:
  ftxui::Component root_component_;
  std::shared_ptr<FancyLog> console_;
  std::shared_ptr<RandomHub> random_;
  entt::registry registry_;
  std::vector<Party> parties_;
};
} // namespace fairlanes