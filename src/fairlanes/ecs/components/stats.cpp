#include <fmt/core.h>

#include "fairlanes/combat/damage.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "stats.hpp"

using fairlanes::ecs::components::Stats;

Stats::Stats(std::string name) : name_(name) {}

bool Stats::is_alive() { return hp_ > 0; }
