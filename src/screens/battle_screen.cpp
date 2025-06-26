#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <battle_screen.hpp>
#include <combat_log.hpp>  // the new widget
#include <entity_types.hpp>
#include <combatant.hpp>

using namespace ftxui;

BattleScreen::BattleScreen(entt::registry &combatants_)
    : screen(ScreenInteractive::Fullscreen()),
      combatants(combatants_),
      log(std::make_shared<CombatLog>(log_height)),
      combatant_list(Container::Vertical({}))
{
  log->Append("[info](Battle begins)");
  log->Append("[warn](Enemies approaching)");
  log->Append("[name](Snail) uses [error](Slime Blast)");

  // Generate combatant views
  combatants.view<Name, Health, Level>().each([&](entt::entity e, Name&, Health&, Level&) {
      auto comp = Make<Combatant>(combatants, e);
      combatant_components.push_back(comp);
      combatant_list->Add(comp);
  });

  // Final layout
  root = Renderer([=, this] {
    return vbox({
      text("Battle UI") | bold | center | border,
      hbox({
        combatant_list->Render()
        | size(WIDTH, LESS_THAN, 40)
        | border,
          }),
      vbox({
        log->Render() | size(HEIGHT, EQUAL, log_height),
      }) | flex,
    });
  });
}


void BattleScreen::Run() {
  screen.Loop(root);
}
