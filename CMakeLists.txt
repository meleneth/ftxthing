cmake_minimum_required(VERSION 3.20)
project(ftxthing VERSION 0.1.0 LANGUAGES CXX)


option(FTXTHING_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(FTXTHING_ENABLE_LTO          "Enable link-time optimization" OFF)
option(BUILD_SHARED_LIBS            "Build shared libs (if any)" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CPM_SOURCE_CACHE)
  set(CPM_SOURCE_CACHE "${CMAKE_BINARY_DIR}/cpm-cache")
endif()

include(cmake/CPM.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.5.2")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

function(ftxthing_set_warnings tgt)
  if(MSVC)
    target_compile_options(${tgt} PRIVATE /W4 $<$<BOOL:${FTXTHING_WARNINGS_AS_ERRORS}>:/WX>)
  else()
    target_compile_options(${tgt} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wdouble-promotion
      $<$<BOOL:${FTXTHING_WARNINGS_AS_ERRORS}>:-Werror>)
  endif()
endfunction()

if(FTXTHING_ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT _ipo_supported OUTPUT _ipo_msg)
  if(_ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  else()
    message(WARNING "IPO/LTO not supported: ${_ipo_msg}")
  endif()
endif()

include(FetchContent)
# --- boost-ext/sml -----------------------------------------------------------
FetchContent_Declare(
  sml_src
  GIT_REPOSITORY https://github.com/boost-ext/sml.git
  GIT_TAG        v1.1.8
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sml_src)

# Prefer upstream target if present; else define our own
if(TARGET sml)
  # Create a SYSTEM-wrapped shim, then alias with '::'
  add_library(boost_sml_system_iface INTERFACE)
  target_link_libraries(boost_sml_system_iface INTERFACE sml)
  # Re-expose sml's include dirs as SYSTEM to mute 3rd-party warnings
  get_target_property(_sml_inc sml INTERFACE_INCLUDE_DIRECTORIES)
  if(_sml_inc)
    target_include_directories(boost_sml_system_iface SYSTEM INTERFACE ${_sml_inc})
  endif()
  # Friendly stable aliases
  if(NOT TARGET Boost::sml)
    add_library(Boost::sml ALIAS sml)
  endif()
  add_library(Boost::sml::system ALIAS boost_sml_system_iface)
else()
  # Fallback: build our own header-only target
  add_library(boost_sml_iface INTERFACE)
  target_include_directories(boost_sml_iface SYSTEM INTERFACE ${sml_src_SOURCE_DIR}/include)
  add_library(Boost::sml ALIAS boost_sml_iface)
  add_library(Boost::sml::system ALIAS boost_sml_iface)
endif()

# --- EnTT --------------------------------------------------------------------
FetchContent_Declare(
  entt_src
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG        v3.15.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(entt_src)

if(TARGET EnTT::EnTT)
  add_library(entt_system_iface INTERFACE)
  target_link_libraries(entt_system_iface INTERFACE EnTT::EnTT)
  get_target_property(_entt_inc EnTT::EnTT INTERFACE_INCLUDE_DIRECTORIES)
  if(_entt_inc)
    target_include_directories(entt_system_iface SYSTEM INTERFACE ${_entt_inc})
  endif()
  add_library(EnTT::EnTT::system ALIAS entt_system_iface)
else()
  add_library(entt_iface INTERFACE)
  target_include_directories(entt_iface SYSTEM INTERFACE ${entt_src_SOURCE_DIR}/src)
  add_library(EnTT::EnTT ALIAS entt_iface)
  add_library(EnTT::EnTT::system ALIAS entt_iface)
endif()


# --- FTXUI -------------------------------------------------------------------
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(ftxui)
# targets: ftxui::screen ftxui::dom ftxui::component

list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")

# --- ExprTk via CPM ----------------------------------------------------------
# Pin to a specific commit for reproducibility (Jan 1, 2024)
# See: https://github.com/ArashPartow/exprtk/commits/master
CPMAddPackage(
  NAME exprtk
  GITHUB_REPOSITORY ArashPartow/exprtk
  GIT_TAG cc1b800c2bd1ac3ac260478c915d2aec6f4eb41c   # replace with the full SHA if you prefer
  DOWNLOAD_ONLY YES
)

# ExprTk is just a single header (exprtk.hpp). Create an INTERFACE target.
if(exprtk_ADDED)
  add_library(exprtk INTERFACE)
  target_include_directories(exprtk INTERFACE "${exprtk_SOURCE_DIR}")
  add_library(ExprTk::exprtk ALIAS exprtk)
endif()

CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.3
)

CPMAddPackage(
  NAME pcg-cpp
  GITHUB_REPOSITORY imneme/pcg-cpp
  GIT_TAG master
)

CPMAddPackage("gh:CLIUtils/CLI11@2.4.2")
# fmt (compiled library)
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 10.2.1
  OPTIONS
    "FMT_DOC OFF"
    "FMT_TEST OFF"
)

# Tell spdlog to use the external fmt we just added
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.14.1
  OPTIONS
    "SPDLOG_BUILD_SHARED OFF"
    "SPDLOG_BUILD_EXAMPLE OFF"
    "SPDLOG_BUILD_TESTS OFF"
)

# --- Your code ---------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(external)  # only if you actually have it

if (EMSCRIPTEN)
  target_compile_definitions(ftxthing PRIVATE __EMSCRIPTEN__)
  target_link_options(ftxthing PRIVATE
    "-sALLOW_MEMORY_GROWTH=1"
    "-sASSERTIONS=1"
    # "--preload-file" "assets@/assets"   # if you have assets
  )
  set_target_properties(ftxthing PROPERTIES OUTPUT_NAME "ftxthing")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
