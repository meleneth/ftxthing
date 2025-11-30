# Declare the animations library
add_library(animation STATIC
  animator.cpp
  animation.cpp
)

# Set include path for other targets
target_include_directories(animation PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link FTXUI (required by battle_animation.cpp)
target_link_libraries(animation PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component
)

# Optional: compile flags
target_compile_options(animation PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(animation PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

