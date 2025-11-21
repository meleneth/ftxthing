// tests/test_entity_builder.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"
#include "entities/component_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/selected_party.hpp"

using json = nlohmann::json;

TEST_CASE("SelectedParty", "[entity][builder]") {
  using fairlanes::ecs::components::IsParty;
  using fairlanes::ecs::components::PartyMember;
  using fairlanes::ecs::components::SelectedParty;

  SECTION("#for_each_party_member") {
    auto some_counter = 0;
    fairlanes::AppConfig app_config;
    fairlanes::GrandCentral gc{app_config};
    auto &reg = gc.ctx_.reg_;
    gc.create_initial_accounts();
    auto view = reg->view<SelectedParty>();
    REQUIRE(view.size() ==
            1); // integration contract: exactly one selected party
    // auto party = *view.begin(); // the entity that has SelectedParty
    // auto &selected_party = reg->get<SelectedParty>(party);
    /* um, no?
        selected_party.for_each_party_member(
            reg, party, [&](entt::entity member) { some_counter++; });
        REQUIRE(some_counter == 5);
        */
  }
}
