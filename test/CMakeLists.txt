cmake_minimum_required(VERSION 3.27.4)
project(r4-wifi-manager CXX)

include_directories(../deps/ArduinoHashtable/src)
include_directories(../deps/Httpino/include)
include_directories(../deps/SimpleVector/src)
include_directories(../include)
include_directories(../src)
include_directories(external/catch/v2.13.10/include)
include_directories(include)

set(TESTING_SRCS
  ../src/r4-wifi-manager.cpp
)

set(INCLUDE_SRCS
  include/WiFiS3.cpp
  include/Arduino.cpp
)

set(TEST_TARGET_SRCS
  src/test-main.cpp
  src/r4-wifi-manager-test.cpp
)

add_compile_definitions(HOST)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(
  test
  ${TEST_TARGET_SRCS}
  ${INCLUDE_SRCS}
  ${TESTING_SRCS}
)

add_library(httpino ../deps/Httpino/src/httpino.cpp)
target_link_libraries(test PRIVATE httpino)
