#include "combat_log.hpp"
#include <algorithm>
#include <ftxui/screen/string.hpp> // to_wstring

using namespace ftxui;

namespace {
CombatLog::StyleMap DefaultStyleMap() {
  return {
      {"info", color(Color::Green)},
      {"warn", color(Color::Yellow)},
      {"error", color(Color::Red) | bold},
      {"hp", color(Color::Red)},
      {"mp", color(Color::Blue)},
      {"name", color(Color::BlueLight)},
      {"xp", color(Color::LightGreen)},
      {"bold", bold},
      {"dim", dim},
  };
}
inline Element stext_utf8(std::string_view s) {
  return text(to_wstring(std::string{s}));
}
} // namespace

CombatLog::CombatLog(int max_height, size_t max_log_size)
    : max_height_(max_height), max_log_size_(max_log_size),
      style_map_(DefaultStyleMap()) {}

void CombatLog::Append(std::string_view line_utf8) {
  Push(ParseStyledLine(line_utf8));
}
void CombatLog::Append(Element el) { Push(std::move(el)); }

void CombatLog::Push(Element el) {
  log_.push_back(std::move(el));
  if (log_.size() > max_log_size_)
    log_.pop_front();
}

void CombatLog::Clear() { log_.clear(); }
void CombatLog::SetStyle(std::string tag, Decorator deco) {
  style_map_[std::move(tag)] = deco;
}
void CombatLog::SetStyleMap(StyleMap map) { style_map_ = std::move(map); }

Element CombatLog::Render() {
  Elements lines;
  const size_t total = log_.size();
  const size_t count = std::min<size_t>(max_height_, total);
  const size_t start = total - count;
  lines.reserve(count);
  for (size_t i = start; i < total; ++i)
    lines.push_back(log_[i]);
  return vbox(std::move(lines)) | vscroll_indicator | frame | border;
}

// ASCII-delimited markup parser on UTF-8; we never slice inside a codepoint.
Element CombatLog::ParseStyledLine(std::string_view line) const {
  Elements parts;
  size_t pos = 0;

  while (pos < line.size()) {
    size_t tag_start = line.find('[', pos);
    if (tag_start == std::string::npos) {
      parts.push_back(stext_utf8(line.substr(pos)));
      break;
    }
    if (tag_start > pos)
      parts.push_back(stext_utf8(line.substr(pos, tag_start - pos)));

    size_t tag_end = line.find("](", tag_start);
    if (tag_end == std::string::npos) {
      parts.push_back(stext_utf8(line.substr(tag_start)));
      break;
    }

    const size_t value_open = tag_end + 2;
    size_t val_end = line.find(')', value_open);
    if (val_end == std::string::npos) {
      parts.push_back(stext_utf8(line.substr(tag_start)));
      break;
    }

    std::string tag{line.substr(tag_start + 1, tag_end - tag_start - 1)};
    std::string value{line.substr(value_open, val_end - value_open)};

    if (auto it = style_map_.find(tag); it != style_map_.end())
      parts.push_back(stext_utf8(value) | it->second);
    else
      parts.push_back(stext_utf8("[" + tag + "](" + value + ")"));

    pos = val_end + 1;
  }
  return hbox(std::move(parts));
}

template <typename... Ds>
void CombatLog::AppendText(std::string_view utf8, Ds... ds) {
  Element el = stext_utf8(utf8);
  ((el = el | ds), ...);
  Push(std::move(el));
}
// keep template body in the header or add explicit instantiations here
