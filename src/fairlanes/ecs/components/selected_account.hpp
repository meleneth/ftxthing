#pragma once
#include <memory>
#include <string>

namespace fairlanes::context {
struct AppCtx;
} // namespace fairlanes::context
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {
struct PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;
// Marks an entity as the Selected Account
struct SelectedAccount {
  SelectedAccount();
};

} // namespace fairlanes::ecs::components
