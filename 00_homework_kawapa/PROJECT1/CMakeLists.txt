cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(main)

set(SRC_LIST
    src/main.cpp
    src/Philosopher.cpp
    src/Philosopher.hpp
    src/Book.cpp
    src/Book.hpp
    src/Reflection.cpp
    src/Reflection.hpp

)

add_library(${PROJECT_NAME}-lib ${SRC_LIST})
add_executable(${PROJECT_NAME} src/main.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib pthread)
endif (UNIX)
