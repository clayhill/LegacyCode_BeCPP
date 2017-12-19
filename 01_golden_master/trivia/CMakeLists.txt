cmake_minimum_required(VERSION 3.1)

project("TriviaGame" LANGUAGES CXX)
set(TESTRUNNER_NAME testrunner)

set(CMAKE_CXX_STANDARD 17)

#####################
## Code under test ##
#####################
set(CODE_UNDER_TEST
  Game.cpp Game.h
)

############################
## Trivia Game executable ##
############################
add_executable(${PROJECT_NAME}
  ${CODE_UNDER_TEST}
  GameRunner.cpp
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
  tests/golden_master_function_test.cpp
)

target_link_libraries(${TESTRUNNER_NAME}
  ${GTEST_BOTH_LIBRARIES}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Let's use some filesystem features in our tests
  target_link_libraries(${TESTRUNNER_NAME} stdc++fs)
endif()

add_dependencies(${TESTRUNNER_NAME} ${PROJECT_NAME})
