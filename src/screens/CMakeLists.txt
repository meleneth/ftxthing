# screens/CMakeLists.txt (or wherever this lives)
file(GLOB_RECURSE SCREENS_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(screens STATIC
  ${SCREENS_SOURCES}
)

# Headers for this target (lets dependents do: #include "screens/…")
target_include_directories(screens PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link deps; PUBLIC so downstream users get the usage requirements
target_link_libraries(screens PUBLIC
  entities
  animation
  ftxui::screen
  ftxui::dom
  ftxui::component
  EnTT::EnTT
  Boost::sml
)

# Portable warnings
if(MSVC)
  target_compile_options(screens PRIVATE /W4)
else()
  target_compile_options(screens PRIVATE -Wall -Wextra -Wpedantic)
  # Add -Werror only if you really want to fail on warnings:
  # target_compile_options(screens PRIVATE -Werror)
endif()
