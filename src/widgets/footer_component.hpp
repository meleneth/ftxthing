#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

class FooterComponent : public ftxui::ComponentBase {
public:
  void SetMessage(std::string msg);
  ftxui::Element Render() override;

private:
  std::string msg_ = "Footer ready.";
};
