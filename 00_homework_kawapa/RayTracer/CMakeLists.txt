cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project)

set(SRC_LIST
    src/main.cpp
    src/algebra.cpp
    src/algebra.h
    src/binary.cpp
    src/chessboard.cpp
    src/chessboard.h
    src/cuboid.cpp
    src/debug.cpp
    src/imager.h
    src/lodepng.cpp
    src/lodepng.h
    src/optics.cpp
    src/reorient.cpp
    src/scene.cpp
    src/setcompl.cpp
    src/setisect.cpp
    src/setunion.cpp
    src/solid.cpp
    src/sphere.cpp
)

add_library(${PROJECT_NAME}-lib ${SRC_LIST})
add_executable(${PROJECT_NAME} src/main.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib -O3 pthread)
endif (UNIX)