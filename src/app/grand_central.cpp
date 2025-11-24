#include <atomic>
#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include <tracy/Tracy.hpp>

#include "entities/entities.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_business.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/selected_account.hpp"
#include "fairlanes/ecs/components/selected_character.hpp"
#include "fairlanes/ecs/components/selected_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/systems/take_damage.hpp"
#include "fairlanes/systems/tick_party_fsms.hpp"
#include "fairlanes/unique_tag.hpp"
#include "fairlanes/widgets/body_component.hpp"
#include "fairlanes/widgets/combatant.hpp"
#include "fairlanes/widgets/console_overlay.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "fairlanes/widgets/root_component.hpp"
#include "grand_central.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "systems/random_hub.hpp"

using namespace fairlanes;

entt::entity GrandCentral::create_account(std::string name) {
  auto e = reg_.create();
  using fairlanes::ecs::components::IsAccount;

  reg_.emplace<IsAccount>(e, ctx_, std::move(name), e);
  account_ids.push_back(e);
  if (selected_account_ == entt::null) {
    selected_account_ = e;
  }
  return e;
}

void GrandCentral::switch_account(std::size_t idx) {
  using IsAccount = fairlanes::ecs::components::IsAccount;
  selected_account_ = account_ids[idx];
  auto &is_account = reg_.get<IsAccount>(selected_account_);
  /* spdlog::debug("console_ was {} and becomes {}", fmt::ptr(console_),
                 fmt::ptr(is_account.log_));
 */
  selected_console_ = is_account.ctx_.log_.get();
  root_component()->change_body_component(ctx_, selected_party_);
  root_component()->change_console(selected_console_);
}

entt::entity GrandCentral::create_party_in_account(std::string name,
                                                   entt::entity account) {
  auto &account_component =
      reg_.get<fairlanes::ecs::components::IsAccount>(account);

  auto e = reg_.create();
  using IsParty = fairlanes::ecs::components::IsParty;
  using PartyBusiness = fairlanes::ecs::components::PartyBusiness;

  // spdlog::debug("create: reg={}, party={}", fmt::ptr(&reg_),
  // (uint32_t)entt::to_integral(e));

  reg_.emplace<PartyBusiness>(e, "idle");
  auto &business = reg_.get<PartyBusiness>(e);
  (void)business;
  reg_.emplace<IsParty>(e, account_component.ctx_.entity_context(e),
                        std::move(name), account);

  return e;
}

entt::entity GrandCentral::create_member_in_party(std::string name,
                                                  entt::entity party) {

  auto &p = reg_.get<fairlanes::ecs::components::IsParty>(party);
  auto e = reg_.create();
  reg_.emplace<fairlanes::ecs::components::PartyMember>(
      e, p.ctx_.entity_context(e), name, party);
  reg_.emplace<fairlanes::ecs::components::TrackXP>(e, p.ctx_.entity_context(e),
                                                    0);
  reg_.emplace<fairlanes::ecs::components::Stats>(e, name);
  return e;
}

GrandCentral::GrandCentral(const AppConfig &cfg)
    : console_(std::make_unique<FancyLog>()), selected_console_(console_.get()),
      root_component_(Make<RootComponent>(console_.get())),
      seed_(cfg.seed.value_or(static_cast<uint64_t>(std::random_device{}()))),
      random_(std::make_shared<RandomHub>(seed_, cfg.stream)),
      ctx_(fairlanes::context::AppCtx{reg_, *random_}) {
  using namespace ftxui;
  ZoneScopedN("Startup");

  // UI bits
  console_->append_markup("[name](Snail) uses [error](Slime Blast) "
                          "[yellow](🔥)[orange](🔥)[red](🔥)");
  spdlog::debug("GrandCentral ctor: seed={}, stream={}", seed_, cfg.stream);
}
void GrandCentral::create_initial_accounts() {
  for (int i = 1; i <= 8; ++i) {
    const auto acc_name = fmt::format("Account {}", i);

    // Create account i
    auto account = create_account(acc_name);
    if (selected_account_ == entt::null) {
      selected_account_ = account;
      set_unique_tag<fairlanes::ecs::components::SelectedAccount>(reg_, account,
                                                                  ctx_);
    }
    auto &is_account =
        ctx_.reg_.get<fairlanes::ecs::components::IsAccount>(account);

    for (int party_no = 1; party_no <= 5; ++party_no) {
      const auto party_name = fmt::format("Party {}.{}", i, party_no);

      // Create party i in account i
      auto party = create_party_in_account(party_name, account);
      if (selected_party_ == entt::null) {
        selected_party_ = party;
        set_unique_tag<fairlanes::ecs::components::SelectedParty>(
            reg_, selected_party_);
      }
      // Log the join (and optionally account/party creation)
      is_account.ctx_.log_->append_markup(fmt::format(
          "Created [info]({}) with [emphasis]({}).", acc_name, party_name));
      for (int party_member_no = 1; party_member_no <= 5; ++party_member_no) {

        const auto member_name =
            fmt::format("Player{}.{}.{}", i, party_no, party_member_no);

        // Create a single member in that party
        auto character = create_member_in_party(member_name, party);
        if (selected_character_ == entt::null) {
          selected_character_ = character;
          set_unique_tag<fairlanes::ecs::components::SelectedCharacter>(
              reg_, selected_character_);
        }
        (void)character;
      }
    }
  }
  console_->append_markup(
      "[name](Snail) uses [ability](Slime Blast) [bravo](🔥)");
  fairlanes::ecs::components::install_encounter_hooks(reg_);
  root_component()->change_body_component(ctx_, selected_party_);
}

inline void GrandCentral::tick_party_fsms(float dt) {
  (void)dt;
  ZoneScoped;
  fairlanes::systems::TickPartyFSMs::commit(reg_);
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

  ui = CatchEvent(ui, [&](Event e) {
    if (e == Event::Character('q') || e == Event::Escape) {
      screen.Exit();
      return true;
    }
    if (e == ftxui::Event::F1) {
      switch_account(0);
      return true;
    }
    if (e == ftxui::Event::F2) {
      switch_account(1);
      return true;
    }
    if (e == ftxui::Event::F3) {
      switch_account(2);
      return true;
    }
    if (e == ftxui::Event::F4) {
      switch_account(3);
      return true;
    }
    if (e == ftxui::Event::F5) {
      switch_account(4);
      return true;
    }
    if (e == ftxui::Event::F6) {
      switch_account(5);
      return true;
    }
    if (e == ftxui::Event::F7) {
      switch_account(6);
      return true;
    }
    if (e == ftxui::Event::F8) {
      switch_account(7);
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
      // TODO 16
      ZoneScopedN("GameTick");
      //      std::this_thread::sleep_for(16ms);

      std::this_thread::sleep_for(250ms);
    }
  });

  screen.Loop(ui);
  running = false;
  ticker.join();
}

entt::entity GrandCentral::get_account(int id) { return account_ids[id]; }
