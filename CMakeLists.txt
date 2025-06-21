cmake_minimum_required(VERSION 3.16)
project(ftxthing VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch FTXUI using FetchContent
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG v5.0.0  # adjust as needed
)
FetchContent_MakeAvailable(ftxui)

# Add source tree
add_subdirectory(src)
