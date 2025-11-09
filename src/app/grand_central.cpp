#include <atomic>
#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "app/app_context.hpp"
#include "entities/entities.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_business.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/systems/tick_party_fsms.hpp"
#include "grand_central.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "systems/random_hub.hpp"
#include "widgets/body_component.hpp"
#include "widgets/console_overlay.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

using namespace fairlanes;

entt::entity GrandCentral::create_account(AppContext &ctx, std::string name) {
  auto e = reg_.create();
  using fairlanes::ecs::components::IsAccount;

  reg_.emplace<IsAccount>(e, ctx, std::move(name));
  account_ids.push_back(e);
  if (selected_account_ == entt::null) {
    selected_account_ = e;
  }
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
  using namespace fairlanes::ecs::components;
  reg_.emplace<PartyMember>(e, ctx, name, party);
  reg_.emplace<TrackXP>(e, ctx, 0);
  reg_.emplace<Stats>(e, ctx, name);
  (void)name;
  return e;
}

GrandCentral::GrandCentral(const AppConfig &cfg)
    : console_(std::make_shared<FancyLog>()),
      root_component_(Make<RootComponent>(console_)),
      seed_(cfg.seed.value_or(static_cast<uint64_t>(std::random_device{}()))),
      random_(std::make_shared<RandomHub>(seed_, cfg.stream)),
      app_context_(AppContext{*console_, reg_, *random_}) {
  using namespace ftxui;

  // UI bits
  console_->append_markup("[name](Snail) uses [error](Slime Blast) 🔥🔥🔥");
  spdlog::debug("GrandCentral ctor: seed={}, stream={}", seed_, cfg.stream);

  for (int i = 1; i <= 8; ++i) {
    const auto acc_name = fmt::format("Account {}", i);
    const auto party_name = fmt::format("Party {}", i);
    const auto member_name = fmt::format("Player{}", i);

    // Create account i
    auto account = create_account(app_context(), acc_name);
    using fairlanes::ecs::components::IsAccount;
    auto &reg = app_context().registry();
    auto &is_account = reg.get<IsAccount>(account);
    auto &account_log = *is_account.log_;
    auto account_specific_app_context =
        AppContext{account_log, reg, app_context().rng()};
    // Create party i in account i
    auto party = create_party_in_account(account_specific_app_context,
                                         party_name, account);

    // Log the join (and optionally account/party creation)
    account_log.append_markup(fmt::format(
        "Created [info]({}) with [emphasis]({}).", acc_name, party_name));

    // Create a single member in that party
    auto character = create_member_in_party(account_specific_app_context,
                                            member_name, party);

    (void)character;
  }
  console_->append_markup("[name](Snail) uses [ability](Slime Blast) 🔥");
}

AppContext &GrandCentral::app_context() { return app_context_; }

inline void GrandCentral::tick_party_fsms(float dt) {
  using fairlanes::systems::TickPartyFSMs;
  (void)dt;
  TickPartyFSMs::commit(reg_);
}

RootComponent *GrandCentral::root_component() {
  return dynamic_cast<RootComponent *>(root_component_.get());
}

void GrandCentral::main_loop() {
  using namespace ftxui;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.SetCursor(Screen::Cursor{.shape = Screen::Cursor::Hidden});

  auto ui = Renderer(root_component_, [&] {
    using clock = std::chrono::steady_clock;
    static auto last = clock::now();
    const auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last).count();
    last = now;

    tick_party_fsms(dt);

    return root_component()->Render();
  });
  using fairlanes::ecs::components::IsAccount;

  ui = CatchEvent(ui, [&](Event e) {
    if (e == Event::Character('q') || e == Event::Escape) {
      screen.Exit();
      return true;
    }
    if (e == ftxui::Event::F1) {
      // Handle F1 key
      spdlog::debug("f1 detected");
      selected_account_ = account_ids[0];
      auto &is_account = reg_.get<IsAccount>(selected_account_);
      console_ = is_account.log_;
      root_component()->console_overlay()->change_console(console_);
      return true;
    }
    if (e == ftxui::Event::F2) {
      // Handle F2 key
      spdlog::debug("f2 detected");
      selected_account_ = account_ids[1];
      auto &is_account = reg_.get<IsAccount>(selected_account_);
      spdlog::debug("console_ was {} and becomes {}", fmt::ptr(console_),
                    fmt::ptr(is_account.log_));

      console_ = is_account.log_;
      root_component()->console_overlay()->change_console(console_);
      return true;
    }
    if (e == ftxui::Event::F3) {
      // Handle F3 key
      spdlog::debug("f3 detected");
      return true;
    }
    if (e == ftxui::Event::F4) {
      // Handle F4 key
      spdlog::debug("f4 detected");
      return true;
    }
    if (e == ftxui::Event::F5) {
      // Handle F5 key
      spdlog::debug("f5 detected");
      return true;
    }
    if (e == ftxui::Event::F6) {
      // Handle F6 key
      spdlog::debug("f6 detected");
      return true;
    }
    if (e == ftxui::Event::F7) {
      // Handle F7 key
      spdlog::debug("f7 detected");
      return true;
    }
    if (e == ftxui::Event::F8) {
      // Handle F8 key
      spdlog::debug("f8 detected");
      return true;
    }
    if (e == Event::Character('`')) {
      root_component()->toggle_console();
      return true;
    }
    if (e == Event::Character('~')) {
      root_component()->set_full_open();
      root_component()->toggle_console();
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

entt::entity GrandCentral::get_account(int id) { return account_ids[id]; }
