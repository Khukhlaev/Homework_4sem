cmake_minimum_required(VERSION 3.17)
project(homework_5)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS        ON)

find_package(Boost REQUIRED COMPONENTS locale)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})
add_executable(Task2 Task2.cpp)
target_include_directories(Task2 PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(Task2 ${Boost_LIBRARIES})

add_executable(Task1 Task1.cpp)
add_executable(Task3 Task3.cpp)
add_executable(Task4 Task4.cpp)