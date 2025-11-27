#include "encounter_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"

namespace fairlanes::concepts {

void EncounterBuilder::thump_it_out(fairlanes::context::EntityCtx &ctx) {
  using namespace fairlanes::ecs::components;

  // Build an enemy entity with default components
  const entt::entity e = EntityBuilder(ctx.reg_)
                             .with_default<Stats>()
                             .with_default<Tags>()
                             .build();

  // Configure the enemy
  auto field_mouse = [&](entt::entity x) {
    auto &s = ctx.reg_.get<Stats>(x); // <- ctx.reg_-> (pointer)
    s.name_ = "Field Mouse";
    s.hp_ = 5;
    s.max_hp_ = std::max<int>(s.max_hp_, 5); // keep bars sane
    s.mp_ = 0;

    //   auto &t = ctx.reg_->get<Tags>(x); // <- ctx.reg_->
    //  t.values = {"mouse", "precious"};
  };
  field_mouse(e);
  // Attach / ensure an Encounter on the party and add the enemy
  auto &enc =
      ctx.reg_.emplace<Encounter>(ctx.self_, ctx.entity_context(ctx.self_));
  auto &is_party = ctx.reg_.get<IsParty>(ctx.self_);
  enc.attackers_ = std::make_unique<fairlanes::concepts::Team>(
      ctx.entity_context(ctx.self_));
  enc.defenders_ = std::make_unique<fairlanes::concepts::Team>(
      ctx.entity_context(ctx.self_));

  // First strike wen?
  enc.attackers_->members_.push_back(e);
  enc.e_to_cleanup_.push_back(e);

  is_party.for_each_member(
      [&](entt::entity member) { enc.defenders_->members_.push_back(member); });
}

} // namespace fairlanes::concepts
