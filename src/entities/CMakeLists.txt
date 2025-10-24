# Declare the library
file(GLOB_RECURSE ENTITIES_SOURCES CONFIGURE_DEPENDS *.cpp)

# Declare the library
add_library(entities STATIC
  ${ENTITIES_SOURCES}
)

# Link EnTT (it's INTERFACE, so PUBLIC propagates usage reqs to dependents)
target_link_libraries(entities 
PRIVATE
app
PUBLIC
  EnTT::EnTT
  nlohmann_json::nlohmann_json
)

# Headers for this target (so dependents can do #include "entities/…")
target_include_directories(entities PUBLIC
  PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# Warnings (portable)
if(MSVC)
  target_compile_options(entities PRIVATE /W4)
else()
  target_compile_options(entities PRIVATE -Wall -Wextra -Wpedantic)
endif()
