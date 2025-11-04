// src/ecs/party_fsm_component.hpp
#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/fsm/party_loop.hpp" // defines PartyLoop, PartyLoopCtx, NextEvent

namespace sml = boost::sml;

namespace fairlanes::ecs::components {

using fairlanes::fsm::NextEvent;
using fairlanes::fsm::PartyLoop;
using fairlanes::fsm::PartyLoopCtx;

struct PartyFSM {
  PartyLoopCtx ctx_;
  sml::sm<PartyLoop> sm_;

  // Store a real ctx so deps(ctx_) is always a valid lvalue reference
  PartyFSM(PartyLoopCtx &context) : ctx_{context}, sm_{PartyLoop{}, ctx_} {};

  void next() { sm_.process_event(NextEvent{}); };

  PartyFSM(PartyFSM &&) noexcept = default;
  PartyFSM &operator=(PartyFSM &&) noexcept = default;
  PartyFSM(const PartyFSM &) = delete;
  PartyFSM &operator=(const PartyFSM &) = delete;
};

} // namespace fairlanes::ecs::components
