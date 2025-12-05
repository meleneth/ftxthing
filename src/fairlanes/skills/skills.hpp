#pragma once
#include <concepts>
#include <variant>

#include "fairlanes/skills/thump.hpp"

using SkillVariant = std::variant<fairlanes::skills::Thump>;
