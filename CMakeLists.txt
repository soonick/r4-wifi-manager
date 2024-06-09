cmake_minimum_required(VERSION 3.27.4)
project(r4-wifi-manager
  VERSION 0.1.0
  LANGUAGES CXX)

add_subdirectory(deps/httpino)

add_library(r4-wifi-manager "src/r4-wifi-manager.cpp")
target_link_arduino_libraries(r4-wifi-manager PUBLIC core WiFiS3)
target_link_libraries(r4-wifi-manager PUBLIC httpino ArduinoHashtable)
target_include_directories(r4-wifi-manager PUBLIC "include")
