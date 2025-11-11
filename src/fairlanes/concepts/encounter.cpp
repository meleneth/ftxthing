#include "encounter.hpp"
#include "app/app_context.hpp"
using namespace fairlanes::concepts;

Encounter::Encounter(AppContext &ctx, entt::entity party) : party_(party) {
  (void)ctx;
}

void Encounter::tick(float dt) { (void)dt; }