# Declare the animations library
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(app STATIC
  ${APP_SOURCES}
)

# Set include path for other targets
target_include_directories(app PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app PRIVATE entities)
target_link_libraries(app PRIVATE systems)
target_link_libraries(app PRIVATE widgets)
target_link_libraries(app PUBLIC fairlanes)

# Link FTXUI (required by battle_animation.cpp)
target_link_libraries(app PUBLIC
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
  ${PROJECT_SOURCE_DIR}/src/include
)

