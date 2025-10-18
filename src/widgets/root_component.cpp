#include"root_component.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "body_component.hpp"
#include "footer_component.hpp"


RootComponent::RootComponent() {
  using namespace ftxui;
  header_ = Renderer([] { return text("Header") | center | bold | border; });
  body_   = Make<BodyComponent>();
  footer_ = Make<FooterComponent>();

  auto container = Container::Vertical({ header_, body_, footer_ });
  Add(container);

  container->SetActiveChild(body_);
  
  // Example: catch events at root level to update footer
  Add(CatchEvent(container, [&](Event e) {
    auto b = std::dynamic_pointer_cast<BodyComponent>(body_);
    auto f = std::dynamic_pointer_cast<FooterComponent>(footer_);
    f->SetMessage("Body count = " + std::to_string(b->counter()));
    (void)e;
    return false;
  }));
}

ftxui::Element RootComponent::Render() {
  using namespace ftxui;
  return vbox({
    header_->Render() | flex,
    body_->Render()   | flex_grow,
    footer_->Render() | flex,
  });
}
