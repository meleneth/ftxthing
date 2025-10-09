# Declare the library
add_library(content STATIC
  cycle_i_origin.cpp
  cycle_ii_resonance.cpp
  cycle_iii_conflict.cpp
  cycle_iv_singularity.cpp
)

# Headers live in the source dir
target_include_directories(content PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Warnings (portable)
if(MSVC)
  target_compile_options(content PRIVATE /W4 /permissive-)
else()
  target_compile_options(content PRIVATE -Wall -Wextra -Wpedantic)
endif()
