#pragma once
#include <entt/entt.hpp>
#include <vector>
namespace fairlanes::fsm {
class PartyLoopCtx;
}

namespace fairlanes::concepts {

class EncounterBuilder {
public:
  static void thump_it_out(fairlanes::fsm::PartyLoopCtx &ctx);
};

} // namespace fairlanes::concepts