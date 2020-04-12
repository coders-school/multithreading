cmake_minimum_required(VERSION 3.0)

project(homework4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wall -Werror -pedantic -Wextra)

add_definitions(-fsanitize=thread)

add_executable(Dinner philosophers.cpp)
target_link_libraries(Dinner pthread)

