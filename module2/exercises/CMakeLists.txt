cmake_minimum_required(VERSION 3.0)
project(data_sharing-exercises)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_definitions(-Wno-unused-variable)
# add_link_options(-fsanitize=thread)
add_definitions(-Wall -Werror -pedantic -Wextra)

file(GLOB SOURCES *.cpp)

foreach(FILE ${SOURCES})
    get_filename_component(BINNAME ${FILE} NAME_WE)
    add_executable(${BINNAME} ${FILE})
    target_link_libraries(${BINNAME} pthread -fsanitize=thread)
endforeach(FILE)
