cmake_minimum_required(VERSION 3.20)
project(images)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    set(OPT_FLAGS "-O3 -flto")
endif ()
set(CMAKE_CXX_FLAGS "--pedantic-errors -Wall -Werror ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ${OPT_FLAGS}")

add_executable(images src/main.cpp src/ImageGallery.cpp src/ImageGallery.hpp src/Image.cpp src/Image.hpp src/Nodiscard.hpp src/filesystem.hpp src/SizedTexture.cpp src/SizedTexture.hpp src/Subtarget.cpp src/Subtarget.hpp src/ImageViewer.cpp src/ImageViewer.hpp src/sfml/Vector2.inl src/sfml/Vector2.hpp src/sfml/Rect.inl src/sfml/Rect.hpp src/Button.cpp src/Button.hpp src/Directory.cpp src/Directory.hpp src/fitTextInBox.cpp src/fitTextInBox.hpp)
target_link_libraries(images sfml-graphics sfml-window sfml-system)
