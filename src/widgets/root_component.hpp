#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

class RootComponent : public ftxui::ComponentBase {
public:
  RootComponent();
  ftxui::Element Render() override;
  void toggle_console();
  void set_full_open();

private:
  ftxui::Component header_;
  ftxui::Component body_;
  ftxui::Component footer_;
  ftxui::Component console_;
  ftxui::Component container_;
};
