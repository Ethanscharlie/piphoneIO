cmake_minimum_required(VERSION 3.18)

project(piphoneIO CXX)
project(piphoneIO_test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Find packages
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# Library
add_library(piphoneIO STATIC
  src/piphone.cpp
)

# Includes
target_include_directories(piphoneIO PRIVATE
  include
  src
)

# External Libaries link
target_link_libraries(piphoneIO PRIVATE 
  SDL2::SDL2
  GTest::gtest_main
)

#### Testing ##########################################################################
add_executable(piphoneIO_test
  tests/test.cpp
)

target_link_libraries(piphoneIO_test PRIVATE 
  piphoneIO
  SDL2::SDL2
  GTest::gtest_main
)

target_include_directories(piphoneIO_test PRIVATE
  include
)

# include(GoogleTest)
# gtest_discover_tests(piphoneIO_test)

#### Testing ##########################################################################
