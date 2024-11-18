cmake_minimum_required(VERSION 3.10)
project(strings C)

set(CMAKE_C_STANDARD 17)

include_directories(include)

file(GLOB STR_SOURCES
  src/*.c
)

add_library(strlib STATIC ${STR_SOURCES})
set_target_properties(strlib PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib 
)

add_executable(test test/test.c)
target_link_libraries(test strlib)
