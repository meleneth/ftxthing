#include "thump.hpp"

ThumpSystem::ThumpSystem() : rng_(std::random_device{}()) {
  // Bind variables to the symbol table
  sym_.add_variable("wd_min", wd_min_);
  sym_.add_variable("wd_max", wd_max_);
  sym_.add_variable("hit_rate", hit_rate_);
  sym_.add_variable("crit_rate", crit_rate_);
  sym_.add_variable("u_roll", u_roll_);
  sym_.add_variable("u_hit", u_hit_);
  sym_.add_variable("u_crit", u_crit_);
  sym_.add_variable("skill_mult", skill_mult_);
  sym_.add_variable("one_handed_mod", one_handed_mod_);
  sym_.add_variable("damage", damage_);
  sym_.add_constants();

  expr_.register_symbol_table(sym_);

  const std::string expr_src = "base := (wd_min + (wd_max - wd_min) * u_roll) "
                               "* skill_mult * one_handed_mod;"
                               "hit  := (u_hit  < hit_rate);"
                               "crit := (u_crit < crit_rate);"
                               "damage := hit ? (base * (1 + crit)) : 0;";

  if (!parser_.compile(expr_src, expr_)) {
    throw std::runtime_error("ExprTk compile failed for Thump");
  }
}

int ThumpSystem::thump(entt::registry &reg, entt::entity attacker,
                       entt::entity defender) {
  // Grab weapon range (you can derive this differently if needed)
  const auto &w = reg.get<WeaponDamage>(attacker);
  auto &dst = reg.get<Stats>(defender);

  // Load per-attack inputs
  wd_min_ = static_cast<double>(w.min_damage);
  wd_max_ = static_cast<double>(w.max_damage);
  hit_rate_ = hit_rate;
  crit_rate_ = crit_rate;
  u_roll_ = uni_(rng_);
  u_hit_ = uni_(rng_);
  u_crit_ = uni_(rng_);

  // Modifiers
  skill_mult_ = skill_mult;
  one_handed_mod_ = (w.one_handed ? one_handed_mod : 1.0);

  // Evaluate
  double dealt = expr_.value();

  // Commit to defender (round however you like)
  int dmg = static_cast<int>(std::floor(dealt + 0.5));
  dmg = std::max(0, std::min(dmg, dst.hp));
  dst.hp -= dmg;

  return dmg;
}
