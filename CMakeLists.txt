cmake_minimum_required(VERSION 3.14)

project(
    ska_sort
    VERSION 0.0.1
    DESCRIPTION "Sorting in less than O(n log n)"
    HOMEPAGE_URL "https://github.com/skarupke/ska_sort"
    LANGUAGES CXX
)

# ---- Warning guard ----

# Protect dependents from this project's warnings if the guard isn't disabled
set(ska_sort_warning_guard SYSTEM)
if(ska_sort_INCLUDE_WITHOUT_SYSTEM)
  set(ska_sort_warning_guard "")
endif()

# ---- Declare library ----

add_library(sort INTERFACE)
add_library(ska::sort ALIAS sort)

target_include_directories(
    sort
    ${ska_sort_warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(sort INTERFACE cxx_std_14)

# ---- Install ----

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(sort_directory "ska_sort-${PROJECT_VERSION}")
set(sort_include_directory "${CMAKE_INSTALL_INCLUDEDIR}/${sort_directory}")

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION "${sort_include_directory}"
    COMPONENT ska_sort_Development
)

install(
    TARGETS sort
    EXPORT ska_sortTargets
    INCLUDES DESTINATION "${sort_include_directory}"
)

write_basic_package_version_file(
    ska_sort-config-version.cmake
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

set(sort_install_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake/${sort_directory}")

install(
    FILES "${PROJECT_BINARY_DIR}/ska_sort-config-version.cmake"
    DESTINATION "${sort_install_cmakedir}"
    COMPONENT ska_sort_Development
)

install(
    EXPORT ska_sortTargets
    FILE ska_sort-config.cmake
    NAMESPACE ska::
    DESTINATION "${sort_install_cmakedir}"
    COMPONENT ska_sort_Development
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  include(CPack)
endif()
