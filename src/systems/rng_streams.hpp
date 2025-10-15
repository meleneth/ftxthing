#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <pcg_random.hpp>
#include <random>

enum class RngStream : uint8_t { Combat, Loot, AI, World, COUNT };

// SplitMix64 (good seed mixer)
static inline uint64_t splitmix64(uint64_t &x) {
  uint64_t z = (x += 0x9E3779B97F4A7C15ull);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

// PCG64 has 128-bit state & 128-bit stream selector (“increment”); we’ll derive
// both
struct Pcg64Streams {
  using Engine = pcg64; // 64-bit output, small fast engine
  std::array<Engine, static_cast<size_t>(RngStream::COUNT)> eng{};
  uint64_t master_seed{};

  explicit Pcg64Streams(uint64_t seed = 0xDEADBEEFCAFEBABEull) { reseed(seed); }

  void reseed(uint64_t seed) {
    master_seed = seed;
    uint64_t x = seed;
    for (size_t i = 0; i < eng.size(); ++i) {
      // Derive 256 bits: 128 for state, 128 for stream (increment must be odd)
      uint64_t s0 = splitmix64(x);
      uint64_t s1 = splitmix64(x);
      uint64_t inc0 = splitmix64(x) | 1ull; // ensure odd
      uint64_t inc1 = splitmix64(x) | 1ull;

      pcg64::state_type state{s0, s1};
      pcg64::state_type seq{inc0, inc1}; // “sequence”/increment selector
      eng[i] = Engine(state, seq);
      // Advance a bit to decorrelate from seeding edge-cases:
      eng[i].discard(16);
    }
  }

  Engine &get(RngStream s) { return eng[static_cast<size_t>(s)]; }

  // Helpers
  bool chance(RngStream s, double p) {
    assert(p >= 0.0 && p <= 1.0);
    std::bernoulli_distribution d(p);
    return d(get(s));
  }
  uint64_t u64(RngStream s, uint64_t lo, uint64_t hi_inclusive) {
    std::uniform_int_distribution<uint64_t> d(lo, hi_inclusive);
    return d(get(s));
  }
  double u01(RngStream s) {
    // 53-bit precision in [0,1)
    return std::generate_canonical<double, 53>(get(s));
  }
};
