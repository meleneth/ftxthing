// combatant_component.hpp
#pragma once
#include <entt/entt.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

class Combatant : public ftxui::ComponentBase {
public:
  Combatant(entt::registry &reg_, entt::entity entity_)
      : reg(reg_), entity(entity_) {}

  ftxui::Element Render() override {
    using namespace ftxui;

    auto &name = reg.get<Name>(entity);
    auto &hp = reg.get<Health>(entity);
    auto &level = reg.get<Level>(entity);

    float percent = static_cast<float>(hp.current) / hp.max;
    int bar_width = 20;
    int filled = static_cast<int>(bar_width * percent);

    std::string bar =
        std::string(filled, '#') + std::string(bar_width - filled, '-');

    // Build the HP line element
    Element hp_line = hbox({
        text("HP: "),
        text("[" + bar + "] " + std::to_string(hp.current) + "/" +
             std::to_string(hp.max)),
    });

    // Add top border labels
    // clang-format off
    Element border = window(
      hbox({
        text(name.value) | ftxui::color(ftxui::Color::BlueLight),
        filler(),
        text("[" + std::to_string(level.value) + "]") | bold,
      }),
      hp_line
    );
    // clang-format on

    return border;
  }

private:
  entt::registry &reg;
  entt::entity entity;
};
