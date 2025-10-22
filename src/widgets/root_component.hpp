#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "widgets/fancy_log.hpp"

namespace fairlanes {

class RootComponent : public ftxui::ComponentBase {
public:
  explicit RootComponent(std::shared_ptr<fairlanes::FancyLog> console);
  ftxui::Element Render() override;
  void toggle_console();
  void set_full_open();

private:
  ftxui::Component header_;
  ftxui::Component body_;
  ftxui::Component footer_;
  std::shared_ptr<fairlanes::FancyLog> console_;
  ftxui::Component console_overlay_;
  ftxui::Component container_;
};

} // namespace fairlanes