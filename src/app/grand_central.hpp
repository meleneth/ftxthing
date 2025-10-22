#pragma once
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

namespace fairlanes {
class GrandCentral {
public:
  GrandCentral();
  void main_loop();

private:
  ftxui::Component root_component_;
  std::shared_ptr<FancyLog> console_;
};
} // namespace fairlanes