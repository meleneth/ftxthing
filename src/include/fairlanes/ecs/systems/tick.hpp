#pragma once
#include "fairlanes/ecs/components/account.hpp"
#include "fairlanes/ecs/components/party.hpp"
#include "fairlanes/ecs/fwd.hpp"
#include <utility>

namespace fairlanes::ecs::systems {
namespace cmp = fairlanes::ecs::components;

// -------- Ranges/iterators (lightweight helpers) --------

// Iterate all accounts (entities with IsAccount)
struct AccountsRange {
  fairlanes::ecs::Registry &reg;
  struct It {
    fairlanes::ecs::Registry *r{};
    entt::basic_view<entt::get_t<cmp::IsAccount>> view;
    entt::basic_view<entt::get_t<cmp::IsAccount>>::iterator it;

    It(fairlanes::ecs::Registry *rr, decltype(view) v, bool end = false)
        : r(rr), view(v), it(end ? v.end() : v.begin()) {}

    fairlanes::ecs::Entity operator*() const { return *it; }
    It &operator++() {
      ++it;
      return *this;
    }
    bool operator==(const It &o) const { return it == o.it; }
    bool operator!=(const It &o) const { return !(*this == o); }
  };

  It begin() { return It{&reg, reg.view<cmp::IsAccount>(), false}; }
  It end() { return It{&reg, reg.view<cmp::IsAccount>(), true}; }
};

inline AccountsRange accounts(fairlanes::ecs::Registry &reg) {
  return AccountsRange{reg};
}

// Iterate parties of a given account
struct PartiesOfAccountRange {
  fairlanes::ecs::Registry &reg;
  fairlanes::ecs::Entity account;

  struct It {
    fairlanes::ecs::Registry *r{};
    fairlanes::ecs::Entity account{};
    entt::basic_view<entt::get_t<cmp::IsParty, cmp::PartyOfAccount>> view;
    entt::basic_view<entt::get_t<cmp::IsParty, cmp::PartyOfAccount>>::iterator
        it;

    It(fairlanes::ecs::Registry *rr, fairlanes::ecs::Entity acc,
       decltype(view) v, bool end = false)
        : r(rr), account(acc), view(v), it(end ? v.end() : v.begin()) {
      // advance to first matching account
      while (it != view.end() &&
             view.get<cmp::PartyOfAccount>(*it).account != account)
        ++it;
    }

    fairlanes::ecs::Entity operator*() const { return *it; }
    It &operator++() {
      if (it != view.end())
        ++it;
      while (it != view.end() &&
             view.get<cmp::PartyOfAccount>(*it).account != account)
        ++it;
      return *this;
    }
    bool operator==(const It &o) const { return it == o.it; }
    bool operator!=(const It &o) const { return !(*this == o); }
  };

  It begin() {
    auto v = reg.view<cmp::IsParty, cmp::PartyOfAccount>();
    return It{&reg, account, v, false};
  }
  It end() {
    auto v = reg.view<cmp::IsParty, cmp::PartyOfAccount>();
    return It{&reg, account, v, true};
  }
};

inline PartiesOfAccountRange parties_of(fairlanes::ecs::Registry &reg,
                                        fairlanes::ecs::Entity account) {
  return PartiesOfAccountRange{reg, account};
}

struct MembersOfPartyRange {
  fairlanes::ecs::Registry &reg;
  fairlanes::ecs::Entity party;

  struct It {
    fairlanes::ecs::Registry *r{};
    fairlanes::ecs::Entity party{};
    using View = entt::basic_view<entt::get_t<cmp::PartyMember>>;
    View view;
    View::iterator it;

    It(fairlanes::ecs::Registry *rr, fairlanes::ecs::Entity p, View v,
       bool end = false)
        : r(rr), party(p), view(v), it(end ? v.end() : v.begin()) {
      // advance to first matching party
      while (it != view.end() && view.get<cmp::PartyMember>(*it).party != party)
        ++it;
    }

    fairlanes::ecs::Entity operator*() const { return *it; }

    It &operator++() {
      if (it != view.end())
        ++it;
      while (it != view.end() && view.get<cmp::PartyMember>(*it).party != party)
        ++it;
      return *this;
    }

    bool operator==(const It &o) const { return it == o.it; }
    bool operator!=(const It &o) const { return !(*this == o); }
  };

  It begin() {
    auto v = reg.view<cmp::PartyMember>();
    return It{&reg, party, v, false};
  }
  It end() {
    auto v = reg.view<cmp::PartyMember>();
    return It{&reg, party, v, true};
  }
};

inline MembersOfPartyRange members_of(fairlanes::ecs::Registry &reg,
                                      fairlanes::ecs::Entity party) {
  return MembersOfPartyRange{reg, party};
}

// -------- Orchestrator: tick_all --------
//
// party_fn(reg, account_entity, party_entity) is user-supplied per-party tick.
//
template <class PartyFn>
void tick_all(fairlanes::ecs::Registry &reg, PartyFn &&party_fn) {
  for (auto account : accounts(reg)) {
    for (auto party : parties_of(reg, account)) {
      std::forward<PartyFn>(party_fn)(reg, account, party);
    }
  }
}

} // namespace fairlanes::ecs::systems
