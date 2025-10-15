// ui_fsm.hpp
#pragma once
#include <boost/sml.hpp>
#include <functional>

namespace sml = boost::sml;

struct ev_tab_next {};
struct ev_tab_prev {};
struct ev_open_party {
  int party_id;
};
struct ev_open_override {
  int screen_kind;
};
struct ev_close_override {};
struct ev_toggle_console {};
struct ev_esc {};

struct UiCtx {
  // data
  int current_index = 0;
  int screen_count = 0;

  // view controls
  std::function<void(int)> show_screen;       // swap root screen by index
  std::function<void(int)> open_party_screen; // swap to specific party detail
  std::function<void(int)> push_override; // push overlay/fullscreen override
  std::function<void()> pop_override;     // pop override
  std::function<void(bool)> set_console_open; // toggle console overlay
  std::function<bool()> is_console_open;      // read console state
  std::function<bool()> has_override;         // read override stack
};

struct UiMachine {
  UiCtx &c;

  // ---- guards ----
  auto has_screens() const {
    return [this] { return c.screen_count > 0; };
  }
  auto console_closed() const {
    return [this] { return !c.is_console_open || !c.is_console_open(); };
  }
  auto console_open() const {
    return [this] { return c.is_console_open && c.is_console_open(); };
  }
  auto overlay_present() const {
    return [this] { return c.has_override && c.has_override(); };
  }

  // ---- tiny helpers ----
  void show_current() const {
    if (c.show_screen)
      c.show_screen(c.current_index);
  }

  // ---- actions ----
  auto next_screen() const {
    return [this] {
      if (c.screen_count <= 0)
        return;
      c.current_index = (c.current_index + 1) % c.screen_count;
      show_current();
    };
  }
  auto prev_screen() const {
    return [this] {
      if (c.screen_count <= 0)
        return;
      c.current_index = (c.current_index + c.screen_count - 1) % c.screen_count;
      show_current();
    };
  }
  auto open_party() const {
    return [this](const ev_open_party &e) {
      if (c.open_party_screen)
        c.open_party_screen(e.party_id);
    };
  }
  auto open_override() const {
    return [this](const ev_open_override &e) {
      if (c.push_override)
        c.push_override(e.screen_kind);
    };
  }
  auto toggle_console() const {
    return [this] {
      if (!c.set_console_open || !c.is_console_open)
        return;
      c.set_console_open(!c.is_console_open());
    };
  }
  auto close_one() const {
    return [this] {
      if (c.has_override && c.has_override()) {
        if (c.pop_override)
          c.pop_override();
        return;
      }
      if (c.is_console_open && c.is_console_open()) {
        if (c.set_console_open)
          c.set_console_open(false);
        return;
      }
      // nothing to close
    };
  }

  // single logical state
  struct Active {};

  auto operator()() const {
    using namespace sml;
    return make_transition_table(
        *state<Active> + event<ev_tab_next>[has_screens() && console_closed()] /
                             next_screen() = state<Active>,
        state<Active> + event<ev_tab_prev>[has_screens() && console_closed()] /
                            prev_screen() = state<Active>,
        state<Active> + event<ev_open_party> / open_party() = state<Active>,
        state<Active> + event<ev_open_override> / open_override() =
            state<Active>,
        state<Active> + event<ev_toggle_console> / toggle_console() =
            state<Active>,
        state<Active> + event<ev_esc>[console_open() || overlay_present()] /
                            close_one() = state<Active>);
  }
};
