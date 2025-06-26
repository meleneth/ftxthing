add_subdirectory(widgets)
add_subdirectory(screens)
add_subdirectory(entities)

add_executable(ftxthing ftxthing.cpp)

target_link_libraries(ftxthing PRIVATE entities)
target_link_libraries(ftxthing PRIVATE widgets)
target_link_libraries(ftxthing PRIVATE screens)
target_include_directories(ftxthing PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

