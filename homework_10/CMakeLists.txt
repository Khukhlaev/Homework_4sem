cmake_minimum_required(VERSION 3.17)
project(homework_10)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS        ON)

find_package(Boost REQUIRED COMPONENTS locale)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

add_executable(Server Server.cpp)
add_executable(Client Client.cpp)
