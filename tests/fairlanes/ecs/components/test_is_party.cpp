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

TEST_CASE("IsPartyParty", "[entity][builder]") {
  using fairlanes::ecs::components::IsParty;
  using fairlanes::ecs::components::PartyMember;
  using fairlanes::ecs::components::SelectedParty;

  SECTION("#for_each_party_member") {
    auto some_counter = 0;
    fairlanes::AppConfig app_config;
    fairlanes::GrandCentral gc{app_config};
    gc.create_initial_accounts();
    auto view = gc.ctx_.reg_->view<SelectedParty>();
    REQUIRE(view.size() == 1);
    // auto party = *view.begin();
    // auto &selected_party = gc.ctx_.reg_->get<SelectedParty>(party);

    /*selected_party.for_each_party_member(
        gc.ctx_.reg_, party, [&](entt::entity member) { some_counter++; });
    REQUIRE(some_counter == 5);
    */
  }
}
