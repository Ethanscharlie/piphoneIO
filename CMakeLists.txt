cmake_minimum_required(VERSION 3.18)

project(piphone CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Find packages
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# Library
add_executable(piphone
  src/io/display.cpp
  tests/test.cpp
)

# Includes
target_include_directories(piphone PRIVATE
  include
  src
)

# External Libaries link
target_link_libraries(piphone PRIVATE 
  SDL2::SDL2
  GTest::gtest_main
)
