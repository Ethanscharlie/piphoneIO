cmake_minimum_required(VERSION 3.18)

project(piphone CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(SIM ON)

# Find packages
find_package(GTest REQUIRED)
find_package(CURL REQUIRED)

set(sourceFiles 
    src/io/display.cpp
    src/io/font.cpp
    src/main.cpp
    src/menus/YouTubeMenu.cpp
    src/utils.cpp
    tests/test.cpp

    hardware/XML/tinyxml2.cpp
)

if (${SIM})
  find_package(SDL2 REQUIRED)

  add_executable(piphone
    ${sourceFiles}
  )

  target_include_directories(piphone PRIVATE
    include
    src
    hardware/XML
  )
else()
  find_library(BCM2835_LIB bcm2835)

  add_executable(piphone
    ${sourceFiles}

    hardware/OLED/SSD1306_OLED.cpp
    hardware/OLED/SSD1306_OLED_font.cpp
    hardware/OLED/SSD1306_OLED_graphics.cpp
    hardware/OLED/SSD1306_OLED_Print.cpp
  )

  target_include_directories(piphone PRIVATE
    include
    src
    hardware/OLED
    hardware/XML
  )
endif()

# External Libaries link
target_link_libraries(piphone PRIVATE 
  GTest::gtest_main
  CURL::libcurl
)

if (${SIM})
  target_link_libraries(piphone PRIVATE 
    SDL2::SDL2
  )

  target_compile_definitions(piphone PRIVATE 
    -DSIM 
  )
else()
  target_link_libraries(piphone PRIVATE 
    ${BCM2835_LIB}
  )
endif()
