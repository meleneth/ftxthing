# Declare the screens library
add_library(entities STATIC
  entities.cpp
)

# Set include path for other targets
target_include_directories(entities PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Optional: compile flags
target_compile_options(entities PRIVATE
  -Wall -Wextra -Wpedantic -Werror
)

target_include_directories(entities
  PRIVATE
    ${entt_SOURCE_DIR}/single_include
)

