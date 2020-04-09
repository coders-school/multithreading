cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project)

set(SRC_LIST
    src/Fork.cpp
    src/Fork.hpp
    src/Philosopher.cpp
    src/Philosopher.hpp
    src/Table.hpp
    src/Table.cpp
)

add_library(${PROJECT_NAME}-lib ${SRC_LIST})
add_executable(${PROJECT_NAME} src/main.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib pthread)
endif (UNIX)
