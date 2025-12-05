// skills/learnable_skill.hpp
#pragma once

#include <concepts>
#include <entt/entt.hpp>

#include "fairlanes/context/entity_ctx.hpp"

namespace fairlanes::skills {

template <typename T>
concept LearnableSkill = requires(T skill, fairlanes::context::EntityCtx &ctx) {
  // member function
  { skill.learn_skill(e) } -> std::same_as<void>;
};

} // namespace fairlanes::skills
