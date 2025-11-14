#pragma once
#include <memory>
#include <string>

namespace fairlanes {
class AppContext;
} // namespace fairlanes
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {
struct PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;
// Marks an entity as an Account
struct SelectedParty {
  SelectedParty(fairlanes::AppContext &context);
};

} // namespace fairlanes::ecs::components
