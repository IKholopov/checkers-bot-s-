cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -fsanitize=address,undefined -fsanitize-undefined-trap-on-error -Wall" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall" )
set(CMAKE_CXX_FLAGS_FINAL "${CMAKE_CXX_FLAGS} -O4 -Wall -DFINAL" )
set(ProjectName MCST)
project(${ProjectName})
file(GLOB ${ProjectName}_SRC
    "*.h"
    "GameState.cpp"
)
set(${ProjectName}_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Include directories")
set(CMAKE_CXX_STANDARD 17)


add_library(${ProjectName}LIB ${${ProjectName}_SRC})
target_include_directories(${ProjectName}LIB PRIVATE ${${ProjectName}_DIR} .)
add_executable(${ProjectName}EXE main.cpp)
target_include_directories(${ProjectName}EXE PRIVATE ${${ProjectName}_DIR} .)
target_link_libraries(${ProjectName}EXE  ${ProjectName}LIB)
add_subdirectory(Tests)
