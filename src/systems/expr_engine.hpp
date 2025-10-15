#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class ExprEngine {
public:
  // Create with an expression and initial variables
  static ExprEngine
  compile(std::string expr,
          const std::unordered_map<std::string, double> &vars);

  ExprEngine(ExprEngine &&) noexcept;
  ExprEngine &operator=(ExprEngine &&) noexcept;
  ~ExprEngine();

  // Set or update a variable; returns false if name is unknown (optional)
  bool set(std::string_view name, double value);

  // Evaluate using current variable values
  double eval() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
  explicit ExprEngine(std::unique_ptr<Impl> impl) noexcept;
  ExprEngine(const ExprEngine &) = delete;
  ExprEngine &operator=(const ExprEngine &) = delete;
};
