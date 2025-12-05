#include "encounter_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/monsters/monster_kind.hpp"
#include "fairlanes/monsters/register_monsters.hpp" // brings all the monsters with it

namespace fairlanes::concepts {
EncounterBuilder::EncounterBuilder(fairlanes::context::EntityCtx is_party_ctx)
    : ctx_(is_party_ctx) {}

void EncounterBuilder::thump_it_out() {
  using namespace fairlanes::ecs::components;

  // Attach / ensure an Encounter on the party and add the enemy
  auto &enc = ctx_.reg_.emplace<Encounter>(ctx_.self_,
                                           ctx_.encounter_context(ctx_.self_));
  auto &is_party = ctx_.reg_.get<IsParty>(ctx_.self_);
  enc.attackers_ = std::make_unique<fairlanes::concepts::Team>(
      ctx_.entity_context(ctx_.self_));
  enc.defenders_ = std::make_unique<fairlanes::concepts::Team>(
      ctx_.entity_context(ctx_.self_));
  add_field_mouse();
  is_party.for_each_member(
      [&](entt::entity member) { enc.defenders_->members_.push_back(member); });
}

void EncounterBuilder::add_field_mouse() {
  using namespace fairlanes::ecs::components;

  entt::entity e = EntityBuilder(ctx_)
                       .monster(fairlanes::monster::MonsterKind::FieldMouse)
                       .build();
  add_to_enemy_team(e);
}

void EncounterBuilder::add_to_enemy_team(entt::entity entity) {
  // First strike wen?
  auto &enc = ctx_.reg_.get<fairlanes::ecs::components::Encounter>(ctx_.self_);

  enc.attackers_->members_.push_back(entity);
  enc.e_to_cleanup_.push_back(entity);
}

} // namespace fairlanes::concepts
