#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/concepts/encounter_builder.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct NextEvent {};

struct PartyLoop {
  void enter_idle(PartyLoopCtx &ctx);
  void enter_farming(PartyLoopCtx &ctx);
  void exit_farming(PartyLoopCtx &ctx);
  void enter_fixing(PartyLoopCtx &ctx);

  bool needs_town(PartyLoopCtx &ctx);
  bool in_combat(PartyLoopCtx &ctx);

  void combat_tick(PartyLoopCtx &ctx);

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
