#include "is_party.hpp"
#include "app/app_context.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using namespace fairlanes::fsm;

IsParty::IsParty(AppContext &context, entt::entity party, std::string name,
                 entt::entity account)
    : ctx_{&context.registry(), party, nullptr}, sm_{PartyLoop{}, ctx_},
      account_{account}, name_{name} {
  // spdlog::debug("IsParty ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

void IsParty::next() {
  using fairlanes::fsm::NextEvent;
  sm_.process_event(NextEvent{});
}
} // namespace fairlanes::ecs::components
