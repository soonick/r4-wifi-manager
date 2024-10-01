cmake_minimum_required(VERSION 3.27.4)
project(r4-wifi-manager
  VERSION 0.1.0
  LANGUAGES CXX)

add_subdirectory(deps/httpino)

add_library(crc
  "deps/CRC/src/CRC.cpp"
  "deps/CRC/src/CrcFastReverse.cpp"
  "deps/CRC/src/CRC8.cpp")
target_link_arduino_libraries(crc PUBLIC core)
target_include_directories("crc" PUBLIC "deps/CRC/src/")

add_library(r4-wifi-manager
  "/root/.arduino15/packages/arduino/hardware/renesas_uno/1.2.1/libraries/BlockDevices/virtualEEPROM.cpp"
  "/root/.arduino15/packages/arduino/hardware/renesas_uno/1.2.1/libraries/BlockDevices/DataFlashBlockDevice.cpp"
  "src/r4-wifi-manager.cpp"
  "src/eeprom-storage.cpp")
target_link_arduino_libraries(r4-wifi-manager PUBLIC core WiFiS3 EEPROM)
target_link_libraries(r4-wifi-manager PUBLIC httpino ArduinoHashtable crc)
target_include_directories(r4-wifi-manager PUBLIC "include")
# This is necessary for EEPROM library to work
target_include_directories(r4-wifi-manager PUBLIC "/root/.arduino15/packages/arduino/hardware/renesas_uno/1.2.1/libraries/BlockDevices")
