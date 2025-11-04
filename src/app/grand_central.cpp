#include <atomic>
#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "app/app_context.hpp"
#include "entities/entities.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "grand_central.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

using namespace fairlanes;

entt::entity GrandCentral::create_account(AppContext &ctx, std::string name) {
  auto e = reg_.create();
  using fairlanes::ecs::components::IsAccount;

  reg_.emplace<IsAccount>(e, ctx, std::move(name));
  return e;
}

entt::entity GrandCentral::create_party_in_account(AppContext &ctx,
                                                   std::string name,
                                                   entt::entity account) {
  auto e = reg_.create();
  using ::fairlanes::ecs::components::IsParty;
  using ::fairlanes::ecs::components::PartyBusiness;

  // spdlog::debug("create: reg={}, party={}", fmt::ptr(&reg_),
  // (uint32_t)entt::to_integral(e));

  reg_.emplace<PartyBusiness>(e, ctx, "idle");
  auto &business = reg_.get<PartyBusiness>(e);
  (void)business;
  reg_.emplace<IsParty>(e, ctx, e, std::move(name), account);

  return e;
}

entt::entity GrandCentral::create_member_in_party(AppContext &ctx,
                                                  std::string name,
                                                  entt::entity party) {
  auto e = reg_.create();
  using fairlanes::ecs::components::PartyMember;
  reg_.emplace<PartyMember>(e, ctx, name, party);
  (void)name;
  return e;
}

GrandCentral::GrandCentral(const AppConfig &cfg) {
  using namespace ftxui;

  // UI bits
  console_ = std::make_shared<FancyLog>();
  root_component_ = Make<RootComponent>(console_);
  console_->append_markup("[name](Snail) uses [error](Slime Blast) 🔥");

  // RNG setup (safe default if seed is absent)
  const uint64_t seed =
      cfg.seed.value_or(static_cast<uint64_t>(std::random_device{}()));
  spdlog::debug("GrandCentral ctor: seed={}, stream={}", seed, cfg.stream);

  random_ = std::make_shared<RandomHub>(seed, cfg.stream);

  // Build a temporary context to wire new subsystems
  AppContext ctx(*console_, reg_, *random_);

  // Parties reference registry/log/rng extracted from ctx; ctx can die after
  // this

  for (int i = 1; i <= 8; ++i) {
    const auto acc_name = fmt::format("Account {}", i);
    const auto party_name = fmt::format("Party {}", i);
    const auto member_name = fmt::format("Player{}", i);

    // Create account i
    auto account = create_account(ctx, acc_name);

    // Create party i in account i
    auto party = create_party_in_account(ctx, party_name, account);

    // Log the join (and optionally account/party creation)
    console_->append_markup(fmt::format(
        "Created [info]({}) with [emphasis]({}).", acc_name, party_name));

    // Create a single member in that party
    auto character = create_member_in_party(ctx, member_name, party);

    (void)character;
  }
  console_->append_markup("[name](Snail) uses [error](Slime Blast) 🔥");
}

inline void GrandCentral::tick_party_fsms(float dt) {
  using fairlanes::ecs::components::IsParty;
  (void)dt;
  auto v = reg_.view<IsParty>();
  for (auto [e, party] : v.each()) {
    party.next();
    (void)e;
  }
}

void GrandCentral::main_loop() {
  using namespace ftxui;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.SetCursor(Screen::Cursor{.shape = Screen::Cursor::Hidden});
  auto root = Make<RootComponent>(console_);

  auto ui = Renderer(root, [&] {
    using clock = std::chrono::steady_clock;
    static auto last = clock::now();
    const auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    tick_party_fsms(dt);
    // TODO: sim.tick(dt);

    return root->Render();
  });

  ui = CatchEvent(ui, [&](Event e) {
    if (e == Event::Character('q') || e == Event::Escape) {
      screen.Exit();
      return true;
    }
    if (e == Event::Character('`')) {
      root->toggle_console();
      return true;
    }
    if (e == Event::Character('~')) {
      root->set_full_open();
      root->toggle_console();
      return true;
    }
    if (e == Event::ArrowLeft) {
      // ...
      return true;
    }
    if (e == Event::ArrowRight) {
      // ...
      return true;
    }
    return false;
  });

  // wake UI at ~60Hz
  std::atomic<bool> running = true;
  std::thread ticker([&] {
    using namespace std::chrono_literals;
    while (running) {
      screen.PostEvent(Event::Custom); // kick a rerender (~60 Hz)
      std::this_thread::sleep_for(16ms);
    }
  });

  screen.Loop(ui);
  running = false;
  ticker.join();
}