// tests/test_grant_xp_to_party.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"

#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/selected_party.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"

TEST_CASE("GrantXPToParty", "[systems][xp]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::IsParty;
  using fairlanes::ecs::components::PartyMember;
  using fairlanes::ecs::components::SelectedParty;
  using fairlanes::ecs::components::TrackXP;
  using fairlanes::systems::GrantXPToParty;

  constexpr int XP_AMOUNT = 123;

  fairlanes::AppConfig app_config;
  fairlanes::GrandCentral gc{app_config};

  auto account = gc.create_account("account1");
  auto party1 = gc.create_party_in_account("party1", account);
  auto party2 = gc.create_party_in_account("party2", account);

  auto player_1 = gc.create_member_in_party("player1", party1);
  auto player_2 = gc.create_member_in_party("player2", party2);
  GrantXPToParty::commit(gc.ctx_.entity_context(party1), 256);

  auto &track_xp_1 = gc.ctx_.reg_.get<TrackXP>(player_1);
  auto &track_xp_2 = gc.ctx_.reg_.get<TrackXP>(player_2);

  REQUIRE(track_xp_1.xp_ == 256);
  REQUIRE(track_xp_2.xp_ == 0);
}
