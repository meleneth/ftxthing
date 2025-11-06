#include <fmt/core.h>

#include "app/app_context.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "stats.hpp"
#include "widgets/fancy_log.hpp"

using fairlanes::ecs::components::Stats;

Stats::Stats(AppContext &context, std::string name) : name_(name) {
  (void)context;
}
