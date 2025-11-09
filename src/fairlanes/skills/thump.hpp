// ThumpSystem.hpp
#pragma once
#include <cmath>
#include <random>

#include <entt/entt.hpp>
#include <exprtk.hpp>

struct WeaponDamage {
  int min_damage;
  int max_damage;
  bool one_handed = true;
};

struct Stats {
  int hp;
  int max_hp;
};

class ThumpSystem {
public:
  ThumpSystem();

  // Apply one Thump from attacker->defender, returns damage dealt (int).
  int thump(entt::registry &reg, entt::entity attacker, entt::entity defender);

  // Tunables
  double hit_rate = 0.90;
  double crit_rate = 0.05;
  double skill_mult = 1.0;     // per-skill scalar (start at 1.0)
  double one_handed_mod = 1.0; // start at 1.0; tweak later if needed

private:
  using expr_t = exprtk::expression<double>;
  expr_t expr_;
  exprtk::symbol_table<double> sym_;
  exprtk::parser<double> parser_;

  // bound variables
  double wd_min_, wd_max_;
  double hit_rate_, crit_rate_;
  double u_roll_, u_hit_, u_crit_;
  double skill_mult_, one_handed_mod_;
  double damage_;

  // RNG – swap for PCG if you prefer
  std::mt19937_64 rng_;
  std::uniform_real_distribution<double> uni_{0.0, 1.0};
};
