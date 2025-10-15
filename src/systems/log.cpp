#include "log.hpp"

#include <spdlog/sinks/base_sink.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/spdlog.h>

#include <mutex>
#include <utility>

namespace {

// A sink that formats messages and pushes them into LogBuffer.
// It optionally calls a wake-up callback (to refresh the UI, etc.)
class logbuffer_sink final : public spdlog::sinks::base_sink<std::mutex> {
public:
  explicit logbuffer_sink(std::shared_ptr<LogBuffer> buf,
                          std::function<void()> wake_ui)
      : buf_(std::move(buf)), wake_ui_(std::move(wake_ui)) {
    // default formatter: "[HH:MM:SS.mmm] lvl: message"
    this->set_formatter(std::make_unique<spdlog::pattern_formatter>(
        "[%H:%M:%S.%e] %^%l%$: %v"));
  }

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    // Format the message into a string
    spdlog::memory_buf_t formatted;
    this->formatter_->format(msg, formatted);

    // Push formatted line into buffer
    buf_->push_line(msg.level, fmt::to_string(formatted));

    // Wake UI (non-blocking)
    if (wake_ui_) {
      try {
        wake_ui_();
      } catch (...) {
        // swallow exceptions to avoid crashing logging thread
      }
    }
  }

  void flush_() override {
    // nothing to do (LogBuffer is in-memory)
  }

private:
  std::shared_ptr<LogBuffer> buf_;
  std::function<void()> wake_ui_;
};

} // namespace

// Factory to create an spdlog logger that writes to LogBuffer and can wake the UI.
std::shared_ptr<spdlog::logger>
make_ftx_logger(std::shared_ptr<LogBuffer> buffer,
                std::function<void()> wake_ui,
                std::string logger_name) {
  // Create our custom sink
  auto sink =
      std::make_shared<logbuffer_sink>(std::move(buffer), std::move(wake_ui));

  // Create logger using only this sink
  auto logger = std::make_shared<spdlog::logger>(
      std::move(logger_name), spdlog::sinks_init_list{sink});

  logger->set_level(spdlog::level::trace);
  logger->flush_on(spdlog::level::err);

  // Optional: make it the default logger
  spdlog::set_default_logger(logger);
  return logger;
}
