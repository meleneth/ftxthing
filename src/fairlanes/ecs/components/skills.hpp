#pragma once
#include <entt/entt.hpp>

#include "fairlanes/skills/skills.hpp"

namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::ecs::components {

struct Skills {
  std::vector<SkillVariant> slots;
  size_t max_skills{5};

  void seal_skills() {
    max_skills = slots.size();
  } // prevent further skill learning

  bool can_learn_skill() {
    // FIXME can't learn skills with this
    return false;
  }

  /*void use_skill(AttackCtx &ctx, SkillVariant &sv) {
    std::visit([&](auto &skill) { skill.use_skill(ctx); }, sv);
  }*/
};

} // namespace fairlanes::ecs::components
