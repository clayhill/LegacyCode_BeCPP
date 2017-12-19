cmake_minimum_required(VERSION 3.6)

project("ModernCode" LANGUAGES CXX)
set(TESTRUNNER_NAME testrunner)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_CODE
  main.cpp
)

##########################
# clang-tidy integration #
##########################
if(CMAKE_VERSION VERSION_GREATER 3.5)
  set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Add clang-tidy automatically to builds")
  if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
      message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
      set(CLANG_TIDY_CHECKS "-*,modernize-*")
      set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_EXE}
        "-checks=${CLANG_TIDY_CHECKS}"
        "-header-filter='${CMAKE_SOURCE_DIR}/*'"
        "-warnings-as-errors=${CLANG_TIDY_CHECKS}"
        CACHE STRING "" FORCE
      )
    else()
      message(AUTHOR_WARNING "clang-tidy not found!")
      set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # Remove it
    endif()
  endif()
endif()

####################
## Our executable ##
####################
add_executable(${PROJECT_NAME}
  ${SOURCE_CODE}
)

#################
## Test runner ##
#################
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${TESTRUNNER_NAME}
  ${CODE_UNDER_TEST}
  tests/testrunner.cpp
  tests/golden_master_test.cpp
)

target_link_libraries(${TESTRUNNER_NAME}
  ${GTEST_BOTH_LIBRARIES}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Let's use some filesystem features in our tests
  target_link_libraries(${TESTRUNNER_NAME} stdc++fs)
endif()

add_dependencies(${TESTRUNNER_NAME} ${PROJECT_NAME})

add_custom_target(run_tests
  COMMAND ./testrunner
  DEPENDS ${TESTRUNNER_NAME}
)
