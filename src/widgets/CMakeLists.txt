# Declare the screens library
add_library(widgets STATIC
  combat_log.cpp
  root_component.cpp
  footer_component.cpp
  body_component.cpp
  console_overlay.cpp
)

# Set include path for other targets
target_include_directories(widgets PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link FTXUI (required by battle_screen.cpp)
target_link_libraries(widgets PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component
)

# Optional: compile flags
target_compile_options(widgets PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(widgets PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

