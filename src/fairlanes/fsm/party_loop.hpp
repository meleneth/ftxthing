#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/ecs/components/party_business.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct NextEvent {};

inline void set_doing(PartyLoopCtx &ctx, const char *label) {
  using fairlanes::ecs::components::PartyBusiness;

  // spdlog::debug("use   : reg={}, party={}", fmt::ptr(ctx.reg),
  //               (uint32_t)entt::to_integral(ctx.party));

  auto &business = ctx.reg->get<PartyBusiness>(ctx.party);
  business.doing = label;
}

struct PartyLoop {
  auto operator()() const {
    using namespace sml;

    struct Idle {};
    struct Farming {};
    struct Fixing {};

    const auto enter_idle = [](PartyLoopCtx &ctx) { set_doing(ctx, "idle"); };
    const auto enter_farming = [](PartyLoopCtx &ctx) {
      set_doing(ctx, "farming");
    };
    const auto enter_fixing = [](PartyLoopCtx &ctx) {
      set_doing(ctx, "fixing");
    };

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
