#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "account_battle_view.hpp"
#include "combatant.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "fairlanes/ecs/components/selected_account.hpp"

namespace fairlanes::widgets {

AccountBattleView::AccountBattleView(fairlanes::context::EntityCtx ctx)
    : ctx_(std::move(ctx)) {
  auto view = ctx_.reg_.view<fairlanes::ecs::components::SelectedAccount>();
  auto entity = *view.begin();
  (void)entity;
  // auto &selected_account =
  //   ctx_.reg_->get<fairlanes::ecs::components::SelectedAccount>(entity);

  // auto row = ftxui::Container::Horizontal({});

  // selected_party.for_each_party_member([&](entt::entity member) {
  //  row->Add(ftxui::Make<fairlanes::widgets::Combatant>(ctx_.reg_, member));
  //});

  // body_ = row;
}

ftxui::Element AccountBattleView::Render() {
  auto stack = ftxui::Container::Vertical({});
  auto &is_account =
      ctx_.reg_.get<fairlanes::ecs::components::IsAccount>(ctx_.self_);
  is_account.for_each_party([&](entt::entity party) {
    auto row = ftxui::Container::Horizontal({});
    stack->Add(row);
    auto &is_party = ctx_.reg_.get<fairlanes::ecs::components::IsParty>(party);
    is_party.for_each_member([&](entt::entity member) {
      row->Add(ftxui::Make<fairlanes::widgets::Combatant>(ctx_.reg_, member));
    });
  });

  return stack->Render();
}

} // namespace fairlanes::widgets
