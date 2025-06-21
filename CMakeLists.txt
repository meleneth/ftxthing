cmake_minimum_required(VERSION 3.16)
project(ftxthing VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use FetchContent to pull in FTXUI
include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG v5.0.0 # or whatever the latest version is
)

FetchContent_MakeAvailable(ftxui)

# Build the executable
add_executable(ftxthing src/ftxthing.cpp)

# Link FTXUI to your executable
target_link_libraries(ftxthing PRIVATE ftxui::screen ftxui::dom ftxui::component)

# Optional: Warnings and flags
target_compile_options(ftxthing PRIVATE -Wall -Wextra -Wpedantic -Werror)
