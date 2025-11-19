#include "encounter.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {
std::vector<entt::entity> Encounter::players(PartyLoopCtx &ctx_) {
  using fairlanes::ecs::components::PartyMember;
  using fairlanes::ecs::components::Stats;
  std::vector<entt::entity> players_;
  auto &reg = *ctx_.reg_;
  auto view = reg.view<PartyMember, Stats>();
  for (auto &&[entity, member, stats] : view.each()) {
    if (member.party_ == ctx_.party_ && stats.hp_ > 0) {
      players_.push_back(entity);
    }
  }
  return players_;
}
entt::entity Encounter::random_alive_enemy(PartyLoopCtx &ctx_) {
  (void)ctx_;
  return enemies_[0]; // TODO you're a bad man
}
entt::entity Encounter::random_alive_player(PartyLoopCtx &ctx_) {
  std::vector<entt::entity> players_ = players(ctx_);
  return players_[0]; // TODO you're a bad man
}

Encounter::Encounter(fairlanes::context::EncounterCtx &ctx_, entt::entity party)
    : ctx_(std::move(ctx)), party_(party)) {

      };

void on_encounter_destroy(entt::registry &reg, entt::entity e) {
  auto &enc = reg.get<Encounter>(e); // valid: signal fires before removal
  enc.finalize(reg, e);
}

void install_encounter_hooks(entt::registry &reg) {
  reg.on_destroy<Encounter>().connect<&on_encounter_destroy>();
}

void Encounter::finalize(entt::registry &reg, entt::entity e) const {
  // Cleanup logic — maybe despawn enemies or notify parties
  for (auto enemy : enemies_) {
    reg.destroy(enemy);
  }
  ctx_.log_->append_markup(fmt::format("Encounter {} finalized and cleaned up",
                                       int(entt::to_integral(e))));
}
} // namespace fairlanes::ecs::components
