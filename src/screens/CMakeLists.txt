# Declare the screens library
add_library(screens STATIC
  battle_screen.cpp
)

# Set include path for other targets
target_include_directories(screens PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(screens PUBLIC widgets)
target_link_libraries(screens PUBLIC entities)

# Link FTXUI (required by battle_screen.cpp)
target_link_libraries(screens PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component
)

# Optional: compile flags
target_compile_options(screens PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(screens PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

