cmake_minimum_required(VERSION 3.0)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB ${ProjectName}Test_SRC
    "*.h"
    "*.cpp"
)
set(CMAKE_CXX_STANDARD 17)

add_executable(${ProjectName}Test ${${ProjectName}Test_SRC})
target_include_directories(${ProjectName}Test PRIVATE ${${ProjectName}_DIR} .)
target_link_libraries(${ProjectName}Test ${GTEST_BOTH_LIBRARIES} ${ProjectName}LIB)
