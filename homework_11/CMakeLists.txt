cmake_minimum_required(VERSION 3.17)
project(homework_11)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR C:/SFML-2.5.1)
set(SFML_ROOT ${SFML_DIR})

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(asteroids asteroids.cpp)

target_link_libraries(asteroids sfml-graphics)

add_executable(problems problems.cpp)

target_link_libraries(problems sfml-graphics)

add_executable(good_asteroids good_asteroids.cpp)

target_link_libraries(good_asteroids sfml-graphics)