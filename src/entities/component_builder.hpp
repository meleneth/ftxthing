#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

template <typename T> struct ComponentBuilder;  // primary template

// ---- Components ----
struct Stats { int hp = 10; int mp = 0; };
struct Tags  { std::vector<std::string> values; };

// Shorthand
using json = nlohmann::json;

// ---- Specializations ----
template <>
struct ComponentBuilder<Stats> {
  static Stats defaults() { return Stats{10, 0}; }  // no designated inits
  static void apply(Stats& s, const json& j) {
    if (auto it = j.find("hp"); it != j.end()) s.hp = it->get<int>();
    if (auto it = j.find("mp"); it != j.end()) s.mp = it->get<int>();
    // Or: s.hp = j.value("hp", s.hp); s.mp = j.value("mp", s.mp);
  }
};

template <>
struct ComponentBuilder<Tags> {
  static Tags defaults() { return Tags{}; }
  static void apply(Tags& t, const json& j) {
    if (auto it = j.find("values"); it != j.end()) {
      t.values = it->get<std::vector<std::string>>();
    }
  }
};
