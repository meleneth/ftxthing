#include "party.hpp"
#include "app/app_context.hpp"
#include "entity_types.hpp"
#include "widgets/fancy_log.hpp"

using namespace fairlanes;
Party::Party(AppContext &ctx) : reg_(ctx.registry()) {
  create_member("Player 1");
}

entt::entity Party::create_member(std::string name) {
  auto player = reg_.create();
  reg_.emplace<Name>(player, name);
  reg_.emplace<Health>(player, 40, 50);
  reg_.emplace<PlayerControlled>(player);
  reg_.emplace<Level>(player, 1);
  return player;
}
