# Declare the library
add_library(entities STATIC
  entities.cpp
  party.cpp
)

# Link EnTT (it's INTERFACE, so PUBLIC propagates usage reqs to dependents)
target_link_libraries(entities PUBLIC
  EnTT::EnTT
  nlohmann_json::nlohmann_json
)

# Headers for this target (so dependents can do #include "entities/…")
target_include_directories(entities PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Warnings (portable)
if(MSVC)
  target_compile_options(entities PRIVATE /W4)
else()
  target_compile_options(entities PRIVATE -Wall -Wextra -Wpedantic)
endif()
