cmake_minimum_required(VERSION 3.17)
project(homework_11)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR C:/SFML-2.5.1)
set(SFML_ROOT ${SFML_DIR})

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(good_asteroids good_asteroids.cpp)

target_link_libraries(good_asteroids sfml-graphics)
add_executable(one_file one_file.cpp)

target_link_libraries(one_file sfml-graphics)