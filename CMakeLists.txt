cmake_minimum_required(VERSION 3.10)
project(MatrixTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(matrix_test_rudakov rudakov.cpp)

target_link_libraries(matrix_test_rudakov
    GTest::GTest
    GTest::Main
)

gtest_discover_tests(matrix_test_rudakov)