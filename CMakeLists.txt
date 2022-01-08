cmake_minimum_required(VERSION 3.20)
project(images)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS --pedantic-errors)

add_executable(images src/main.cpp src/ImageDisplay.cpp src/ImageDisplay.hpp src/ImageLoader.cpp src/ImageLoader.hpp src/Image.cpp src/Image.hpp src/Tag.cpp src/Tag.hpp src/Globals.hpp src/filesystem.hpp src/SizedTexture.cpp src/SizedTexture.hpp)
target_link_libraries(images sfml-graphics sfml-window sfml-system)
