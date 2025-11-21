#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "fairlanes/ecs/components/stats.hpp"

template <typename T> struct ComponentBuilder; // primary template

// ---- Components ----

struct Tags {
  std::vector<std::string> values;
};

// Shorthand
using json = nlohmann::json;
using fairlanes::ecs::components::Stats;

// ---- Specializations ----
template <> struct ComponentBuilder<Stats> {
  static Stats defaults() { return Stats{"some name"}; }
  static void apply(Stats &s, const json &j) {
    if (auto it = j.find("hp"); it != j.end())
      s.hp_ = it->get<int>();
    if (auto it = j.find("mp"); it != j.end())
      s.mp_ = it->get<int>();
    // Or: s.hp = j.value("hp", s.hp); s.mp = j.value("mp", s.mp);
  }
};

template <> struct ComponentBuilder<Tags> {
  static Tags defaults() { return Tags{}; }
  static void apply(Tags &t, const json &j) {
    if (auto it = j.find("values"); it != j.end()) {
      t.values = it->get<std::vector<std::string>>();
    }
  }
};
