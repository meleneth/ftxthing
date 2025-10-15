#pragma once
#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

class CombatLog : public ftxui::ComponentBase {
public:
  using StyleMap = std::unordered_map<std::string, ftxui::Decorator>;

  explicit CombatLog(int max_height = 10, size_t max_log_size = 512);

  // UTF-8 markup: "[name](Snail) uses [error](Slime Blast) 🔥"
  void Append(std::string_view line_utf8);

  // Prebuilt element:
  void Append(ftxui::Element el);

  // Direct text + decorators (no markup parsing):
  template <typename... Ds> void AppendText(std::string_view utf8, Ds... ds);

  void Clear();
  void SetStyle(std::string tag, ftxui::Decorator deco);
  void SetStyleMap(StyleMap map);

  ftxui::Element Render() override;

private:
  ftxui::Element ParseStyledLine(std::string_view line_utf8) const;
  void Push(ftxui::Element el);

  std::deque<ftxui::Element> log_;
  int max_height_;
  size_t max_log_size_;
  StyleMap style_map_;
};
