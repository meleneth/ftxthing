#include "body_component.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>


bool BodyComponent::OnEvent(ftxui::Event e)
{
  if (e == ftxui::Event::Character('a')) { counter_++; return true; }
  if (e == ftxui::Event::Character('z')) { counter_--; return true; }
  return false;
}

ftxui::Element BodyComponent::Render()
{
  return ftxui::text("Body count: " + std::to_string(counter_)) | ftxui::border | ftxui::center;
}

int BodyComponent::counter() const
{
  return counter_;
}