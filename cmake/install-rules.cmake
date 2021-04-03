if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/ska_sort CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ska_sort_Development
)

install(
    TARGETS ska_sort
    EXPORT ska_sortTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    ska_sortConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

set(
    ska_sort_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/sort"
    CACHE STRING "CMake package config location relative to the install prefix"
)

mark_as_advanced(ska_sort_INSTALL_CMAKEDIR)

install(
    FILES
    "${PROJECT_SOURCE_DIR}/cmake/ska_sortConfig.cmake"
    "${PROJECT_BINARY_DIR}/ska_sortConfigVersion.cmake"
    DESTINATION "${ska_sort_INSTALL_CMAKEDIR}"
    COMPONENT ska_sort_Development
)

install(
    EXPORT ska_sortTargets
    NAMESPACE ska::
    DESTINATION "${ska_sort_INSTALL_CMAKEDIR}"
    COMPONENT ska_sort_Development
)
