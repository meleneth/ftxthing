#pragma once
#include <memory>
#include <string>

namespace fairlanes::widgets {
class FancyLog;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog; // Marks an entity as an Account
struct SelectedCharacter {
  SelectedCharacter();
};

} // namespace fairlanes::ecs::components
