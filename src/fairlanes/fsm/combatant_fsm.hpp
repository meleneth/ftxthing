#pragma once

#include <boost/sml.hpp>
#include <entt/entt.hpp>

#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/turn_timer.hpp"

namespace fairlanes::combat {

namespace sml = boost::sml;
using fairlanes::context::EntityCtx;
using fairlanes::ecs::components::Encounter;
using fairlanes::ecs::components::TurnTimer;

// ----- Events -----

struct Tick {
  float dt{};
};

struct BeginActing {};
struct FinishActing {};
struct Kill {};

// ----- Context -----

struct CombatantCtx {
  entt::registry &reg;
  EntityCtx entity_ctx; // optional; if you want logging/rng
  entt::entity encounter{};
  entt::entity self{};
};

// ----- State machine definition -----

struct CombatantSM {
  using Self = CombatantSM;

  CombatantCtx ctx;

  // Guards
  auto is_alive() const {
    return [this](const auto &) {
      auto &stats = ctx.reg.get<fairlanes::ecs::components::Stats>(ctx.self);
      return stats.hp_ > 0; // or hp_ > 0.0f
    };
  }

  auto gauge_full() const {
    return [this](const Tick &ev) {
      auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
      return timer.gauge_ + timer.speed_ * ev.dt >= 1.0f;
    };
  }

  // Actions
  void accumulate(const Tick &ev) {
    auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
    timer.gauge_ += timer.speed_ * ev.dt;
    if (timer.gauge_ > 1.0f)
      timer.gauge_ = 1.0f;
    timer.state_ = TurnTimer::State::Charging;
  }

  void enqueue_ready(const Tick &) {
    auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
    timer.gauge_ = 1.0f;
    timer.state_ = TurnTimer::State::Queued;

    auto &enc = ctx.reg.get<Encounter>(ctx.encounter);
    fairlanes::ecs::components::ReadyToActEvent ev{ctx.self};
    enc.event_queue_.enqueue(
        fairlanes::ecs::components::CombatEventId::ReadyToAct, &ev);
  }

  void on_begin_acting(const BeginActing &) {
    auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
    timer.state_ = TurnTimer::State::Acting;
    timer.gauge_ = 0.0f;
  }

  void on_finish_acting(const FinishActing &) {
    auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
    if (timer.state_ != TurnTimer::State::Disabled) {
      timer.state_ = TurnTimer::State::Charging;
      timer.gauge_ = 0.0f;
    }
  }

  void on_kill(const Kill &) {
    auto &timer = ctx.reg.get<TurnTimer>(ctx.self);
    timer.state_ = TurnTimer::State::Disabled;
  }

  // Transition table
  auto operator()() const {
    using namespace boost::sml;
    using namespace boost::sml::literals;

    return make_transition_table(
        // CHARGING: can only advance by Tick; no queuing yet.
        *"Charging"_s + event<Tick>[is_alive() && !gauge_full()] /
                            &Self::accumulate = "Charging"_s,

        "Charging"_s + event<Tick>[is_alive() && gauge_full()] /
                           &Self::enqueue_ready = "Queued"_s,

        // QUEUED: no Tick transitions -> no accumulation.
        "Queued"_s + event<BeginActing> / &Self::on_begin_acting = "Acting"_s,

        // ACTING: waits for animation to finish.
        "Acting"_s + event<FinishActing> / &Self::on_finish_acting =
            "Charging"_s,

        // Death from anywhere.
        "Charging"_s + event<Kill> / &Self::on_kill = "Disabled"_s,
        "Queued"_s + event<Kill> / &Self::on_kill = "Disabled"_s,
        "Acting"_s + event<Kill> / &Self::on_kill = "Disabled"_s);
  }
};

} // namespace fairlanes::combat

namespace fairlanes::ecs::components {

struct CombatantFSM {
  boost::sml::sm<fairlanes::combat::CombatantSM> sm;

  // convenience ctor
  explicit CombatantFSM(const fairlanes::combat::CombatantCtx &ctx) : sm{ctx} {}
};

} // namespace fairlanes::ecs::components
