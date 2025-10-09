#include <sstream>

#include <catch2/catch_test_macros.hpp>

namespace ftxthing {
};

TEST_CASE("SimplePlayerCase")
{
  using namespace ftxthing;

  REQUIRE(2 == 1);
}
