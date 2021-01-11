cmake_minimum_required(VERSION 3.0)
project(module3-exercises)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-unused-variable)
add_definitions(-Wall -Werror -pedantic -Wextra -g)

file(GLOB SOURCES *.cpp)

foreach(FILE ${SOURCES})
    get_filename_component(BINNAME ${FILE} NAME_WE)
    add_executable(${BINNAME} ${FILE})
    target_link_libraries(${BINNAME} pthread -fsanitize=thread)
endforeach(FILE)
