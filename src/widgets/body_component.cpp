#include "body_component.hpp"
#include "combatant.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace fairlanes;

bool BodyComponent::OnEvent(ftxui::Event e) {
  if (e == ftxui::Event::Character('a')) {
    counter_++;
    return true;
  }
  if (e == ftxui::Event::Character('z')) {
    counter_--;
    return true;
  }
  return false;
}

ftxui::Element BodyComponent::Render() {
  return ftxui::text("Party doing: " + party_doing) | ftxui::border |
         ftxui::center;
  // return ftxui::text("Body count: " + std::to_string(counter_)) |
  //       ftxui::border | ftxui::center;
}

int BodyComponent::counter() const { return counter_; }

bool BodyComponent::Focusable() const { return true; }
