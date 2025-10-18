#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>


class BodyComponent : public ftxui::ComponentBase {
 public:
  bool OnEvent(ftxui::Event e) override;
  ftxui::Element Render() override;
  int counter() const;

 private:
  int counter_ = 0;
};


