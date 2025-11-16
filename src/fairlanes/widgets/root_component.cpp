#include "root_component.hpp"

#include <tracy/Tracy.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "app/app_context.hpp"
#include "body_component.hpp"
#include "combatant.hpp"
#include "console_overlay.hpp"
#include "fairlanes/ecs/components/selected_party.hpp"
#include "fancy_log.hpp"
#include "footer_component.hpp"

using namespace fairlanes::widgets;
void RootComponent::change_body_component(fairlanes::AppContext &ctx,
                                          entt::entity party) {

  auto &selected_party =
      ctx.registry().get<fairlanes::ecs::components::SelectedParty>(party);

  auto row = ftxui::Container::Horizontal({});

  selected_party.for_each_party_member(
      ctx.registry_, party, [&](entt::entity member) {
        row->Add(
            ftxui::Make<fairlanes::widgets::Combatant>(ctx.registry(), member));
      });

  body_ = row;
}

RootComponent::RootComponent(std::shared_ptr<FancyLog> console)
    : console_(std::move(console)) {
  using namespace ftxui;
  header_ = Renderer([this] { return console_->Render() | border; });
  body_ = Make<BodyComponent>();
  footer_ = Make<FooterComponent>();
  console_overlay_ = Make<ConsoleOverlay>(console_);

  container_ = Container::Vertical({header_, body_, footer_});
  container_->SetActiveChild(body_);

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
  auto content = vbox({
      header_->Render() | flex,
      body_->Render(),
      footer_->Render() | flex,
  });
  if (overlay->should_show()) {
    return dbox({content, console_overlay_->Render()});
  }
  return content;
}

void RootComponent::change_console(
    std::shared_ptr<fairlanes::widgets::FancyLog> console) {
  console_ = console;
  console_overlay()->change_console(console);
}

void RootComponent::select_account(entt::entity account) {
  selected_account_ = account;
}
