// team.hpp
#pragma once
#include <optional>
#include <vector>

#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/stats.hpp"

namespace fairlanes::concepts {

struct Team {
  fairlanes::context::EntityCtx ctx_;
  std::vector<entt::entity> members_;

  Team(fairlanes::context::EntityCtx ctx) : ctx_(std::move(ctx)) {}

  MARK_CLASS_MOVEONLY(Team);

  template <typename Fn> inline void for_each_alive_member(Fn &&fn) {
    using fairlanes::ecs::components::Stats;

    for (auto e : members_) {
      auto &stats = ctx_.reg_.get<Stats>(e); // from BE_REGISTRY
      if (stats.is_alive()) { // or stats.is_alive if it's a field
        std::forward<Fn>(fn)(e);
      }
    }
  }
  template <typename Fn> inline void for_each_member(Fn &&fn) {
    using fairlanes::ecs::components::Stats;

    for (auto e : members_) {
      std::forward<Fn>(fn)(e);
    }
  }

  bool has_alive_members() const {
    using fairlanes::ecs::components::Stats;
    auto &reg = ctx_.reg_;
    for (auto e : members_) {
      if (auto stats = reg.try_get<Stats>(e); stats && stats->hp_ > 0) {
        return true;
      }
    }
    return false;
  }

  std::vector<entt::entity> alive_members() const {
    using fairlanes::ecs::components::Stats;
    auto &reg = ctx_.reg_;
    std::vector<entt::entity> out;
    out.reserve(members_.size());
    for (auto e : members_) {
      if (auto stats = reg.try_get<Stats>(e); stats && stats->hp_ > 0) {
        out.push_back(e);
      }
    }
    return out;
  }

  std::optional<entt::entity> random_alive_member() {
    using fairlanes::ecs::components::Stats;

    auto rs = ctx_.rng_.stream("encounter/players");

    std::optional<entt::entity> selected;
    int seen = 0;

    for_each_alive_member([&](entt::entity candidate) {
      ++seen;
      if (rs.random_index(seen) == 0) {
        selected = candidate;
      }
    });
    return selected;
  }
};

} // namespace fairlanes::concepts
