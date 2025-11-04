#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "fairlanes/ecs/components/party_business.hpp"

namespace fairlanes::fsm {

namespace sml = boost::sml;

struct PartyLoopCtx {
  entt::registry *reg{nullptr};
  entt::entity party{entt::null};
  spdlog::logger *log{nullptr};
};

struct NextEvent {};

struct PartyLoop {
  auto operator()() const {
    using namespace sml;

    struct Idle {};
    struct Farming {};
    struct Fixing {};

    // Typed ctx so it's NOT dependent (no boost::sml::back::_!)
    const auto set_doing = [](PartyLoopCtx &ctx, const char *label) {
      using fairlanes::ecs::components::PartyBusiness;
      auto &business = ctx.reg->get<PartyBusiness>(ctx.party);
      business.doing = label;
    };

    const auto enter_idle = [=](PartyLoopCtx &ctx) { set_doing(ctx, "idle"); };
    const auto enter_farming = [=](PartyLoopCtx &ctx) {
      set_doing(ctx, "farming");
    };
    const auto enter_fixing = [=](PartyLoopCtx &ctx) {
      set_doing(ctx, "fixing");
    };

    return make_transition_table(
        *state<Idle> + on_entry<_> / enter_idle,

        state<Idle> + event<NextEvent> = state<Farming>,
        state<Farming> + event<NextEvent> = state<Fixing>,
        state<Fixing> + event<NextEvent> = state<Idle>,

        state<Farming> + on_entry<_> / enter_farming,
        state<Fixing> + on_entry<_> / enter_fixing);
  }
};

} // namespace fairlanes::fsm
