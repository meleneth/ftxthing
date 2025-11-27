#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "account_battle_view.hpp"
#include "combatant.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "fairlanes/ecs/components/selected_account.hpp"

namespace fairlanes::widgets {

AccountBattleView::AccountBattleView(fairlanes::context::EntityCtx ctx)
    : ctx_(std::move(ctx)) {}

ftxui::Element AccountBattleView::Render() {
  auto stack = ftxui::Container::Vertical({});

  auto &is_account =
      ctx_.reg_.get<fairlanes::ecs::components::IsAccount>(ctx_.self_);

  is_account.for_each_party([&](entt::entity party) {
    using fairlanes::ecs::components::Encounter;
    using fairlanes::ecs::components::IsParty;

    auto &is_party = ctx_.reg_.get<IsParty>(party);
    auto encounter = ctx_.reg_.try_get<Encounter>(party);

    // Row for attackers (if any)
    if (encounter && encounter->attackers_) {
      auto attackers_row = ftxui::Container::Horizontal({});
      stack->Add(attackers_row);

      encounter->attackers_->for_each_member([&](entt::entity member) {
        attackers_row->Add(
            ftxui::Make<fairlanes::widgets::Combatant>(ctx_.reg_, member));
      });
    }

    // Row for this party's members
    auto party_row = ftxui::Container::Horizontal({});
    stack->Add(party_row);

    is_party.for_each_member([&](entt::entity member) {
      party_row->Add(
          ftxui::Make<fairlanes::widgets::Combatant>(ctx_.reg_, member));
    });
  });

  return stack->Render();
}

} // namespace fairlanes::widgets
