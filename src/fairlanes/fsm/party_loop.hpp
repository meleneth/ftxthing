#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct NextEvent {};

struct PartyLoop {
  auto operator()() const {
    using namespace sml;
    using namespace fairlanes::ecs::components;

    struct Idle {};
    struct Farming {};
    struct Fixing {};

    const auto enter_idle = [](PartyLoopCtx &ctx) {
      // Mark the party attached to this FSM as idle.
      using fairlanes::systems::GrantXPToParty;
      entt::handle h{*ctx.reg_, ctx.party_};
      GrantXPToParty::commit(h, 256);
    };

    const auto enter_farming = [](PartyLoopCtx &ctx) {
      // Also set the label for the party tied to this FSM (nice for local UI)
      (void)ctx;
    };

    const auto enter_fixing = [](PartyLoopCtx &ctx) { (void)ctx; };

    return make_transition_table(
        *state<Idle> + on_entry<_> / enter_idle,
        state<Farming> + on_entry<_> / enter_farming,
        state<Fixing> + on_entry<_> / enter_fixing,

        state<Idle> + event<NextEvent> = state<Farming>,
        state<Farming> + event<NextEvent> = state<Fixing>,
        state<Fixing> + event<NextEvent> = state<Idle>);
  }
};

} // namespace fairlanes::fsm
