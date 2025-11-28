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
  std::vector<ftxui::Element> rows;

  auto &is_account =
      ctx_.reg_.get<fairlanes::ecs::components::IsAccount>(ctx_.self_);
  is_account.for_each_party([&](entt::entity party) {
    using fairlanes::ecs::components::Encounter;
    using fairlanes::ecs::components::IsParty;
    auto &is_party = ctx_.reg_.get<IsParty>(party);
    auto encounter = ctx_.reg_.try_get<Encounter>(party);

    if (encounter && encounter->attackers_) {
      std::vector<ftxui::Element> attackers_row;
      encounter->attackers_->for_each_member([&](entt::entity member) {
        fairlanes::widgets::Combatant combatant{ctx_.reg_, member};
        attackers_row.push_back(combatant.Render());
      });
      rows.push_back(ftxui::hbox(std::move(attackers_row)));

    } else {
      ftxui::Element blank = ftxui::vbox({
                                 ftxui::filler(), // line 1
                                 ftxui::filler(), // line 2
                                 ftxui::filler(), // line 3
                             }) |
                             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1) |
                             ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3);
      rows.push_back(blank);
    }

    std::vector<ftxui::Element> party_row;
    is_party.for_each_member([&](entt::entity member) {
      fairlanes::widgets::Combatant combatant{ctx_.reg_, member};
      party_row.push_back(combatant.Render());
    });
    rows.push_back(ftxui::hbox(std::move(party_row)));
  });

  return ftxui::vbox(std::move(rows));
}

} // namespace fairlanes::widgets
