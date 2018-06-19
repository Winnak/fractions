cmake_minimum_required(VERSION 3.10)
project(fractions)

set(CMAKE_CXX_STANDARD 17)

add_executable(fractions_test tests/test.cpp fractions.h)