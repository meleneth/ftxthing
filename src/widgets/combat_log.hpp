#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <deque>
#include <string>
#include <unordered_map>

class CombatLog : public ftxui::ComponentBase {
public:
  using StyleMap = std::unordered_map<std::string, ftxui::Decorator>;

  explicit CombatLog(int max_height = 10, size_t max_log_size = 512);

  // Append UTF-8 text with markup like: [name](Snail) uses [error](Slime Blast)
  void Append(const std::string& line_utf8);

  // Append a prebuilt FTXUI element (icons, bars, etc.)
  void Append(ftxui::Element el);

  void Append(std::wstring_view w);

  void Clear();

  // Optional: customize or replace the style map at runtime
  void SetStyle(std::string tag, ftxui::Decorator deco);
  void SetStyleMap(StyleMap map);

  ftxui::Element Render() override;

private:
  ftxui::Element ParseStyledLine(const std::string& line_utf8) const;
  void Push(ftxui::Element el);

  std::deque<ftxui::Element> log_;  // already-parsed, render-ready
  int max_height_;
  size_t max_log_size_;
  StyleMap style_map_;
};
