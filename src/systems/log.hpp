#pragma once
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

struct LogEntry {
  std::chrono::system_clock::time_point ts{};
  spdlog::level::level_enum level{spdlog::level::info};
  std::string message;
};

class LogBuffer {
public:
  explicit LogBuffer(std::size_t capacity) : capacity_(capacity) {
    buf_.reserve(capacity);
  }

  // push a fully formatted line
  void push_line(spdlog::level::level_enum lvl, std::string line) {
    std::lock_guard<std::mutex> lk(mu_);
    LogEntry e;
    e.ts = std::chrono::system_clock::now();
    e.level = lvl;
    e.message = std::move(line);
    if (size_ < capacity_) {
      buf_.push_back(std::move(e));
      ++size_;
    } else {
      buf_[head_] = std::move(e);
      head_ = (head_ + 1) % capacity_;
    }
    ++seq_; // bump monotonic sequence
  }

  // snapshot: copy out entries in buffer order (oldest..newest)
  std::vector<LogEntry> snapshot() const {
    std::lock_guard<std::mutex> lk(mu_);
    std::vector<LogEntry> out;
    out.reserve(size_);
    for (std::size_t i = 0; i < size_; ++i) {
      std::size_t idx = (head_ + i) % capacity_;
      out.push_back(buf_[idx]);
    }
    return out;
  }

  // incremental fetch: return all entries with seq > since_seq
  // returns {entries, new_seq}
  std::pair<std::vector<LogEntry>, std::uint64_t>
  since(std::uint64_t since_seq) const {
    std::lock_guard<std::mutex> lk(mu_);
    std::vector<LogEntry> out;
    if (since_seq == seq_)
      return {out, seq_};

    // if the gap is larger than capacity, just return snapshot()
    if (seq_ - since_seq > capacity_) {
      out.reserve(size_);
      for (std::size_t i = 0; i < size_; ++i) {
        std::size_t idx = (head_ + i) % capacity_;
        out.push_back(buf_[idx]);
      }
      return {out, seq_};
    }

    // compute start index relative to newest
    // newest logical index is (head_ + size_ - 1) % capacity_
    // number of new items = (seq_ - since_seq) but cap to size_
    std::size_t new_items = static_cast<std::size_t>(
        std::min<std::uint64_t>(seq_ - since_seq, size_));
    std::size_t start = (head_ + size_ - new_items) % capacity_;
    out.reserve(new_items);
    for (std::size_t i = 0; i < new_items; ++i) {
      std::size_t idx = (start + i) % capacity_;
      out.push_back(buf_[idx]);
    }
    return {out, seq_};
  }

  std::size_t size() const {
    std::lock_guard<std::mutex> lk(mu_);
    return size_;
  }
  std::size_t capacity() const { return capacity_; }
  std::uint64_t seq() const {
    std::lock_guard<std::mutex> lk(mu_);
    return seq_;
  }

private:
  const std::size_t capacity_;
  mutable std::mutex mu_;
  std::vector<LogEntry> buf_;
  std::size_t head_{0};  // next overwrite position when full
  std::size_t size_{0};  // number of valid entries <= capacity
  std::uint64_t seq_{0}; // monotonic sequence for incremental reads
};

// Create a logger that writes to LogBuffer and nudges the FTXUI screen to
// refresh.
std::shared_ptr<spdlog::logger>
make_ftx_logger(std::shared_ptr<LogBuffer> buffer,
                std::function<void()> wake_ui, std::string logger_name = "ui");

// Convenience overload: no wake callback.
std::shared_ptr<spdlog::logger>
make_ftx_logger(std::shared_ptr<LogBuffer> buffer,
                std::string logger_name = "ui");