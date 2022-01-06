cmake_minimum_required(VERSION 3.20)
project(images)

set(CMAKE_CXX_STANDARD 17)

add_executable(images src/main.cpp)
target_link_libraries(images sfml-graphics sfml-window sfml-system)
