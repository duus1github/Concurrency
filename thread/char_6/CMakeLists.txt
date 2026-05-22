cmake_minimum_required(VERSION 3.10)
project(char6 CXX)
set(CMAKE_CXX_STANDARD 17)


add_executable(char6 main.cpp)
target_link_libraries(char6 pthread)
