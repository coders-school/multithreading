cmake_minimum_required(VERSION 3.0)

project(homework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wall -Werror -pedantic -Wextra)

add_executable(${PROJECT_NAME} hw2_task4.cpp)
target_link_libraries(${PROJECT_NAME} pthread -fsanitize=thread)
