cmake_minimum_required(VERSION 3.15)
project(Proj2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proj2 sim_bp.cpp sim_bp.h)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")