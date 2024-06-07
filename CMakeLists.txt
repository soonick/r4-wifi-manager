cmake_minimum_required(VERSION 3.27.4)
project(r4-wifi-manager CXX)

add_subdirectory(deps/Httpino)

include_directories(
  deps/ArduinoHashtable/src
  deps/SimpleVector/src
  deps/Httpino/include
)

add_library(r4-wifi-manager src/r4-wifi-manager.cpp)
target_link_arduino_libraries(r4-wifi-manager PRIVATE core WiFiS3)
target_link_libraries(r4-wifi-manager PRIVATE httpino ArduinoHashtable)
target_include_directories(r4-wifi-manager PRIVATE include)
