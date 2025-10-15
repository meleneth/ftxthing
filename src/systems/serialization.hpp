#pragma once
// CMake: header-only, just add include path for zpp_bits
#include <cstdint>
#include <string>
#include <vector>
#include <zpp_bits.h>

struct Player {
  std::string name;
  int level{};
  double gold{};
  // zpp_bits can reflect member order automatically; no boilerplate needed.
};

struct SaveV1 {
  uint64_t seed{};
  Player player;
  std::vector<int> inventory_ids;
  template <typename Archive> auto serialize(Archive &ar) {
    return ar(seed, player, inventory_ids);
  }
};

struct SaveV2 { // later addition: difficulty and map seed
  uint64_t seed{};
  Player player;
  std::vector<int> inventory_ids;
  int difficulty{1};
  uint64_t map_seed{};
  template <typename Archive> auto serialize(Archive &ar) {
    return ar(seed, player, inventory_ids, difficulty, map_seed);
  }
};

// --- Writing (to bytes) ---
template <class T> std::vector<std::byte> save_to_bytes(const T &s) {
  std::vector<std::byte> out;
  zpp::bits::out out_archive(out);
  out_archive(s); // serialize
  return out;
}

// --- Reading (from bytes) ---
template <class T>
bool load_from_bytes(const std::vector<std::byte> &in, T &s) {
  zpp::bits::in in_archive(in);
  return (in_archive(s) == zpp::bits::success);
}
