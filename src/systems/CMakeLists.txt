# Declare the library
add_library(systems STATIC
  systems.cpp
)

# Headers live in the source dir
target_include_directories(systems PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against SML (header-only target)
target_link_libraries(systems PUBLIC Boost::sml::system)

# Warnings (portable)
if(MSVC)
  target_compile_options(systems PRIVATE /W4 /permissive-)
else()
  target_compile_options(systems PRIVATE -Wall -Wextra -Wpedantic)
endif()
