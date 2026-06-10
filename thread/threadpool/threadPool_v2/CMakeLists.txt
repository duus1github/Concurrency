cmake_minimum_required(VERSION 3.10)
project(pool CXX)
set(CMAKE_CXX_STANDARD 17)


add_executable(pool main.cpp)
target_link_libraries(pool pthread)
