cmake_minimum_required(VERSION 2.6)
project(ffmpeguse)

add_executable(ffmpeguse main.cpp)

install(TARGETS ffmpeguse RUNTIME DESTINATION bin)
