// combatant_component.hpp
#pragma once
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <entt/entt.hpp>

class Combatant : public ftxui::ComponentBase {
 public:
  Combatant(entt::registry& reg, entt::entity e)
      : reg_(reg), entity_(e) {}

  ftxui::Element Render() override {
    using namespace ftxui;

    auto& name = reg_.get<Name>(entity_);
    auto& hp = reg_.get<Health>(entity_);
    auto& level = reg_.get<Level>(entity_);

    float percent = static_cast<float>(hp.current) / hp.max;
    int bar_width = 20;
    int filled = static_cast<int>(bar_width * percent);

    std::string bar = std::string(filled, '#') + std::string(bar_width - filled, '-');

    // Build the HP line element
    Element hp_line = hbox({
      text("HP: "),
      text("[" + bar + "] " + std::to_string(hp.current) + "/" + std::to_string(hp.max)),
    });

    // Add top border labels
    Element border = window(
      hbox({
        text(name.value) | bold,
        filler(),
        text("[" + std::to_string(level.value) + "]") | bold,
      }),
      hp_line
    );

    return border;
  }

 private:
  entt::registry& reg_;
  entt::entity entity_;
};
