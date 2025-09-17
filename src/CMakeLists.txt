add_subdirectory(widgets)
add_subdirectory(screens)
add_subdirectory(entities)
add_subdirectory(animation)

add_executable(ftxthing ftxthing.cpp)

target_link_libraries(ftxthing PRIVATE entities)
target_link_libraries(ftxthing PRIVATE animation)
target_link_libraries(ftxthing PRIVATE widgets)
target_link_libraries(ftxthing PRIVATE screens)

target_include_directories(ftxthing PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

target_link_libraries(ftxthing PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
  Boost::sml::system
  EnTT::EnTT::system
)

ftxthing_set_warnings(ftxthing)
target_compile_features(ftxthing PRIVATE cxx_std_20)

if(WIN32)
  target_compile_definitions(ftxthing PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
