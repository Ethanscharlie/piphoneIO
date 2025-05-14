cmake_minimum_required(VERSION 3.18)

project(piphone CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(SIM ON)
ADD_DEFINITIONS( -D_LOGICAL_WIDTH=\"${LOGICAL_WIDTH}\")

# Find packages
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# Library
add_executable(piphone
  src/io/display.cpp
  tests/test.cpp
)

if (NOT ${SIM})
  add_executable(piphone
    hardware/OLED/SSD1306_OLED.cpp
    hardware/OLED/SSD1306_OLED_font.cpp
    hardware/OLED/SSD1306_OLED_graphics.cpp
    hardware/OLED/SSD1306_OLED_Print.cpp
  )

  target_include_directories(piphone PRIVATE
    hardware
  )
endif()

# Includes
target_include_directories(piphone PRIVATE
  include
  src
)

# External Libaries link
target_link_libraries(piphone PRIVATE 
  GTest::gtest_main
)

if (${SIM})
  target_link_libraries(piphone PRIVATE 
    SDL2::SDL2
  )

  target_compile_definitions(piphone PRIVATE 
    -DSIM 
  )
endif()
