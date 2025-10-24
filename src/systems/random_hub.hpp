#pragma once

#include <cstdint>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <pcg_random.hpp>

namespace fairlanes {
// Choose your engine here (pcg64_fast is good for games/sims).
using Engine = pcg64;

// ------------------------------ Utilities ------------------------------

constexpr uint64_t fnv1a64(std::string_view s) noexcept {
  constexpr uint64_t kOffset = 14695981039346656037ULL;
  constexpr uint64_t kPrime = 1099511628211ULL;

  uint64_t h = kOffset;
  for (size_t i = 0; i < s.size(); ++i) {
    const unsigned char c = static_cast<unsigned char>(s[i]);
    h ^= c;
    h *= kPrime;
  }
  return h;
}

constexpr uint64_t splitmix64_once(uint64_t x) noexcept {
  x += 0x9E3779B97F4A7C15ULL;
  x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
  x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
  return x ^ (x >> 31);
}

inline uint64_t time_seed_now() {
  using clock = std::chrono::steady_clock;
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          clock::now().time_since_epoch())
          .count());
}

// ------------------------------ RandomStream ------------------------------

class RandomStream {
public:
  RandomStream() = default;
  RandomStream(uint64_t master_seed, uint64_t sequence, std::string key)
      : master_seed_(master_seed), sequence_(sequence), key_(std::move(key)) {}

  // Thread-local engine per (stream key). Deterministic (no thread salt).
  Engine &eng() const {
    struct Slot {
      bool init{false};
      Engine e;
    };
    thread_local std::unordered_map<std::string, Slot> tls;
    auto it = tls.find(key_);
    if (it == tls.end()) {
      Slot s;
      // PCG constructor uses (initstate, initseq). initseq picks the stream.
      s.e = Engine(master_seed_, sequence_);
      s.init = true;
      it = tls.emplace(key_, std::move(s)).first;
    }
    return it->second.e;
  }

  // Core draws
  uint64_t u64() { return std::uniform_int_distribution<uint64_t>()(eng()); }
  uint32_t u32() { return std::uniform_int_distribution<uint32_t>()(eng()); }
  bool bernoulli(double p) { return std::bernoulli_distribution(p)(eng()); }

  template <typename Int> Int uniform_int(Int lo, Int hi_inclusive) {
    return std::uniform_int_distribution<Int>(lo, hi_inclusive)(eng());
  }

  template <typename Real = double> Real uniform_real(Real lo, Real hi) {
    return std::uniform_real_distribution<Real>(lo, hi)(eng());
  }

  template <typename Real = double> Real normal(Real mean, Real stddev) {
    return std::normal_distribution<Real>(mean, stddev)(eng());
  }

  template <typename T> void shuffle(std::vector<T> &v) {
    std::shuffle(v.begin(), v.end(), eng());
  }

  template <typename Range> const auto &pick(const Range &r) {
    const auto n = static_cast<int>(r.size());
    const int i = uniform_int<int>(0, n - 1);
    return r[i];
  }

  // Burn n draws:
  void advance(uint64_t n) {
    for (uint64_t i = 0; i < n; ++i)
      (void)u64();
  }

  uint64_t master_seed() const noexcept { return master_seed_; }
  uint64_t sequence() const noexcept { return sequence_; }
  const std::string &key() const noexcept { return key_; }

private:
  uint64_t master_seed_{0};
  uint64_t sequence_{
      1}; // pcg allows any; constructor handles odd increment internally
  std::string key_{"<unset>"}; // used for TLS slot key
};

// ------------------------------ RandomHub ------------------------------

class RandomHub {
public:
  // If seed == nullopt → time-based; base_sequence is an optional global
  // “stream id”.
  explicit RandomHub(std::optional<uint64_t> seed = std::nullopt,
                     uint64_t base_sequence = 0)
      : master_seed_(seed.value_or(splitmix64_once(time_seed_now()))),
        base_sequence_(base_sequence) {}

  // Stable master seed & base sequence
  uint64_t master_seed() const noexcept { return master_seed_; }
  uint64_t base_sequence() const noexcept { return base_sequence_; }

  // Named stream: sequence selector derived from base_sequence ^ hash(name) ^
  // sub_seq
  RandomStream stream(std::string_view name, uint64_t sub_seq = 0) const {
    const uint64_t seq = mix_seq(base_sequence_, fnv1a64(name), sub_seq);
    return RandomStream(master_seed_, seq, key_for(name, sub_seq));
  }

  // Pure numeric stream (no name)
  RandomStream stream(uint64_t sub_seq) const {
    const uint64_t seq =
        mix_seq(base_sequence_, 0xD1B54A32D192ED03ULL, sub_seq);
    return RandomStream(master_seed_, seq, key_for("<num>", sub_seq));
  }

  // Hierarchical namespaces (party/3/combat, etc.)
  class NS {
  public:
    NS(const RandomHub &hub, std::string prefix)
        : hub_(hub), prefix_(std::move(prefix)) {}
    NS ns(std::string_view child) const {
      return NS{hub_, prefix_ + "/" + std::string(child)};
    }
    NS ns(std::string_view child, uint64_t id) const {
      return NS{hub_,
                prefix_ + "/" + std::string(child) + "/" + std::to_string(id)};
    }
    RandomStream stream(uint64_t sub_seq = 0) const {
      return hub_.stream(prefix_, sub_seq);
    }
    const std::string &prefix() const { return prefix_; }

  private:
    const RandomHub &hub_;
    std::string prefix_;
  };

  NS ns(std::string_view root) const { return NS{*this, std::string(root)}; }

private:
  static uint64_t mix_seq(uint64_t base, uint64_t name_hash, uint64_t sub) {
    // Robust 3-way mix → stream selector
    return splitmix64_once(base ^ (name_hash + 0x9E3779B97F4A7C15ULL) ^
                           (sub * 0xBF58476D1CE4E5B9ULL));
  }

  static std::string key_for(std::string_view name, uint64_t sub) {
    if (sub == 0)
      return std::string(name);
    return std::string(name) + "#" + std::to_string(sub);
  }

  uint64_t master_seed_;
  uint64_t base_sequence_;
};
} // namespace fairlanes