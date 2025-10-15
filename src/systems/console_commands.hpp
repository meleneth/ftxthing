// console_commands.hpp
#pragma once
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

struct CommandEngine {
  CLI::App app{"ftx console"};

  CommandEngine() {
    // Example commands:
    // : log level set
    auto *log_cmd = app.add_subcommand("log", "Logging commands");
    std::string level;
    log_cmd->add_option("level", level, "trace|debug|info|warn|error|critical");
    log_cmd->callback([&, level_ptr = &level]() {
      auto s = *level_ptr;
      std::unordered_map<std::string, spdlog::level::level_enum> map{
          {"trace", spdlog::level::trace},
          {"debug", spdlog::level::debug},
          {"info", spdlog::level::info},
          {"warn", spdlog::level::warn},
          {"error", spdlog::level::err},
          {"critical", spdlog::level::critical}};
      auto it = map.find(s);
      if (it == map.end()) {
        spdlog::warn("unknown level: {}", s);
        return;
      }
      spdlog::set_level(it->second);
      spdlog::info("log level set to {}", s);
    });

    // : party list
    app.add_subcommand("party-list", "List parties")->callback([] {
      spdlog::info("[party] A=adventuring; B=town; C=idle");
    });

    // : party focus <id>
    int id = -1;
    auto *pf = app.add_subcommand("party-focus", "Focus party by id");
    pf->add_option("id", id, "party id")->required();
    pf->callback([id] {
      spdlog::info("Focusing party {}", id);
      // emit an event to UI FSM if you want (via mailbox)
    });
  }

  // Minimal shell-words split (keep simple for now)
  static std::vector<std::string> split(const std::string &line) {
    std::vector<std::string> out;
    std::string cur;
    bool inq = false;
    for (char c : line) {
      if (c == '"') {
        inq = !inq;
        continue;
      }
      if (!inq && std::isspace((unsigned char)c)) {
        if (!cur.empty()) {
          out.push_back(cur);
          cur.clear();
        }
      } else
        cur.push_back(c);
    }
    if (!cur.empty())
      out.push_back(cur);
    return out;
  }

  void execute_line(const std::string &line) {
    auto tokens = split(line);
    if (tokens.empty())
      return;

    // Build fake argv (argv[0] is program name)
    std::vector<const char *> argv;
    argv.push_back("ftx-console");
    for (auto &s : tokens)
      argv.push_back(s.c_str());
    int argc = (int)argv.size();

    // Parse this one command against a fresh copy of the app to avoid sticky
    // state.
    CLI::App local = app;
    try {
      local.parse(argc, argv.data());
    } catch (const CLI::ParseError &e) {
      auto rc = local.exit(e);
      spdlog::warn("parse error: {}", rc);
    }
  }
};
