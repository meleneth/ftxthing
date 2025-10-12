#include "expr_engine.hpp"
#include <exprtk.hpp>
#include <stdexcept>

struct ExprEngine::Impl {
  using T = double;
  exprtk::symbol_table<T> sym;
  exprtk::expression<T>   expr;
  exprtk::parser<T>       parser;

  // Keep storage for variables so the symbol_table can bind to them
  std::unordered_map<std::string, T> vars;

  Impl(std::string source,
       const std::unordered_map<std::string, double>& init) {
    vars = init;
    for (auto& [name, val] : vars) sym.add_variable(name, val);
    sym.add_constants();
    expr.register_symbol_table(sym);
    if (!parser.compile(source, expr)) {
      throw std::runtime_error("ExprTk compile failed");
    }
  }
};

ExprEngine ExprEngine::compile(std::string expr,
                               const std::unordered_map<std::string, double>& vars) {
  auto impl = std::make_unique<Impl>(std::move(expr), vars);
  return ExprEngine(std::move(impl));
}

ExprEngine::ExprEngine(std::unique_ptr<Impl> impl) noexcept : impl_(std::move(impl)) {}
ExprEngine::~ExprEngine() = default;
ExprEngine::ExprEngine(ExprEngine&&) noexcept = default;
ExprEngine& ExprEngine::operator=(ExprEngine&&) noexcept = default;

bool ExprEngine::set(std::string_view name, double value) {
  auto it = impl_->vars.find(std::string(name));
  if (it == impl_->vars.end()) return false;
  it->second = value;
  return true;
}

double ExprEngine::eval() const { return impl_->expr.value(); }
