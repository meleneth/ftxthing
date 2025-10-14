#include <cmath>                                 // std::sqrt

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "systems/expr_engine.hpp"

TEST_CASE("ExprTk basic arithmetic works") {
  auto eng = ExprEngine::compile("x * y + sqrt(x)", {{"x",2.0 },{"y",5.0}});

  REQUIRE_THAT( eng.eval(), Catch::Matchers::WithinRel(2.0 * 5.0 + std::sqrt(2.0), 0.001));
  eng.set("x", 9.0);
  REQUIRE_THAT( eng.eval(), Catch::Matchers::WithinRel(9.0 * 5.0 + 3.0, 0.001));
}
