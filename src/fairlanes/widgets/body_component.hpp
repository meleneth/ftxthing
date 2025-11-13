#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace fairlanes::widgets {
class BodyComponent : public ftxui::ComponentBase {
public:
  bool OnEvent(ftxui::Event e) override;
  ftxui::Element Render() override;
  int counter() const;

  // 🔑 This makes the component a keyboard target.
  bool Focusable() const override;

  std::string party_doing;

private:
  int counter_ = 0;
};

} // namespace fairlanes::widgets