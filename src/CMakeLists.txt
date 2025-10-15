# Make narrow string literals compiled as UTF-8 everywhere.
if(MSVC)
  add_compile_options(/utf-8)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

add_subdirectory(widgets)
add_subdirectory(screens)
add_subdirectory(entities)
add_subdirectory(animation)
add_subdirectory(systems)
add_subdirectory(content)
add_subdirectory(app)

add_executable(ftxthing ftxthing.cpp)

target_link_libraries(ftxthing PRIVATE entities)
target_link_libraries(ftxthing PRIVATE animation)
target_link_libraries(ftxthing PRIVATE widgets)
target_link_libraries(ftxthing PRIVATE screens)
target_link_libraries(ftxthing PRIVATE systems)
target_link_libraries(ftxthing PRIVATE app)
target_link_libraries(ftxthing PRIVATE content)

target_include_directories(ftxthing PRIVATE
  ${entt_SOURCE_DIR}/single_include
)

target_link_libraries(ftxthing PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
  Boost::sml::system
  EnTT::EnTT::system
  ExprTk::exprtk
  CLI11::CLI11
  spdlog::spdlog
)

ftxthing_set_warnings(ftxthing)
target_compile_features(ftxthing PRIVATE cxx_std_20)

if(WIN32)
  target_compile_definitions(ftxthing PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
