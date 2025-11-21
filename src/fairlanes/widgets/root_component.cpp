#include "root_component.hpp"

#include <tracy/Tracy.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "body_component.hpp"
#include "combatant.hpp"
#include "console_overlay.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/selected_party.hpp"
#include "fairlanes/widgets/account_battle_view.hpp"
#include "fancy_log.hpp"
#include "footer_component.hpp"

using namespace fairlanes::widgets;
void RootComponent::change_body_component(fairlanes::context::AppCtx &ctx,
                                          entt::entity party) {

  auto &is_party = ctx.reg_->get<fairlanes::ecs::components::IsParty>(party);
  body_ = ftxui::Make<fairlanes::widgets::AccountBattleView>(
      ctx.account_context(is_party.account_));

  auto row = ftxui::Container::Horizontal({});

  is_party.for_each_member([&](entt::entity member) {
    row->Add(ftxui::Make<fairlanes::widgets::Combatant>(*ctx.reg_, member));
  });

  body_ = row;
}

RootComponent::RootComponent(fairlanes::context::AppCtx ctx_) {
  using namespace ftxui;
  console_overlay_ = Make<ConsoleOverlay>(ctx_.log_.get());
  Add(console_overlay_);
}

void RootComponent::toggle_console() {
  auto overlay = console_overlay();
  overlay->toggle();
  if (overlay->should_show()) {
    container_->SetActiveChild(console_overlay_);
    overlay->FocusInput(); // ensure cursor lands in Input
  } else {
    container_->SetActiveChild(body_);
  }
}

BodyComponent *RootComponent::body() {
  return dynamic_cast<BodyComponent *>(body_.get());
}

ConsoleOverlay *RootComponent::console_overlay() {
  return dynamic_cast<ConsoleOverlay *>(console_overlay_.get());
}

void RootComponent::set_full_open() {
  auto overlay = console_overlay();
  overlay->set_full_open();
}

ftxui::Element RootComponent::Render() {
  using namespace ftxui;
  auto overlay = console_overlay();
  overlay
      ->tick(); // TODO make a lock here, mouse events are making ticks too fast
  ZoneScoped;
  if (body_) {
    auto content = vbox({body_->Render()});
    if (overlay->should_show()) {
      return dbox({content, console_overlay_->Render()});
    }
    return content;
  } else {
    if (overlay->should_show()) {
      return dbox({ftxui::text("some other text"), console_overlay_->Render()});
    }
    return ftxui::text("some text");
  }
}

void RootComponent::change_console(fairlanes::widgets::FancyLog *console) {
  console_ = console;
  console_overlay()->change_console(console);
}

void RootComponent::select_account(entt::entity account) {
  selected_account_ = account;
}
