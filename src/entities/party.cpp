#include <fmt/core.h>

#include "app/app_context.hpp"
#include "entity_types.hpp"
#include "party.hpp"
#include "widgets/fancy_log.hpp"

using namespace fairlanes;
Party::Party(AppContext &ctx, std::string label)
    : reg_(ctx.registry()), log_(ctx.log()), rng_(ctx.rng()), label_(label) {}

entt::entity Party::create_member(AppContext &ctx, std::string name) {
  (void)ctx;
  log_.append_markup(fmt::format("Party Member [name]({}) joined!", name));
  auto player = reg_.create();
  reg_.emplace<Name>(player, name);
  reg_.emplace<Health>(player, 40, 50);
  reg_.emplace<PlayerControlled>(player);
  reg_.emplace<Level>(player, 1);
  return player;
}

void Party::tick(float dt) { (void)dt; }