#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "account_battle_view.hpp"
#include "combatant.hpp"
#include "fairlanes/ecs/components/selected_account.hpp"

namespace fairlanes::widgets {

AccountBattleView::AccountBattleView(AccountCtx &ctx) : ctx_(ctx) {
  auto view = ctx_.reg_->view<fairlanes::ecs::components::SelectedAccount>();
  auto entity = *view.begin();
  // auto &selected_account =
  //   ctx_.reg_->get<fairlanes::ecs::components::SelectedAccount>(entity);

  auto row = ftxui::Container::Horizontal({});

  // selected_party.for_each_party_member([&](entt::entity member) {
  //  row->Add(ftxui::Make<fairlanes::widgets::Combatant>(ctx_.reg_, member));
  //});

  body_ = row;
}
} // namespace fairlanes::widgets