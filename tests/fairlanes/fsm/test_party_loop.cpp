// tests/test_party_loop.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"

#include "fairlanes/context/entity_ctx.hpp"

#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"

#include "fairlanes/concepts/encounter_builder.hpp"
#include "fairlanes/fsm/party_loop.hpp"

TEST_CASE("PartyLoop SML enters Farming and creates Encounter",
          "[fsm][party][encounter]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::IsParty;

  fairlanes::AppConfig app_config;
  fairlanes::GrandCentral gc{app_config};

  // Set up a basic account/party with a single member so EncounterBuilder
  // has something sane to work with.
  auto account = gc.create_account("account1");
  auto party = gc.create_party_in_account("party1", account);
  auto player = gc.create_member_in_party("player1", party);
  (void)player;

  auto &reg = gc.ctx_.reg_;
  REQUIRE_FALSE(reg.all_of<Encounter>(party));

  // Grab the IsParty component that owns the PartyLoop SML instance.
  auto &is_party = reg.get<IsParty>(party);

  // First NextEvent should take us Idle -> Farming and call enter_farming,
  // which should build an Encounter for this party.
  is_party.next();

  REQUIRE(reg.all_of<Encounter>(party));
}

TEST_CASE("PartyLoop enter/exit_farming create and clean up Encounter",
          "[fsm][party][encounter][lifecycle]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::Encounter;

  fairlanes::AppConfig app_config;
  fairlanes::GrandCentral gc{app_config};

  auto account = gc.create_account("account1");
  auto party = gc.create_party_in_account("party1", account);
  auto player = gc.create_member_in_party("player1", party);
  (void)player;

  auto &reg = gc.ctx_.reg_;
  REQUIRE_FALSE(reg.all_of<Encounter>(party));

  // Build an EntityCtx for this party, like the rest of your code does.
  EntityCtx ctx = gc.ctx_.entity_context(party);

  fairlanes::fsm::PartyLoop loop;

  // Explicitly exercise the hooks the SML uses.
  loop.enter_farming(ctx);
  REQUIRE(reg.all_of<Encounter>(party));

  loop.exit_farming(ctx);
  REQUIRE_FALSE(reg.all_of<Encounter>(party));

  /// SECTION("")
}

TEST_CASE("PartyLoop#in_combat", "[fsm][party][encounter][lifecycle]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::IsParty;

  fairlanes::AppConfig app_config{};
  fairlanes::GrandCentral gc{app_config};

  auto account = gc.create_account("account1");
  auto party = gc.create_party_in_account("party1", account);
  auto player = gc.create_member_in_party("player1", party);
  (void)player; // silence unused for now

  auto &reg = gc.ctx_.reg_;

  auto &is_party = reg.get<IsParty>(party);
  spdlog::info("player and is_party created");

  REQUIRE_FALSE(is_party.in_combat());
  fairlanes::concepts::EncounterBuilder(gc.ctx_.entity_context(party))
      .thump_it_out();
  REQUIRE(is_party.in_combat());
}

TEST_CASE("PartyLoop#needs_town", "[fsm][party][encounter][lifecycle]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::IsParty;

  fairlanes::AppConfig app_config{};
  fairlanes::GrandCentral gc{app_config};

  auto account = gc.create_account("account1");
  auto party = gc.create_party_in_account("party1", account);
  auto player = gc.create_member_in_party("player1", party);
  (void)player; // silence unused for now

  auto &reg = gc.ctx_.reg_;

  auto &is_party = reg.get<IsParty>(party);
  spdlog::info("player and is_party created");

  REQUIRE_FALSE(is_party.needs_town());
  fairlanes::concepts::EncounterBuilder{is_party.ctx_}.thump_it_out();
  auto &player_stats = reg.get<fairlanes::ecs::components::Stats>(player);
  player_stats.hp_ = 0;
  REQUIRE(is_party.needs_town());
}
/*
TEST_CASE("PartyLoop utility functions", "[fsm][party][encounter][lifecycle]") {
  using fairlanes::context::EntityCtx;
  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::IsParty;

  fairlanes::AppConfig app_config{};
  fairlanes::GrandCentral gc{app_config};

  auto account = gc.create_account("account1");
  auto party = gc.create_party_in_account("party1", account);
  auto player = gc.create_member_in_party("player1", party);
  (void)player; // silence unused for now

  auto &reg = gc.ctx_.reg_;
  auto &is_party = reg.get<IsParty>(party);
  spdlog::info("player and is_party created");

  // Get an EntityCtx for this party so we can use it with Encounter / actions.
  EntityCtx ctx = gc.ctx_.entity_context(party);

  // Initially: no Encounter while we're in Farming / Idle.
  REQUIRE_FALSE(reg.all_of<Encounter>(party));
  spdlog::info("no Encounter while Idle");

  // First NextEvent from Farming should put us into combat and attach
  // Encounter.
  is_party.next();
  REQUIRE(reg.all_of<Encounter>(party));
  spdlog::info("an Encounter in combat");

  // Drive FSM one more time (this depends on your transition table, but matches
  // what you sketched earlier: Farming + NextEvent [...] ->
  // Farming/CombatIdle/etc).
  is_party.next();

  spdlog::info("check 'needs town'");
  REQUIRE(is_party.needs_town() == false);

  spdlog::info("check 'in_combat'");
  REQUIRE(is_party.in_combat() == true);

  // Make sure we can actually touch the Encounter component.
  auto &encounter = ctx.reg_.get<Encounter>(party);
  (void)encounter; // for now, just prove it exists

  // Exiting farming/combat should clean up the Encounter.
  // is_party.exit_farming(ctx);
  // REQUIRE_FALSE(reg.all_of<Encounter>(party));
}
*/
