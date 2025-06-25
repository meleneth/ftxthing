add_subdirectory(widgets)
add_subdirectory(screens)

add_executable(ftxthing ftxthing.cpp)

target_link_libraries(ftxthing PRIVATE widgets)
target_link_libraries(ftxthing PRIVATE screens)
