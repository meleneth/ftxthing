file(GLOB_RECURSE SYSTEMS_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(systems STATIC
  ${SYSTEMS_SOURCES}
)

# Headers live in the source dir
target_include_directories(systems PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${pcg-cpp_SOURCE_DIR}/include
)

# Link against SML (header-only target)
target_link_libraries(systems
PUBLIC
Boost::sml::system
  ftxui::screen
  ftxui::dom
  ftxui::component
ExprTk::exprtk
spdlog::spdlog
CLI11::CLI11
)

target_include_directories(systems PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(systems PRIVATE -O0 -g -Wa,-mbig-obj)

# Warnings (portable)
if(MSVC)
  target_compile_options(systems PRIVATE /W4 /permissive-)
else()
  target_compile_options(systems PRIVATE -Wall -Wextra -Wpedantic)
endif()
