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
    auto title = text("Battle UI") | bold | center | border;

    auto combatant_box = 
      hbox({
        filler(),
        combatant_list->Render()
        | size(WIDTH, LESS_THAN, 40)
        | border,
        filler(),
      }) | flex;

    auto log_box = log->Render() | size(HEIGHT, EQUAL, log_height);

    return vbox({
      title,
      combatant_box,
      filler(),
      log_box
    }) | flex;
  });
}


void BattleScreen::Run() {
  screen.Loop(root);
}

void BattleScreen::StartDropInAnimations() {
  /*
  using namespace std::chrono;

  for (size_t i = 0; i < player_offsets_x.size(); ++i) {
    animator.Add(Animation{
      .duration = 500ms,
      .tick_interval = 16ms,
      .on_tick = [this, i](float t) {
        player_offsets_x[i] = static_cast<int>(Lerp(-20, 0, t));
        PostEvent(ftxui::Event::Custom);
      }
    });
  }
*/
}
