#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

class RootComponent : public ftxui::ComponentBase {
public:
  RootComponent();
  ftxui::Element Render() override;

private:
  ftxui::Component header_;
  ftxui::Component body_;
  ftxui::Component footer_;
  ftxui::Component console_;
};
