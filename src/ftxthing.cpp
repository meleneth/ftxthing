#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  using namespace ftxui;

  // Create the screen
  auto screen = ScreenInteractive::TerminalOutput();

  // Create a component that just displays a centered static box
  Component component = Renderer([] {
    return vbox({
      text("Hello from FTX:UI!") | bold | color(Color::Green),
      filler(),
    }) | border | center;
  });

  // Start the event loop
  screen.Loop(component);

  return 0;
}
