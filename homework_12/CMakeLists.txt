cmake_minimum_required(VERSION 3.17)
project(homework_12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR C:/SFML-2.5.1)
set(SFML_ROOT ${SFML_DIR})

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(task1 Task1/main.cpp Task1/visualizer.cpp Task1/calculator.cpp)

target_link_libraries(task1 sfml-graphics)

add_executable(task2 Task2/main.cpp Task2/link.cpp Task2/particle.cpp Task2/system.cpp)

target_link_libraries(task2 sfml-graphics)

add_executable(task3 Task3/main.cpp Task3/visualizer.cpp Task3/calculator.cpp)

target_link_libraries(task3 sfml-graphics)