// account.hpp
#pragma once
#include <bitset>
#include <entt/entt.hpp>
#include <memory>
#include <string>
#include <vector>

namespace fairlanes {
class Party;
class AppContext;
class FancyLog;
class RandomHub;

class Account {
public:
  static constexpr std::size_t kMaxParties = 3; // tweak if you want

  Account(AppContext &ctx, std::string name);

  // Create and own a party; returns reference for immediate setup
  Party &create_party(AppContext &ctx, std::string label);

  // Active party
  Party *active_party();
  const Party *active_party() const;
  void set_active_party(std::size_t idx);

  std::vector<std::unique_ptr<Party>> &parties();
  const std::vector<std::unique_ptr<Party>> &parties() const;

  // Unlocks
  bool party_unlocked(std::size_t idx) const {
    return idx < kMaxParties && unlocked_.test(idx);
  }
  void unlock_party(std::size_t idx) {
    if (idx < kMaxParties)
      unlocked_.set(idx, true);
  }

  // Access
  std::size_t party_count() const { return parties_.size(); }
  Party *party(std::size_t idx) {
    return idx < parties_.size() ? parties_[idx].get() : nullptr;
  }

private:
  std::vector<std::unique_ptr<Party>> parties_;
  std::bitset<kMaxParties> unlocked_;
  std::size_t active_idx_ = 0;
  FancyLog &log_;
  RandomHub &rng_;
  std::string name;
};
} // namespace fairlanes
