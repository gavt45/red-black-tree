cmake_minimum_required(VERSION 3.16)
project(red_black_tree)

set(CMAKE_CXX_STANDARD 14)

add_compile_options("-Wall" "-Wpedantic" "-O3" "-v")
add_link_options("-v")

add_executable(red_black_tree RBTree.tpp main.cpp)
#RBTree.h