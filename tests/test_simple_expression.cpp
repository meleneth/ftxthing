#include <cmath>                                 // std::sqrt
#include <exprtk.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("ExprTk basic arithmetic works") {
  using T = double;
  T x = 2.0, y = 5.0;

  exprtk::symbol_table<T> sym;
  sym.add_variable("x", x);
  sym.add_variable("y", y);
  sym.add_constants();

  exprtk::expression<T> e;
  e.register_symbol_table(sym);

  exprtk::parser<T> p;
  REQUIRE(p.compile("x * y + sqrt(x)", e));

  REQUIRE_THAT( e.value(), Catch::Matchers::WithinRel(9.0 * 5.0 + std::sqrt(2.0), 0.001));
  x = 9.0;
  REQUIRE_THAT( e.value(), Catch::Matchers::WithinRel(9.0 * 5.0 + 3.0, 0.001));
}
