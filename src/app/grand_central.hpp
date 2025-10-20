#pragma once
#include "widgets/root_component.hpp"

namespace fairways {
class GrandCentral {
  GrandCentral();

private:
  std::shared_ptr<RootComponent> root_component;
};
} // namespace fairways