cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

add_definitions(-Wno-unused-variable)
add_definitions(-Wall -Werror -pedantic -Wextra -fsanitize=thread)
#add_link_options(-fsanitize=thread)

file(GLOB SOURCES *.cpp)

set(BINNAME dining_philosophers)
add_executable(${BINNAME} ${SOURCES})
target_link_libraries(${BINNAME} pthread -fsanitize=thread)

