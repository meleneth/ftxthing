# tests/CMakeLists.txt

# Glob test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

# Test executable (uses Catch2WithMain)
add_executable(ftxthing_tests ${TEST_SOURCES})

target_compile_options(ftxthing_tests PRIVATE -O0 -g ${BIGOBJ_FLAG})

# If you’re globally using LTO, explicitly disable it for tests:
set_property(TARGET ftxthing_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)

target_precompile_headers(ftxthing_tests PRIVATE
  <catch2/catch_test_macros.hpp>
  <catch2/matchers/catch_matchers_floating_point.hpp>
)

target_link_libraries(ftxthing_tests PRIVATE app)
target_link_libraries(ftxthing_tests PRIVATE fairlanes)
target_link_libraries(ftxthing_tests PRIVATE animation)
target_link_libraries(ftxthing_tests PRIVATE systems)
target_link_libraries(ftxthing_tests PRIVATE content)

# Link test binary with main code + test framework
target_link_libraries(ftxthing_tests
  PRIVATE
    Catch2::Catch2WithMain
    ExprTk::exprtk
    nlohmann_json::nlohmann_json
  PUBLIC
  TracyClient
)

target_include_directories(ftxthing_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)


# Test discovery integration
include(CTest)
include(Catch)
catch_discover_tests(ftxthing_tests)
