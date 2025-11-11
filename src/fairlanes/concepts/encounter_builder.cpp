#include "encounter_builder.hpp"
#include "app/app_context.hpp"
#include "encounter.hpp"

namespace fairlanes::concepts {
using fairlanes::AppContext;

Encounter EncounterBuilder::thump_it_out(AppContext &ctx, entt::entity party) {
  Encounter encounter{ctx, party};
  return encounter;
}

} // namespace fairlanes::concepts