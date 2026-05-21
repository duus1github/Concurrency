cmake_minimum_required(VERSION 3.10)
project(char9 CXX)
set(CMAKE_CXX_STANDARD 17)


add_executable(char9 main.cpp)
target_link_libraries(char9 pthread)
