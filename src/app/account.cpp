// account.cpp
#include "account.hpp"
#include "app_context.hpp"
#include "entities/party.hpp"
#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"

using namespace fairlanes;

Account::Account(AppContext &ctx, std::string name)
    : log_(ctx.log()), rng_(ctx.rng()), name(std::move(name)) {
  unlocked_.reset();
  unlocked_.set(0, true); // first party unlocked by default
}

std::vector<std::unique_ptr<Party>> &Account::parties() { return parties_; }

const std::vector<std::unique_ptr<Party>> &Account::parties() const {
  return parties_;
}

Party &Account::create_party(AppContext &ctx, std::string label) {
  parties_.push_back(std::make_unique<Party>(ctx, std::move(label)));
  return *parties_.back();
}

Party *Account::active_party() {
  return active_idx_ < parties_.size() ? parties_[active_idx_].get() : nullptr;
}

const Party *Account::active_party() const {
  return active_idx_ < parties_.size() ? parties_[active_idx_].get() : nullptr;
}

void Account::set_active_party(std::size_t idx) {
  if (idx < parties_.size() && party_unlocked(idx))
    active_idx_ = idx;
}
