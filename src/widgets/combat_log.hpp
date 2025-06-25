#pragma once
#include <ftxui/component/component.hpp>
#include <deque>
#include <string>

class CombatLog : public ftxui::ComponentBase {
public:
  CombatLog(int max_height = 10);

  void Append(const std::string& line);
  ftxui::Element Render() override;

private:
  std::deque<std::string> log;
  int max_height;
  static constexpr size_t max_log_size = 512;
};

