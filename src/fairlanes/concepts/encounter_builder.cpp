#include "encounter_builder.hpp"
#include "app/app_context.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"

namespace fairlanes::concepts {
using fairlanes::fsm::PartyLoopCtx;

void EncounterBuilder::thump_it_out(PartyLoopCtx &ctx) {
  using namespace fairlanes::ecs::components;

  // Build an enemy entity with default components
  const entt::entity e = EntityBuilder(*ctx.reg_)
                             .with_default<Stats>()
                             .with_default<Tags>()
                             .build();

  // Configure the enemy
  auto field_mouse = [&](entt::entity x) {
    auto &s = ctx.reg_->get<Stats>(x); // <- ctx.reg_-> (pointer)
    s.name_ = "Field Mouse";
    s.hp_ = 5;
    s.max_hp_ = std::max<int>(s.max_hp_, 5); // keep bars sane
    s.mp_ = 0;

    //   auto &t = ctx.reg_->get<Tags>(x); // <- ctx.reg_->
    //  t.values = {"mouse", "precious"};
  };
  field_mouse(e);

  // Attach / ensure an Encounter on the party and add the enemy
  auto &enc = ctx.reg_->emplace<Encounter>(ctx.party_); // <- ctx.reg_->
  enc.enemies_.push_back(e);
}

} // namespace fairlanes::concepts
