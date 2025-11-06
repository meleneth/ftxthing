# Declare the screens library

file(GLOB_RECURSE FAIRLANES_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(fairlanes STATIC
  ${FAIRLANES_SOURCES}
)

# Set include path for other targets
target_include_directories(fairlanes 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Link FTXUI (required by battle_screen.cpp)
target_link_libraries(fairlanes
PRIVATE 
  systems 
PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component
)

# Optional: compile flags
target_compile_options(fairlanes PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(fairlanes PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

