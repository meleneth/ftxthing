// src/ecs/query.hpp
#pragma once
#include "ecs/components.hpp"
#include <entt/entt.hpp>

namespace fairlanes::ecs {

// for_each_account(reg, fn(entt::entity account))
template <class Fn> inline void for_each_account(entt::registry &reg, Fn &&fn) {
  auto v = reg.view<IsAccount>();
  for (auto e : v)
    fn(e);
}

// for_each_party_of(reg, account, fn(entt::entity party))
template <class Fn>
inline void for_each_party_of(entt::registry &reg, entt::entity account,
                              Fn &&fn) {
  auto v = reg.view<PartyTag, OwnedBy>();
  for (auto [e, owned] : v.each()) {
    if (owned.owner == account)
      fn(e);
  }
}

// Optional: tight loop over players of a party
template <class Fn>
inline void for_each_player_of(entt::registry &reg, entt::entity party,
                               Fn &&fn) {
  auto v = reg.view<PlayerTag, OwnedBy>();
  for (auto [e, owned] : v.each()) {
    if (owned.owner == party)
      fn(e);
  }
}

} // namespace fairlanes::ecs
