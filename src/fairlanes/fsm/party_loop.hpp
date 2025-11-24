#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/concepts/encounter_builder.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct NextEvent {};

struct PartyLoop {
  void enter_idle(fairlanes::context::EntityCtx &ctx);
  void enter_farming(fairlanes::context::EntityCtx &ctx);
  void exit_farming(fairlanes::context::EntityCtx &ctx);
  void enter_fixing(fairlanes::context::EntityCtx &ctx);

  bool needs_town(fairlanes::context::EntityCtx &ctx);
  bool in_combat(fairlanes::context::EntityCtx &ctx);

  void combat_tick(fairlanes::context::EntityCtx &ctx);

  auto operator()() const {
    using namespace sml;
    using namespace fairlanes::ecs::components;

    struct Idle {};
    struct Farming {};
    struct Fixing {};

    return make_transition_table(
        *state<Idle> + on_entry<_> / enter_idle,
        state<Farming> + on_entry<_> / enter_farming,
        state<Farming> + on_exit<_> / exit_farming,
        state<Fixing> + on_entry<_> / enter_fixing,

        state<Idle> + event<NextEvent> = state<Farming>,
        state<Farming> + event<NextEvent>[needs_town] = state<Fixing>,
        state<Farming> + event<NextEvent> / combat_tick,
        state<Fixing> + event<NextEvent> = state<Idle>);
  }
};

} // namespace fairlanes::fsm
