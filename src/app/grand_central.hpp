#pragma once
#include "widgets/root_component.hpp"

namespace fairways {
class GrandCentral {
public:
  GrandCentral();
  void main_loop();

private:
  std::shared_ptr<RootComponent> root_component;
};
} // namespace fairways