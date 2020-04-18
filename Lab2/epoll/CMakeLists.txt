cmake_minimum_required(VERSION 3.10)
project(epoll)

#使用c11
add_compile_options(-std=c++11)

set(CMAKE_CXX_STANDARD 14)

add_executable(epoll main.cpp http_server.cpp handler.cpp debug_log.cpp)

