cmake_minimum_required(VERSION 3.10)
project(char8 CXX)
set(CMAKE_CXX_STANDARD 17)


add_executable(char8 main.cpp)
target_link_libraries(char8 pthread)
