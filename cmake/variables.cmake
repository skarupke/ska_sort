include(cmake/project-is-top-level.cmake)

# ---- Developer mode ----

if(PROJECT_IS_TOP_LEVEL)
  option(ska_sort_DEVELOPER_MODE "Enable developer mode" "$ENV{CI}")
endif()

# ---- Warning guard ----

# Protect dependents from this project's warnings if the guard isn't disabled
set(ska_sort_warning_guard SYSTEM)
if(PROJECT_IS_TOP_LEVEL OR ska_sort_INCLUDE_WITHOUT_SYSTEM)
  set(ska_sort_warning_guard "")
endif()

# ---- Warnings ----

if(ska_sort_DEVELOPER_MODE)
  include(cmake/warnings.cmake)
endif()
