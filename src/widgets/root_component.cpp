#include "root_component.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "body_component.hpp"
#include "console_overlay.hpp"
#include "footer_component.hpp"

RootComponent::RootComponent() {
  using namespace ftxui;
  header_ = Renderer([] { return text("Header") | center | bold | border; });
  body_ = Make<BodyComponent>();
  footer_ = Make<FooterComponent>();
  console_ = Make<ConsoleOverlay>();

  auto overlay = std::dynamic_pointer_cast<ConsoleOverlay>(console_);

  overlay->push_line("some line");
  overlay->push_line("another line");

  container_ = Container::Vertical({header_, body_, footer_});
  Add(container_);
  Add(console_);

  container_->SetActiveChild(body_);

  // Example: catch events at root level to update footer
  Add(CatchEvent(container_, [&](Event e) {
    auto b = std::dynamic_pointer_cast<BodyComponent>(body_);
    auto f = std::dynamic_pointer_cast<FooterComponent>(footer_);
    f->SetMessage("Body count = " + std::to_string(b->counter()));
    (void)e;
    return false;
  }));
}

void RootComponent::toggle_console() {
  auto overlay = std::dynamic_pointer_cast<ConsoleOverlay>(console_);
  overlay->toggle();
  if (overlay->should_show()) {
    container_->SetActiveChild(console_);
    overlay->FocusInput(); // ensure cursor lands in Input
  } else {
    container_->SetActiveChild(body_);
  }
}

void RootComponent::set_full_open() {
  auto overlay = std::dynamic_pointer_cast<ConsoleOverlay>(console_);
  overlay->set_full_open();
}

ftxui::Element RootComponent::Render() {
  using namespace ftxui;
  auto overlay = std::dynamic_pointer_cast<ConsoleOverlay>(console_);
  overlay->tick();

  auto content = vbox({
      header_->Render() | flex,
      body_->Render() | flex_grow,
      footer_->Render() | flex,
  });
  if (overlay->should_show()) {
    return dbox({content, console_->Render()});
  }
  return content;
}
