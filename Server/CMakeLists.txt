cmake_minimum_required(VERSION 3.26)
project(untitled)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 23)

add_executable(untitled main.cpp)
target_link_libraries(untitled ${Boost_LIBRARIES} ws2_32 wsock32)  # Add the Winsock libraries here
