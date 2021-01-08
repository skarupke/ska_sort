cmake_minimum_required(VERSION 3.14)

project(ska_sort_tests CXX)

# ---- Dependencies ----

include(FetchContent)
FetchContent_Declare(ska_sort SOURCE_DIR "${PROJECT_SOURCE_DIR}/..")
FetchContent_MakeAvailable(ska_sort)

find_package(doctest 2.4.4 CONFIG REQUIRED)
include(doctest)

# ---- Options ----

option(FULL_TESTS "Include slow to compile tests" ON)

# ---- Test ----

enable_testing()

function(test_case NAME)
  add_executable("${NAME}" "source/${NAME}.cpp")
  set_property(
      SOURCE "source/${NAME}.cpp"
      PROPERTY COMPILE_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
      DOCTEST_CONFIG_SUPER_FAST_ASSERTS
  )
  target_include_directories("${NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries("${NAME}" PRIVATE doctest::doctest ska::sort)
  target_compile_features("${NAME}" PRIVATE cxx_std_14)
  doctest_discover_tests("${NAME}" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endfunction()

test_case(ska_sort_tests)
if(FULL_TESTS)
  target_compile_definitions(ska_sort_tests PRIVATE FULL_TESTS)
endif()

# test_case(ska_sort_benchmarks)
