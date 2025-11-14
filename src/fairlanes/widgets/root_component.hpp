#pragma once
#include <memory>

#include <entt/entt.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace fairlanes {
struct AppContext;
}

namespace fairlanes::widgets {
class BodyComponent;
class ConsoleOverlay;
class FancyLog;

class RootComponent : public ftxui::ComponentBase {
public:
  explicit RootComponent(std::shared_ptr<fairlanes::widgets::FancyLog> console);
  ftxui::Element Render() override;
  void toggle_console();
  ConsoleOverlay *console_overlay();
  void change_console(std::shared_ptr<fairlanes::widgets::FancyLog> console);
  void select_account(entt::entity account);
  void set_full_open();
  void change_body_component(fairlanes::AppContext &ctx,
                             entt::entity character);

  BodyComponent *body();

private:
  ftxui::Component header_;
  ftxui::Component body_;
  ftxui::Component footer_;
  std::shared_ptr<fairlanes::widgets::FancyLog> console_;
  ftxui::Component console_overlay_;
  ftxui::Component container_;
  entt::entity selected_account_{entt::null};
};

} // namespace fairlanes::widgets