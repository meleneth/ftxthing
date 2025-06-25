#include <ftxui/dom/elements.hpp>
#include <unordered_map>

#include <combat_log.hpp>

using namespace ftxui;

namespace {
using StyleMap = std::unordered_map<std::string, Decorator>;

const StyleMap STYLE_MAP = {
  {"info", color(Color::Green)},
  {"warn", color(Color::Yellow)},
  {"error", color(Color::Red) | bold},
  {"hp", color(Color::Red)},
  {"mp", color(Color::Blue)},
  {"xp", color(Color::LightGreen)},
  {"bold", bold},
  {"dim", dim},
};

Element parse_styled_line(const std::string& line) {
  Elements parts;
  size_t pos = 0;

  while (pos < line.size()) {
    size_t tag_start = line.find('[', pos);
    if (tag_start == std::string::npos) {
      parts.push_back(text(line.substr(pos)));
      break;
    }

    if (tag_start > pos)
      parts.push_back(text(line.substr(pos, tag_start - pos)));

    size_t tag_end = line.find("](", tag_start);
    if (tag_end == std::string::npos) {
      parts.push_back(text(line.substr(tag_start)));
      break;
    }

    std::string tag = line.substr(tag_start + 1, tag_end - tag_start - 1);

    size_t val_end = line.find(')', tag_end + 2);
    if (val_end == std::string::npos) {
      parts.push_back(text(line.substr(tag_start)));
      break;
    }

    std::string value = line.substr(tag_end + 2, val_end - (tag_end + 2));

    auto it = STYLE_MAP.find(tag);
    if (it != STYLE_MAP.end()) {
      parts.push_back(text(value) | it->second);
    } else {
      parts.push_back(text("[" + tag + "](" + value + ")"));
    }

    pos = val_end + 1;
  }

  return hbox(std::move(parts));
}
} // namespace

CombatLog::CombatLog(int max_height)
  : max_height(max_height) {}

void CombatLog::Append(const std::string& line) {
  log.push_back(line);
  if (log.size() > max_log_size)
    log.pop_front();
}

Element CombatLog::Render() {
  Elements parsed;
  size_t total = log.size();
  size_t count = std::min<size_t>(max_height, total);
  size_t start = total - count;

  for (size_t i = start; i < total; ++i) {
    parsed.push_back(parse_styled_line(log[i]));
  }

  return vbox(std::move(parsed)) | vscroll_indicator | frame | border;
}

