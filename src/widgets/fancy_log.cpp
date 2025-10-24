#include <algorithm>
#include <cstddef>
#include <utility>

#include "fancy_log.hpp"
#include "systems/log.hpp"

using namespace ftxui;
using namespace fairlanes;

// ---- construction ----------------------------------------------------------

FancyLog::FancyLog() : FancyLog(Options{}) {}

FancyLog::FancyLog(Options opt) : opts(opt) {
  // sane defaults you can override with styles(...)
  style_map = {
      {"name", bold},
      {"error", color(Color::Red)},
      {"warn", color(Color::Yellow)},
      {"ok", color(Color::Green)},
      {"hint", dim},
  };
}

// ---- append ---------------------------------------------------------------

void FancyLog::append_markup(std::string_view utf8_line) {
  spdlog::info(utf8_line);
  push(parse_markup(utf8_line));
}

void FancyLog::append(Element el) { push(std::move(el)); }

// ---- styles ---------------------------------------------------------------

void FancyLog::style(std::string tag, Decorator deco) {
  style_map[std::move(tag)] = deco;
}

void FancyLog::styles(StyleMap map) { style_map = std::move(map); }

void FancyLog::clear_styles() { style_map.clear(); }

// ---- housekeeping ---------------------------------------------------------

void FancyLog::clear() { log.clear(); }

// ---- sizing / scrolling ---------------------------------------------------

void FancyLog::set_max_rows(int rows) { opts.max_rows = std::max(0, rows); }

int FancyLog::max_rows() const { return opts.max_rows; }

void FancyLog::set_max_entries(size_t n) {
  opts.max_entries = n == 0 ? 1 : n;
  // downsize immediately if needed
  while (log.size() > opts.max_entries)
    log.pop_front();
}

size_t FancyLog::max_entries() const { return opts.max_entries; }

void FancyLog::set_autoscroll(bool v) { opts.autoscroll = v; }
bool FancyLog::autoscroll() const { return opts.autoscroll; }

size_t FancyLog::size() const { return log.size(); }
bool FancyLog::empty() const { return log.empty(); }

// ---- ComponentBase --------------------------------------------------------

Element FancyLog::Render() {
  const int rows = std::max(0, opts.max_rows);
  const size_t take = std::min(log.size(), static_cast<size_t>(rows));

  std::vector<Element> last;
  last.reserve(take);

  auto begin = log.size() > take ? log.end() - static_cast<std::ptrdiff_t>(take)
                                 : log.begin();
  std::copy(begin, log.end(), std::back_inserter(last));

  // size() clamps the box visually; swap to yframe()+vscroll_indicator() if you
  // add manual scrolling.
  return vbox(std::move(last)) |
         ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, rows);
}

// ---- internals ------------------------------------------------------------

// Parse "[tag](content)" segments; degrade gracefully on malformed input.
Element FancyLog::parse_markup(std::string_view s) const {
  std::vector<Element> parts;
  std::string plain;
  const size_t n = s.size();
  size_t i = 0;

  auto flush_plain = [&](std::string &buf) {
    if (!buf.empty()) {
      parts.push_back(text(buf));
      buf.clear();
    }
  };

  while (i < n) {
    size_t lb = s.find('[', i);
    if (lb == std::string_view::npos) {
      plain.append(s.substr(i));
      break;
    }

    // prefix plain
    plain.append(s.substr(i, lb - i));

    size_t rb = s.find(']', lb + 1);
    if (rb == std::string_view::npos) {
      // treat as literal
      plain.push_back(s[lb]);
      i = lb + 1;
      continue;
    }

    if (rb + 1 >= n || s[rb + 1] != '(') {
      plain.append(s.substr(lb, rb - lb + 1));
      i = rb + 1;
      continue;
    }

    size_t rp = s.find(')', rb + 2);
    if (rp == std::string_view::npos) {
      plain.append(s.substr(lb));
      i = n;
      break;
    }

    std::string tag{s.substr(lb + 1, rb - (lb + 1))};
    std::string content{s.substr(rb + 2, rp - (rb + 2))};

    flush_plain(plain);

    if (auto it = style_map.find(tag); it != style_map.end()) {
      parts.push_back(text(content) | it->second);
    } else {
      parts.push_back(text(content));
    }

    i = rp + 1;
  }

  flush_plain(plain);

  if (parts.empty())
    return text("");
  if (parts.size() == 1)
    return std::move(parts.front());
  return hbox(std::move(parts));
}

void FancyLog::push(Element el) {
  // append, enforce capacity
  log.push_back(std::move(el));
  if (log.size() > opts.max_entries)
    log.pop_front();

  // autoscroll is a no-op visually for now because we render only tail.
  // If you later add manual scrolling state, pin the cursor/index here.
}
