#include "footer_component.hpp"

void FooterComponent::SetMessage(std::string msg) { msg_ = std::move(msg); }

ftxui::Element FooterComponent::Render() {
  return ftxui::text(msg_) | ftxui::center | ftxui::border;
}