# Declare the animations library
add_library(app STATIC
  account.cpp
  my_ftx_app.cpp
)

# Set include path for other targets
target_include_directories(app PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app PUBLIC entities)

# Link FTXUI (required by battle_animation.cpp)
target_link_libraries(app PUBLIC
  systems
  ftxui::screen
  ftxui::dom
  ftxui::component
)

# Optional: compile flags
target_compile_options(app PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(app PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

