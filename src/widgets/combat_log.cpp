#include <algorithm>
#include <ftxui/screen/string.hpp>   // to_wstring
#include "combat_log.hpp"

using namespace ftxui;

namespace {
// Default styles (tweak as you like)
CombatLog::StyleMap DefaultStyleMap() {
  return {
    {"info",  color(Color::Green)},
    {"warn",  color(Color::Yellow)},
    {"error", color(Color::Red) | bold},
    {"hp",    color(Color::Red)},
    {"mp",    color(Color::Blue)},
    {"name",  color(Color::BlueLight)},
    {"xp",    color(Color::LightGreen)},
    {"bold",  bold},
    {"dim",   dim},
  };
}

// convenience: UTF-8 → Element
static inline Element stext(std::string_view s) {
  return text(to_wstring(std::string{s}));
}
} // namespace

CombatLog::CombatLog(int max_height, size_t max_log_size)
  : max_height_(max_height),
    max_log_size_(max_log_size),
    style_map_(DefaultStyleMap()) {}

// Parse once on append, store Element
void CombatLog::Append(const std::string& line_utf8) {
  Push(ParseStyledLine(line_utf8));
}

void CombatLog::Append(Element el) {
  Push(std::move(el));
}

void CombatLog::Append(std::wstring_view w) {
  Append(ftxui::text(std::wstring{w}));  // goes through your Element path
}

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

// --- Markup parser ---
// Markup: [tag](value) where tag is ASCII; value is UTF-8.
// We only slice on ASCII tokens, so we won’t split multi-byte sequences.
Element CombatLog::ParseStyledLine(const std::string& line_utf8) const {
  Elements parts;
  parts.reserve(8);
  size_t pos = 0;

  while (pos < line_utf8.size()) {
    size_t tag_start = line_utf8.find('[', pos);
    if (tag_start == std::string::npos) {
      parts.push_back(stext(std::string_view(line_utf8).substr(pos)));
      break;
    }

    if (tag_start > pos) {
      parts.push_back(stext(std::string_view(line_utf8).substr(pos, tag_start - pos)));
    }

    size_t tag_end = line_utf8.find("](", tag_start);
    if (tag_end == std::string::npos) {
      parts.push_back(stext(std::string_view(line_utf8).substr(tag_start)));
      break;
    }

    const size_t value_open = tag_end + 2;
    size_t val_end = line_utf8.find(')', value_open);
    if (val_end == std::string::npos) {
      parts.push_back(stext(std::string_view(line_utf8).substr(tag_start)));
      break;
    }

    std::string tag = line_utf8.substr(tag_start + 1, tag_end - tag_start - 1);
    std::string value = line_utf8.substr(value_open, val_end - value_open);

    auto it = style_map_.find(tag);
    if (it != style_map_.end()) {
      parts.push_back(stext(value) | it->second);
    } else {
      // Unknown tag: render literally
      parts.push_back(stext("[" + tag + "](" + value + ")"));
    }

    pos = val_end + 1;
  }

  return hbox(std::move(parts));
}
