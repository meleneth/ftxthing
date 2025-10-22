# Declare the library
file(GLOB_RECURSE CONTENT_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(content STATIC
  ${CONTENT_SOURCES}
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
