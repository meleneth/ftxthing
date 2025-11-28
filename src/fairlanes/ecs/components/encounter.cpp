#include "encounter.hpp"
#include "fairlanes/concepts/team.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::ecs::components {

Encounter::Encounter(EntityCtx context)
    : ctx_(std::move(context)) {

      };

void on_encounter_destroy(entt::registry &reg, entt::entity e) {
  auto &enc = reg.get<Encounter>(e); // valid: signal fires before removal
  enc.finalize();
}

void install_encounter_hooks(entt::registry &reg) {
  reg.on_destroy<Encounter>().connect<&on_encounter_destroy>();
}

void Encounter::finalize() {
  for (auto e_cleanup : e_to_cleanup_) {
    ctx_.reg_.destroy(e_cleanup);
  }

  // ctx_.log_.append_markup(fmt::format("Encounter {} finalized and cleaned
  // up",
  //                                     int(entt::to_integral(ctx_.self_))));
}

bool Encounter::has_alive_enemies() {
  using fairlanes::ecs::components::Stats;

  for (auto e : e_to_cleanup_) {
    if (!ctx_.reg_.valid(e) || !ctx_.reg_.all_of<Stats>(e)) {
      continue; // stale or already destroyed, ignore
    }

    auto &enemy = ctx_.reg_.get<Stats>(e);
    if (enemy.is_alive()) {
      return true;
    }
  }

  return false;
}

bool Encounter::is_over() {
  // For now, "over" simply means there are no alive enemies.
  return !has_alive_enemies();
}
} // namespace fairlanes::ecs::components
