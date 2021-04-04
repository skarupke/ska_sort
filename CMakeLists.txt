cmake_minimum_required(VERSION 3.14)

project(
    ska_sort
    VERSION 0.1.0
    DESCRIPTION "Sorting in less than O(n log n)"
    HOMEPAGE_URL "https://github.com/skarupke/ska_sort"
    LANGUAGES CXX
)

include(cmake/in-source-guard.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(ska_sort INTERFACE)
add_library(ska::sort ALIAS ska_sort)

set_property(TARGET ska_sort PROPERTY EXPORT_NAME sort)

target_include_directories(
    ska_sort
    ${ska_sort_warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(ska_sort INTERFACE cxx_std_14)

# ---- Install ----

include(cmake/install-rules.cmake)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()

# ---- Developer mode ----

if(NOT ska_sort_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(AUTHOR_WARNING "Developer mode is for developers of ska_sort.")
endif()

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
