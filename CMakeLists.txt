cmake_minimum_required(VERSION 3.1)

project(SingleLinkedList)


include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(SingleLinkedList src/main.cpp)

